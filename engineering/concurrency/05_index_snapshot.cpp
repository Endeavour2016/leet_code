/**
 * @file 05_index_snapshot.cpp
 * @brief 演示不可变索引快照的发布和旧版本释放。
 * @details 用 C++11 shared_ptr 原子操作替换当前版本，查询持有引用直到读取结束。
 * @note 示例由单个更新线程按版本顺序发布；shared_ptr 原子操作不保证无锁。
 * @see notes/engineering/05_index_snapshot.md
 * @par 运行方式
 * 在仓库根目录执行：make run eng_index_snapshot
 */

#include <atomic>
#include <cstdint>
#include <future>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

#include "../common/check.h"

// 每个快照对应一个完整版本，发布后不允许再修改。
struct Snapshot {
  std::uint64_t version;
  std::vector<int> documents;
  Snapshot(std::uint64_t v, std::vector<int> d)
      : version(v), documents(std::move(d)) {
  }
};

// 当前版本可被原子替换；查询线程通过 shared_ptr 保留旧版本。
class SnapshotStore {
 public:
  SnapshotStore()
      : current_(std::make_shared<const Snapshot>(0, std::vector<int>())) {
  }

  // 由一个更新线程按版本顺序调用；多个更新线程须在外部协调顺序。
  void publish(std::uint64_t version, std::vector<int> documents) {
    auto next = std::make_shared<const Snapshot>(version, std::move(documents));
    std::atomic_store_explicit(&current_, next, std::memory_order_release);
  }

  // 返回自己的引用，保证查询期间对象不会被更新线程提前销毁。
  std::shared_ptr<const Snapshot> acquire() const {
    return std::atomic_load_explicit(&current_, std::memory_order_acquire);
  }

 private:
  std::shared_ptr<const Snapshot>
      current_;  // 并发读写必须使用 shared_ptr 的原子读写函数。
};

// 以下为验证代码；复习时先阅读上方实现，main 用于检查正常和边界情况。
int main() {
  SnapshotStore store;
  store.publish(1, {10, 20});
  // weak_ptr 不延长生命周期，用于检查旧查询结束后快照是否已释放。
  std::weak_ptr<const Snapshot> retired;
  std::promise<void> pinned;
  std::promise<void> changed;
  auto changed_future = changed.get_future();
  std::atomic<bool> correct(false);
  std::thread reader([&] {
    auto old = store.acquire();
    retired = old;
    pinned.set_value();
    changed_future.wait();
    correct.store(old->version == 1 &&
                  old->documents == std::vector<int>({10, 20}));
    auto latest = store.acquire();
    if (latest->version != 2 || latest->documents != std::vector<int>({30})) {
      correct.store(false);
    }
  });
  pinned.get_future().wait();
  store.publish(2, {30});
  CHECK(!retired.expired());
  changed.set_value();
  reader.join();
  CHECK(correct.load());
  CHECK(retired.expired());
}
