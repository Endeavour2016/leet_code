/**
 * @file 15_latency_diagnosis.cpp
 * @brief 演示请求的排队耗时、执行耗时和队列满拒绝统计。
 * @details 用单调时钟记录阶段耗时，并通过有界队列构造可重复的拒绝场景。
 * @note 输出仅演示测量方法，不用于比较吞吐量或推断线上 P99。
 * @see notes/engineering/15_latency_diagnosis.md
 * @par 运行方式
 * 在仓库根目录执行：make run eng_latency_diagnosis
 */

#include <chrono>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

#include "../common/blocking_queue.h"
#include "../common/check.h"

typedef std::chrono::steady_clock Clock;
// arrival 在尝试入队前记录，因此队列耗时包含入队操作的开销。
struct Request {
  int id;
  Clock::time_point arrival;
};

// 所有耗时单位均为微秒，使用不会倒退的 steady_clock。
struct Timing {
  int id;
  double queue_us;
  double execute_us;
  double total_us;
};

double micros(Clock::duration duration) {
  return std::chrono::duration<double, std::micro>(duration).count();
}

// 以下为验证代码；复习时先阅读上方实现，main 用于检查正常和边界情况。
int main() {
  BlockingQueue<Request> queue(2);
  std::promise<void> start;
  auto gate = start.get_future();
  std::vector<Timing> timings;  // 只有工作线程写入，主线程在 join 后读取。
  std::thread worker([&] {
    gate.wait();
    Request request;
    while (queue.pop(request)) {
      const auto begin = Clock::now();
      // 用少量计算模拟检索步骤；测量结果不作为性能基准。
      volatile unsigned sum = 0;
      for (unsigned i = 0; i < 1000; ++i) {
        sum += i;
      }
      (void)sum;
      const auto end = Clock::now();
      timings.push_back({request.id,
                         micros(begin - request.arrival),
                         micros(end - begin),
                         micros(end - request.arrival)});
    }
  });
  // 工作线程尚未放行，前两次成功，第三次必定因容量不足而拒绝。
  const bool first = queue.try_push({1, Clock::now()});
  const bool second = queue.try_push({2, Clock::now()});
  const bool overloaded = !queue.try_push({3, Clock::now()});
  queue.close();
  start.set_value();
  worker.join();
  CHECK(first && second && overloaded);
  CHECK(timings.size() == 2);
  CHECK(timings[0].id == 1 && timings[1].id == 2);
  for (const auto& t : timings) {
    CHECK(t.queue_us >= 0 && t.execute_us >= 0);
    CHECK(t.total_us >= t.queue_us && t.total_us >= t.execute_us);
    std::cout << "id=" << t.id << " queue_us=" << t.queue_us
              << " execute_us=" << t.execute_us << " total_us=" << t.total_us
              << '\n';
  }

  std::cout
      << "accepted=2 rejected=1 (gated demonstration, not throughput data)\n";
}
