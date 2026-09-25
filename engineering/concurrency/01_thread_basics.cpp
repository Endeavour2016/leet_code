/**
 * @file 01_thread_basics.cpp
 * @brief 演示 C++11 线程生命周期、同步和结果传递。
 * @details 通过互斥锁和条件变量传递数据，通过原子变量计数，通过 future 获取结果及业务状态。
 * @note 示例中的引用对象必须在线程结束前保持有效。
 * @see notes/engineering/01_thread_basics.md
 * @par 运行方式
 * 在仓库根目录执行：make run eng_thread_basics
 */

#include <atomic>
#include <condition_variable>
#include <future>
#include <mutex>
#include <thread>
#include <utility>

#include "../common/check.h"

// 用析构函数等待线程结束，避免提前返回时遗漏 join。
class JoiningThread {
 public:
  explicit JoiningThread(std::thread t) : thread_(std::move(t)) {
  }

  ~JoiningThread() {
    if (thread_.joinable()) {
      thread_.join();
    }
  }

  JoiningThread(const JoiningThread&) = delete;
  JoiningThread& operator=(const JoiningThread&) = delete;

 private:
  std::thread thread_;
};

// 以下为验证代码；复习时先阅读上方实现，main 用于检查正常和边界情况。
int main() {
  std::mutex mutex;
  std::condition_variable cv;
  bool ready = false;
  int payload = 0;
  int observed = 0;
  {
    JoiningThread consumer(std::thread([&] {
      std::unique_lock<std::mutex> lock(mutex);
      cv.wait(lock, [&] {
        return ready;
      });
      observed = payload;
    }));
    {
      std::lock_guard<std::mutex> lock(mutex);
      payload = 42;
      ready = true;
    }
    cv.notify_one();
  }  // 先等待线程结束，再读取结果或销毁它引用的变量。
  CHECK(observed == 42);
  // 这里只统计次数，不用计数值通知其他数据就绪，因此 relaxed 足够。
  std::atomic<int> count(0);
  {
    JoiningThread a(std::thread([&] {
      for (int i = 0; i < 10000; ++i) {
        count.fetch_add(1, std::memory_order_relaxed);
      }
    }));
    JoiningThread b(std::thread([&] {
      for (int i = 0; i < 10000; ++i) {
        count.fetch_add(1, std::memory_order_relaxed);
      }
    }));
  }

  CHECK(count.load() == 20000);
  // 业务失败作为普通结果传递；promise 只设置一次，future 只获取一次。
  struct TaskResult {
    bool ok;
    int value;
  };
  std::promise<TaskResult> promise;
  std::future<TaskResult> answer = promise.get_future();
  {
    JoiningThread worker(std::thread([&] {
      promise.set_value(TaskResult{false, 0});
    }));
    const TaskResult result = answer.get();
    CHECK(!result.ok);
  }
  auto async_answer = std::async(std::launch::async, [] {
    return 7;
  });
  CHECK(async_answer.get() == 7);
  // deferred 不立即执行，直到 get() 才在调用线程执行函数。
  int calls = 0;
  auto deferred = std::async(std::launch::deferred, [&] {
    ++calls;
    return 9;
  });
  CHECK(calls == 0);
  CHECK(deferred.get() == 9);
  CHECK(calls == 1);
}
