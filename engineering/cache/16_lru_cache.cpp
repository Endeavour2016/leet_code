/**
 * @file 16_lru_cache.cpp
 * @brief 实现按条目数限制容量的 LRU 缓存。
 * @details 哈希表定位双向链表节点，命中和更新时移到表头，容量满时淘汰表尾。
 * @note 仅支持单线程或外部串行访问；容量为零时拒绝写入，不恢复内存分配失败。
 * @see notes/engineering/16_lru_cache.md
 * @par 运行方式
 * 在仓库根目录执行：make run eng_lru_cache
 */

#include <cstddef>
#include <iostream>
#include <limits>
#include <list>
#include <random>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../common/check.h"

class LruCache {
 public:
  explicit LruCache(std::size_t capacity) : capacity_(capacity) {
  }

  // 哈希表保存本对象链表的迭代器；默认复制会错误地引用原对象的节点。
  // 教学实现不提供复制或移动，避免引入额外的跨容器生命周期约定。
  LruCache(const LruCache&) = delete;
  LruCache& operator=(const LruCache&) = delete;
  LruCache(LruCache&&) = delete;
  LruCache& operator=(LruCache&&) = delete;

  // true 表示命中并更新最近使用顺序；false 表示未命中，output 保持不变。
  bool Get(int key, int& output) {
    auto found = index_.find(key);
    if (found == index_.end()) {
      return false;
    }
    entries_.splice(entries_.begin(), entries_, found->second);
    output = found->second->second;
    return true;
  }

  // true 表示已插入或更新；容量为零时返回 false，缓存保持不变。
  // 资源分配失败不在 bool 的可恢复失败范围内。
  bool Put(int key, int value) {
    if (capacity_ == 0) {
      return false;
    }
    auto found = index_.find(key);
    if (found != index_.end()) {
      found->second->second = value;
      entries_.splice(entries_.begin(), entries_, found->second);
      return true;
    }
    if (entries_.size() == capacity_) {
      // 先删除哈希表中的引用，再销毁链表节点，避免保存悬空迭代器。
      index_.erase(entries_.back().first);
      entries_.pop_back();
    }
    entries_.emplace_front(key, value);
    index_.emplace(key, entries_.begin());
    return true;
  }

  std::size_t size() const {
    return entries_.size();
  }

 private:
  typedef std::list<std::pair<int, int>> EntryList;
  const std::size_t capacity_;
  EntryList entries_;  // 从表头到表尾：最近使用到最久未使用。
  std::unordered_map<int, EntryList::iterator> index_;
};

// 以下为验证代码。基线只用数组线性查找，避免重复实现哈希表与链表的关联。
namespace {
class ReferenceCache {
 public:
  explicit ReferenceCache(std::size_t capacity) : capacity_(capacity) {
  }

  bool Get(int key, int& output) {
    for (std::size_t i = 0; i < entries_.size(); ++i) {
      if (entries_[i].first == key) {
        const auto entry = entries_[i];
        entries_.erase(entries_.begin() + i);
        entries_.push_back(entry);
        output = entry.second;
        return true;
      }
    }
    return false;
  }

  bool Put(int key, int value) {
    if (capacity_ == 0) {
      return false;
    }
    for (std::size_t i = 0; i < entries_.size(); ++i) {
      if (entries_[i].first == key) {
        entries_.erase(entries_.begin() + i);
        entries_.push_back(std::make_pair(key, value));
        return true;
      }
    }
    if (entries_.size() == capacity_) {
      entries_.erase(entries_.begin());
    }
    entries_.push_back(std::make_pair(key, value));
    return true;
  }

  std::size_t size() const {
    return entries_.size();
  }

