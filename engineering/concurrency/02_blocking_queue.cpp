/**
 * @file 02_blocking_queue.cpp
 * @brief 验证有界阻塞队列的关闭行为和多线程安全性。
 * @details 检查先进先出、队列满时的处理，以及多个生产者和消费者是否丢失或重复任务。
 * @note 队列实现位于 common/blocking_queue.h；用线程间通知协调测试，不依赖 sleep。
 * @see notes/engineering/02_blocking_queue.md
 * @par 运行方式
 * 在仓库根目录执行：make run eng_blocking_queue
 */

#include <future>
#include <thread>
#include <vector>

#include "../common/blocking_queue.h"
#include "../common/check.h"

// 以下为验证代码；复习时先阅读上方实现，main 用于检查正常和边界情况。
int main() {
  BlockingQueue<int> invalid(0);
  CHECK(!invalid.valid());
  CHECK(!invalid.push(1));
  CHECK(!invalid.try_push(1));
  int unchanged = 7;
  CHECK(!invalid.pop(unchanged));
  CHECK(unchanged == 7);
  BlockingQueue<int> fifo(2);
  CHECK(fifo.valid());
  CHECK(fifo.push(1));
  CHECK(fifo.push(2));
  CHECK(!fifo.try_push(3));
  fifo.close();
  fifo.close();
  CHECK(!fifo.push(3));
  int item = 0;
  CHECK(fifo.pop(item) && item == 1);
  CHECK(fifo.pop(item) && item == 2);
  CHECK(!fifo.pop(item));

  // 通知只表示即将调用队列操作，不表示线程已经进入等待状态。
  // 先关闭或先进入等待，这两种执行顺序都必须正确。
  BlockingQueue<int> full(1);
  CHECK(full.push(1));
  std::promise<void> entered;
  auto blocked = std::async(std::launch::async, [&] {
    entered.set_value();
    return full.push(2);
  });
  entered.get_future().wait();
  full.close();
  CHECK(!blocked.get());
  BlockingQueue<int> empty(1);
  std::promise<void> reading;
  auto reader = std::async(std::launch::async, [&] {
    reading.set_value();
    int value = 0;
    return empty.pop(value);
  });
  reading.get_future().wait();
  empty.close();
  CHECK(!reader.get());

  const int producers = 4;
  const int consumers = 3;
  const int each = 2000;
  BlockingQueue<int> queue(7);
  std::vector<std::vector<int>> received(consumers);
  std::vector<std::thread> writers;
  std::vector<std::thread> readers;
  for (int c = 0; c < consumers; ++c) {
    readers.emplace_back([&, c] {
      int value;
      while (queue.pop(value)) {
        received[c].push_back(value);
      }
    });
  }

  for (int p = 0; p < producers; ++p) {
    writers.emplace_back([&, p] {
      for (int i = 0; i < each; ++i) {
        queue.push(p * each + i);
      }
    });
  }

  for (auto& t : writers) {
    t.join();
  }

  queue.close();
  for (auto& t : readers) {
    t.join();
  }

  std::vector<int> seen(producers * each, 0);
  for (const auto& batch : received) {
    for (int value : batch) {
      CHECK(value >= 0 && value < producers * each);
      ++seen[value];
    }
  }

  for (int times : seen) {
    CHECK(times == 1);
  }
}
