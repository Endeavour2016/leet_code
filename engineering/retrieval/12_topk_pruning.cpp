/**
 * @file 12_topk_pruning.cpp
 * @brief 演示不改变 Top K 结果的块级评分剪枝。
 * @details 使用预先计算的块分数上界，跳过不可能进入前 K 名的文档。
 * @note 采用非负整数加法评分；同分按 docID 升序；不实现完整 WAND 算法。
 * @see notes/engineering/12_topk_pruning.md
 * @par 运行方式
 * 在仓库根目录执行：make run eng_topk_pruning
 */

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <queue>
#include <random>
#include <utility>
#include <vector>

#include "../common/check.h"
#include "../common/hit.h"

// 两个查询词的分数相加；eligible 表示文档是否符合业务条件。
struct Document {
  std::uint32_t doc;
  std::uint32_t term_a;
  std::uint32_t term_b;
  bool eligible;
};

// 上界不得低于块内任何文档的实际总分，否则可能错误跳过正确结果。
struct Block {
  std::vector<Document> docs;
  std::uint64_t upper_bound;
};

// 预先构建各块的分数上界，查询时不能为计算上界再次完整评分。
// block_size 为 0 时返回 false，output 保持不变。
bool build_blocks(const std::vector<Document>& docs,
                  std::size_t block_size,
                  std::vector<Block>& output) {
  if (!block_size) {
    return false;
  }

  std::vector<Block> blocks;
  for (std::size_t start = 0; start < docs.size();) {
    const auto end = start + std::min(block_size, docs.size() - start);
    Block block;
    std::uint32_t max_a = 0;
    std::uint32_t max_b = 0;
    for (auto i = start; i < end; ++i) {
      block.docs.push_back(docs[i]);
      max_a = std::max(max_a, docs[i].term_a);
      max_b = std::max(max_b, docs[i].term_b);
    }
    block.upper_bound = std::uint64_t(max_a) + max_b;
    blocks.push_back(std::move(block));
    start = end;
  }

  output.swap(blocks);
  return true;
}

// 完整评分并排序，作为剪枝算法的正确性对照。
std::vector<Hit> exhaustive(const std::vector<Document>& docs, std::size_t k) {
  std::vector<Hit> result;
  for (const auto& d : docs) {
    if (d.eligible) {
      result.push_back({d.doc, std::uint64_t(d.term_a) + d.term_b});
    }
  }

  std::sort(result.begin(), result.end(), BetterHit());
  if (result.size() > k) {
    result.resize(k);
  }

  return result;
}

// 堆未满时不剪枝；scored 记录实际评分的合格文档数。
std::vector<Hit> topk(const std::vector<Block>& blocks,
                      std::size_t k,
                      std::size_t& scored) {
  scored = 0;
  std::priority_queue<Hit, std::vector<Hit>, BetterHit>
      best;  // 堆顶是当前保留结果中最差的一项。
  if (!k) {
    return {};
  }

  for (const auto& block : blocks) {
    // 上界等于当前最低分时仍要检查：更小的 docID 可能赢得同分排序。
    if (best.size() == k && block.upper_bound < best.top().score) {
      continue;
    }
    for (const auto& d : block.docs) {
      if (d.eligible) {
        ++scored;
        const Hit hit = {d.doc, std::uint64_t(d.term_a) + d.term_b};
        if (best.size() < k) {
          best.push(hit);
        } else if (BetterHit()(hit, best.top())) {
          best.pop();
          best.push(hit);
        }
      }
    }
  }

  std::vector<Hit> result;
  while (!best.empty()) {
    result.push_back(best.top());
    best.pop();
  }

  std::sort(result.begin(), result.end(), BetterHit());
  return result;
}

// 以下为验证代码；复习时先阅读上方实现，main 用于检查正常和边界情况。
int main() {
  std::vector<Document> docs = {{9, 10, 0, true},
                                {1, 10, 0, true},
                                {3, 1, 0, true},
                                {4, 999, 999, false}};
  std::size_t scored;
  std::vector<Block> blocks;
  CHECK(build_blocks(docs, 1, blocks));
  CHECK(topk(blocks, 1, scored) == exhaustive(docs, 1));
  CHECK(scored == 2);  // 检查同分文档，跳过低分块，并在评分前排除不合格文档。
  CHECK(topk({}, 3, scored).empty());
  CHECK(build_blocks(docs, 2, blocks));
  CHECK(topk(blocks, 0, scored).empty());
  CHECK(!build_blocks(docs, 0, blocks));
  CHECK(blocks.size() == 2);
  CHECK(build_blocks({}, 1, blocks));
  CHECK(blocks.empty());
  std::mt19937 rng(42);
  for (int round = 0; round < 50; ++round) {
    docs.clear();
    for (std::uint32_t i = 0; i < 300; ++i) {
      docs.push_back({i,
                      static_cast<std::uint32_t>(rng() % 100),
                      static_cast<std::uint32_t>(rng() % 100),
                      rng() % 3 != 0});
    }
    std::shuffle(docs.begin(), docs.end(), rng);
    CHECK(build_blocks(docs, 11, blocks));
    for (std::size_t k : {0u, 1u, 10u, 500u}) {
      CHECK(topk(blocks, k, scored) == exhaustive(docs, k));
    }
  }

  docs = {{0, UINT32_MAX, UINT32_MAX, true}, {1, 0, 0, true}};
  CHECK(build_blocks(docs, 1, blocks));
  CHECK(topk(blocks, 1, scored) == exhaustive(docs, 1));
}