 private:
  const std::size_t capacity_;
  std::vector<std::pair<int, int>> entries_;  // 最久未使用的元素在开头。
};

void TestBoundariesAndOrder() {
  int value = 123;
  LruCache disabled(0);
  CHECK(!disabled.Put(1, 10));
  CHECK(!disabled.Get(1, value));
  CHECK(value == 123);
  CHECK(disabled.size() == 0);

  LruCache single(1);
  CHECK(!single.Get(1, value));
  CHECK(value == 123);
  CHECK(single.Put(1, -1));
  CHECK(single.Get(1, value) && value == -1);
  CHECK(single.Put(1, 20));
  CHECK(single.size() == 1);
  CHECK(single.Get(1, value) && value == 20);
  CHECK(single.Put(2, 30));
  CHECK(!single.Get(1, value) && value == 20);
  CHECK(single.Get(2, value) && value == 30);

  LruCache cache(2);
  CHECK(cache.Put(1, 10));
  CHECK(cache.Put(2, 20));
  CHECK(cache.Get(1, value) && value == 10);  // 顺序：1、2。
  CHECK(cache.Get(1, value) && value == 10);  // 重复命中表头也是合法操作。
  CHECK(!cache.Get(9, value) && value == 10);
  CHECK(cache.Put(3, 30));  // 未命中不改变顺序，淘汰 2。
  CHECK(!cache.Get(2, value) && value == 10);
  CHECK(cache.Put(1, 100));  // 更新旧键也要变成最近使用。
  CHECK(cache.size() == 2);
  CHECK(cache.Put(4, 40));  // 淘汰 3，不能淘汰刚更新的 1。
  CHECK(!cache.Get(3, value) && value == 10);
  CHECK(cache.Get(1, value) && value == 100);
  CHECK(cache.Get(4, value) && value == 40);
  CHECK(cache.Put(3, 300));  // 被淘汰的键可以重新插入。
  CHECK(!cache.Get(1, value) && value == 40);
  CHECK(cache.Get(3, value) && value == 300);

  LruCache extremes(2);
  CHECK(extremes.Put(std::numeric_limits<int>::min(),
                     std::numeric_limits<int>::max()));
  CHECK(extremes.Put(std::numeric_limits<int>::max(),
                     std::numeric_limits<int>::min()));
  CHECK(extremes.Get(std::numeric_limits<int>::min(), value));
  CHECK(value == std::numeric_limits<int>::max());
  CHECK(extremes.Get(std::numeric_limits<int>::max(), value));
  CHECK(value == std::numeric_limits<int>::min());
}

void TestAgainstReference() {
  std::mt19937 random(42);
  const std::size_t capacities[] = {0, 1, 2, 7, 64};
  for (std::size_t capacity : capacities) {
    LruCache cache(capacity);
    ReferenceCache reference(capacity);
    for (int step = 0; step < 20000; ++step) {
      const int key = static_cast<int>(random() % 101) - 50;
      if (random() % 2 == 0) {
        const int value = static_cast<int>(random() % 2001) - 1000;
        CHECK(cache.Put(key, value) == reference.Put(key, value));
      } else {
        int actual = 123456;
        int expected = 123456;
        CHECK(cache.Get(key, actual) == reference.Get(key, expected));
        CHECK(actual == expected);
      }
      CHECK(cache.size() == reference.size());
      CHECK(cache.size() <= capacity);
    }
    // 最后逐键核对内容；两个实现按相同顺序访问，最近使用顺序也同步改变。
    for (int key = -50; key <= 50; ++key) {
      int actual = 123456;
      int expected = 123456;
      CHECK(cache.Get(key, actual) == reference.Get(key, expected));
      CHECK(actual == expected);
    }
  }
}
}  // namespace

int main() {
  static_assert(!std::is_copy_constructible<LruCache>::value,
                "LruCache must not copy iterators into another cache");
  static_assert(!std::is_move_constructible<LruCache>::value,
                "LruCache does not support move construction");
  TestBoundariesAndOrder();
  TestAgainstReference();
  std::cout << "LRU cache checks passed (100000 random operations).\n";
}
