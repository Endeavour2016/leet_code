/**
 * @file 14_shard_merge.cpp
 * @brief 将多个分片的有序结果合并为全局 Top K。
 * @details 堆中保存每个分片的下一个候选，取出最佳结果后只推进它所在的分片。
 * @note 各片使用统一评分和同分规则，文档不跨片重复；缺失分片需标明结果不完整。
 * @see notes/engineering/14_shard_merge.md
 * @par 运行方式
 * 在仓库根目录执行：make run eng_shard_merge
 */

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <queue>
#include <random>
#include <vector>

#include "../common/check.h"
#include "../common/hit.h"

// complete 为 false 时，本例忽略该分片的文档列表。
struct ShardResult {
  bool complete;
  std::vector<Hit> hits;
};

// 即使返回了足够的文档，也必须保留分片缺失的信息。
struct SearchResult {
  bool partial;
  std::vector<Hit> hits;
};

struct Cursor {
  std::size_t shard;
  std::size_t position;
  Hit hit;
};

// priority_queue 将“不是更好的候选”放在后面，使堆顶始终最好。
struct LaterCursor {
  bool operator()(const Cursor& a, const Cursor& b) const {
    return BetterHit()(b.hit, a.hit);
  }
};

// 各分片提供有序的局部 Top K，使用统一分数，且文档不跨片重复。
// 忽略未完整返回的分片，并在结果中明确标记不完整。
SearchResult merge_shards(const std::vector<ShardResult>& shards,
                          std::size_t k) {
  SearchResult result = {false, {}};
  std::priority_queue<Cursor, std::vector<Cursor>, LaterCursor> heap;
  for (std::size_t i = 0; i < shards.size(); ++i) {
    if (!shards[i].complete) {
      result.partial = true;
      continue;
    }
    if (!shards[i].hits.empty()) {
      heap.push({i, 0, shards[i].hits[0]});
    }
  }

  while (!heap.empty() && result.hits.size() < k) {
    Cursor current = heap.top();
    heap.pop();
    result.hits.push_back(current.hit);
    // 只推进刚输出结果的分片，其他分片的当前候选保持不变。
    ++current.position;
    if (current.position < shards[current.shard].hits.size()) {
      current.hit = shards[current.shard].hits[current.position];
      heap.push(current);
    }
  }

  return result;
}

// 以下为验证代码；复习时先阅读上方实现，main 用于检查正常和边界情况。
int main() {
  std::vector<ShardResult> shards = {
      {true, {{1, 10}, {3, 5}}}, {true, {{2, 10}, {4, 1}}}, {true, {}}};
  auto result = merge_shards(shards, 3);
  CHECK(!result.partial);
  CHECK(result.hits == std::vector<Hit>({{1, 10}, {2, 10}, {3, 5}}));
  shards[1].complete = false;
  result = merge_shards(shards, 9);
  CHECK(result.partial);
  CHECK(result.hits == shards[0].hits);
  CHECK(merge_shards(shards, 0).hits.empty());
  CHECK(merge_shards(shards, 0).partial);
  CHECK(!merge_shards({}, 3).partial);
  std::mt19937 rng(99);
  for (std::size_t k : {1u, 10u, 100u, 1000u}) {
    std::vector<ShardResult> inputs(7, ShardResult{true, {}});
    std::vector<Hit> baseline;
    for (std::uint32_t i = 0; i < 400; ++i) {
      Hit hit = {i, rng() % 100};
      inputs[i % 7].hits.push_back(hit);
      baseline.push_back(hit);
    }
    for (auto& shard : inputs) {
      std::sort(shard.hits.begin(), shard.hits.end(), BetterHit());
      if (shard.hits.size() > k) {
        shard.hits.resize(k);
      }
    }
    std::sort(baseline.begin(), baseline.end(), BetterHit());
    if (baseline.size() > k) {
      baseline.resize(k);
    }
    CHECK(merge_shards(inputs, k).hits == baseline);
  }
}
