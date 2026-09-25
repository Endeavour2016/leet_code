/**
 * @file 11_inverted_index.cpp
 * @brief 实现支持多词查询和逻辑删除的最小倒排索引。
 * @details 将外部 ID 映射为内部编号，按词建立列表，查询求交后过滤无效文档。
 * @note 单线程追加；外部 ID 不复用；更新与版本选择见第 13 题。
 * @see notes/engineering/11_inverted_index.md
 * @par 运行方式
 * 在仓库根目录执行：make run eng_inverted_index
 */

#include <algorithm>
#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "../common/check.h"
#include "../common/posting.h"

// 只追加新文档；删除时标记无效，不立即修改各词的倒排列表。
class InvertedIndex {
 public:
  // 外部 ID 重复或编号耗尽时返回 false，且不修改索引。
  // 同一文档的词先去重，再追加递增编号，使各列表自然有序。
  bool add(const std::string& external, const std::vector<std::string>& terms) {
    if (ids_.count(external)) {
      return false;
    }
    if (external_.size() > UINT32_MAX) {
      return false;
    }
    const auto doc = static_cast<std::uint32_t>(external_.size());
    ids_[external] = doc;
    external_.push_back(external);
    live_.push_back(true);
    for (const auto& term : std::set<std::string>(terms.begin(), terms.end())) {
      postings_[term].push_back(doc);
    }
    return true;
  }

  // 逻辑删除只修改有效标志；旧倒排记录留待将来的段合并清理。
  bool erase(const std::string& external) {
    auto it = ids_.find(external);
    if (it == ids_.end() || !live_[it->second]) {
      return false;
    }
    live_[it->second] = false;
    return true;
  }

  // 从最短列表开始求交，最后过滤已删除文档并返回业务 ID。
  std::vector<std::string> search_all(
      const std::vector<std::string>& terms) const {
    if (terms.empty()) {
      return {};  // 本题约定空查询返回空结果。
    }
    std::vector<const Posting*> lists;
    for (const auto& term : terms) {
      auto it = postings_.find(term);
      if (it == postings_.end()) {
        return {};
      }
      lists.push_back(&it->second);
    }
    std::sort(
        lists.begin(), lists.end(), [](const Posting* a, const Posting* b) {
          return a->size() < b->size();
        });
    Posting candidates = *lists.front();
    for (std::size_t i = 1; i < lists.size() && !candidates.empty(); ++i) {
      candidates = posting_and(candidates, *lists[i]);
    }
    std::vector<std::string> result;
    for (auto doc : candidates) {
      if (live_[doc]) {
        result.push_back(external_[doc]);
      }
    }
    return result;
  }

 private:
  std::map<std::string, std::uint32_t> ids_;
  std::vector<std::string> external_;
  std::vector<bool> live_;
  std::map<std::string, Posting> postings_;
};

// 以下为验证代码；复习时先阅读上方实现，main 用于检查正常和边界情况。
int main() {
  InvertedIndex index;
  CHECK(index.add("ad-A", {"phone", "phone", "sale"}));
  CHECK(index.add("ad-B", {"phone"}));
  CHECK(index.add("ad-C", {"sale"}));
  CHECK(index.add("empty", {}));
  CHECK(index.search_all({"phone", "sale"}) ==
        std::vector<std::string>({"ad-A"}));
  CHECK(index.search_all({"phone", "phone"}) ==
        std::vector<std::string>({"ad-A", "ad-B"}));
  CHECK(index.search_all({}).empty());
  CHECK(index.search_all({"missing"}).empty());
  CHECK(index.erase("ad-A"));
  CHECK(!index.erase("ad-A"));
  CHECK(!index.erase("unknown"));
  CHECK(index.search_all({"phone"}) == std::vector<std::string>({"ad-B"}));
  CHECK(!index.add("ad-A", {}));
  CHECK(index.search_all({"phone"}) == std::vector<std::string>({"ad-B"}));
}
