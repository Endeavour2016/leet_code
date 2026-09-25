/**
 * @file 13_realtime_segments.cpp
 * @brief 演示实时索引的暂存、刷新、版本选择和段合并。
 * @details 查询先选择每个外部 ID 的最高已发布版本，再检查删除标记和查询词。
 * @note 仅为单线程内存模型；合并保留删除标记，不包含日志或磁盘恢复。
 * @see notes/engineering/13_realtime_segments.md
 * @par 运行方式
 * 在仓库根目录执行：make run eng_realtime_segments
 */

#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "../common/check.h"

// 删除也保存版本号，以便拒绝旧版本重新写入。
struct Record {
  std::uint64_t version;
  bool deleted;
  std::set<std::string> terms;
};

typedef std::map<std::string, Record> Segment;
// 暂存数据只有 refresh 后才参与查询；所有方法由同一线程调用。
class RealtimeIndex {
 public:
  // 只接受更高版本；接收成功不代表当前查询已经可见。
  bool write(const std::string& id,
             std::uint64_t version,
             bool deleted,
             const std::set<std::string>& terms) {
    auto it = latest_version_.find(id);
    if (it != latest_version_.end() && version <= it->second) {
      return false;
    }
    pending_[id] = Record{version, deleted, terms};
    latest_version_[id] = version;
    return true;
  }

  // 将暂存更新转换为新的只读段，然后清空暂存区。
  void refresh() {
    if (!pending_.empty()) {
      segments_.push_back(pending_);
      pending_.clear();
    }
  }

  std::vector<std::string> search(const std::string& term) const {
    // 必须先选最新版本，否则旧版本中的词可能错误地再次匹配。
    Segment visible = resolve();
    std::vector<std::string> result;
    for (const auto& pair : visible) {
      if (!pair.second.deleted && pair.second.terms.count(term)) {
        result.push_back(pair.first);
      }
    }
    return result;
  }

  // 仅合并已发布的段；不提前暴露暂存更新，不丢弃删除标记。
  void merge() {
    Segment compact = resolve();
    std::vector<Segment> next;
    if (!compact.empty()) {
      next.push_back(std::move(compact));
    }
    segments_.swap(next);  // 保留删除标记，不让尚未刷新的更新提前参与查询。
  }

 private:
  // 对每个外部 ID 选择已发布段中的最高版本。
  Segment resolve() const {
    Segment latest;
    for (const auto& segment : segments_) {
      for (const auto& pair : segment) {
        auto it = latest.find(pair.first);
        if (it == latest.end() || pair.second.version > it->second.version) {
          latest[pair.first] = pair.second;
        }
      }
    }
    return latest;
  }

  Segment pending_;
  std::vector<Segment> segments_;
  std::map<std::string, std::uint64_t> latest_version_;
};

// 以下为验证代码；复习时先阅读上方实现，main 用于检查正常和边界情况。
int main() {
  RealtimeIndex index;
  CHECK(index.write("A", 1, false, {"old"}));
  CHECK(index.search("old").empty());
  index.refresh();
  CHECK(index.search("old") == std::vector<std::string>({"A"}));
  CHECK(index.write("A", 2, false, {"new"}));
  index.merge();
  CHECK(index.search("old") == std::vector<std::string>({"A"}));
  index.refresh();
  CHECK(index.search("old").empty());
  CHECK(index.search("new").size() == 1);
  CHECK(!index.write("A", 1, false, {"old"}));
  CHECK(!index.write("A", 2, false, {"different"}));
  CHECK(index.write("A", 3, true, {}));
  index.refresh();
  CHECK(index.search("new").empty());
  index.merge();
  CHECK(index.search("new").empty());
  CHECK(!index.write("A", 2, false, {"new"}));
  CHECK(index.write("A", 4, false, {"again"}));
  index.refresh();
  CHECK(index.write("B", 1, false, {"again"}));
  index.refresh();
  auto before = index.search("again");
  index.merge();
  CHECK(index.search("again") == before);
  CHECK(before == std::vector<std::string>({"A", "B"}));
}
