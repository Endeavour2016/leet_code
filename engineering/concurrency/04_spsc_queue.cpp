/**
 * @file 04_spsc_queue.cpp
 * @brief 实现单生产者、单消费者的固定容量环形队列。
 * @details 通过 acquire/release 操作保证写完后才能读取、读完后才能覆盖。
 * @note N 个数组位置可存 N-1 个元素；不支持多个生产者或消费者。
 * @see notes/engineering/04_spsc_queue.md
 * @par 运行方式
 * 在仓库根目录执行：make run eng_spsc_queue
 */

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <thread>

#include "../common/check.h"

// 只允许一个生产者和一个消费者；留一个空位区分满和空。
// 固定整数元素使核心逻辑不涉及动态分配或复杂的对象移动。
template <std::size_t N>
class SpscQueue {
  static_assert(N >= 2, "at least two slots");

 public:
  SpscQueue() : head_(0), tail_(0) {
  }

  // 只有生产者调用；满时返回 false，不等待消费者。
  bool try_push(std::uint64_t value) {
    const std::size_t tail = tail_.load(std::memory_order_relaxed);
    const std::size_t next = (tail + 1) % N;
    if (next == head_.load(std::memory_order_acquire)) {
      return false;
    }
    // 先写入数据，再用 release 更新 tail，让消费者读取完整元素。
    data_[tail] = value;
    tail_.store(next, std::memory_order_release);
    return true;
  }

  // 只有消费者调用；空时返回 false，不等待生产者。
  bool try_pop(std::uint64_t& value) {
    const std::size_t head = head_.load(std::memory_order_relaxed);
    if (head == tail_.load(std::memory_order_acquire)) {
      return false;
    }
    value = data_[head];
    // 先读完数据，再用 release 更新 head，允许生产者复用该位置。
    head_.store((head + 1) % N, std::memory_order_release);
    return true;
  }

  // 运行时确认当前平台的下标原子类型是否以无锁方式实现。
  bool indices_are_lock_free() const {
    return head_.is_lock_free() && tail_.is_lock_free();
  }

 private:
  std::array<std::uint64_t, N> data_;
  // 将下标放到不同缓存行是额外优化，C++11 的动态内存对齐需单独处理。
  std::atomic<std::size_t> head_;
  std::atomic<std::size_t> tail_;
};

// 以下为验证代码；复习时先阅读上方实现，main 用于检查正常和边界情况。
int main() {
  SpscQueue<2> small;
  std::uint64_t value = 0;
  CHECK(!small.try_pop(value));
  for (std::uint64_t i = 0; i < 100; ++i) {
    CHECK(small.try_push(i));
    CHECK(!small.try_push(99));
    CHECK(small.try_pop(value) && value == i);
    CHECK(!small.try_pop(value));
  }

  SpscQueue<127> queue;
  const std::uint64_t total = 100000;
  std::atomic<bool> correct(true);
  std::thread consumer([&] {
    for (std::uint64_t i = 0; i < total; ++i) {
      std::uint64_t actual;
      while (!queue.try_pop(actual)) {
        std::this_thread::yield();
      }
      if (actual != i) {
        correct.store(false, std::memory_order_relaxed);
      }
    }
  });
  std::thread producer([&] {
    for (std::uint64_t i = 0; i < total; ++i) {
      while (!queue.try_push(i)) {
        std::this_thread::yield();
      }
    }
  });
  producer.join();
  consumer.join();
  CHECK(correct.load());
  CHECK(!queue.try_pop(value));
  std::cout << "atomic indices lock-free: " << queue.indices_are_lock_free()
            << '\n';
}
