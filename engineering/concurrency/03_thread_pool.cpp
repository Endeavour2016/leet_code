/**
 * @file 03_thread_pool.cpp
 * @brief 实现固定线程数的 C++11 线程池。
 * @details 工作线程持有互斥锁时取出任务，释放锁后执行；submit 返回接单状态，future 传递任务结果。
 * @note 关闭后拒绝新任务并执行完已接受任务；关闭和析构由外部管理线程完成。
 * @see notes/engineering/03_thread_pool.md
 * @see notes/engineering/references/thread_pool_work_note.md 原始笔记实现的对照副本。
 * @par 运行方式
 * 在仓库根目录执行：make run eng_thread_pool
 */

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#include "../common/check.h"

// 工作线程执行已接受的任务；关闭操作只能由外部管理线程调用。
class ThreadPool {
 public:
  // count 为 0 时创建无效且不接受任务的线程池，通过 valid() 查询。
  explicit ThreadPool(std::size_t count) : stopped_(count == 0) {
    for (std::size_t i = 0; i < count; ++i) {
      workers_.emplace_back([this] {
        work();
      });
    }
  }

  ~ThreadPool() {
    shutdown();
  }

  bool valid() const {
    return !workers_.empty();
  }

  // true 表示任务已入队，output 用于获取任务结果；拒绝任务时 output 保持不变。
  // 任务本身的业务失败应通过返回值或状态结构表达，不能抛出异常。
  //
  // 先用一次加法理解结果传递：
  //   std::future<int> result;
  //   if (pool.submit([] { return 2 + 3; }, result)) {
  //     const int value = result.get();  // 等待并取出 5，在这里使用 value。
  //   }
  // submit 负责接单，工作线程负责计算，future 负责领取这一次计算的结果。
  // 只接收无参数任务；需要参数时用 lambda 捕获，省去可变参数模板和 bind。
  // 按引用捕获的对象必须存活到任务结束，线程池不会延长其生命周期。
  //
  // future<R> 可以看成领取结果的凭据，关联到保存结果和就绪标志的共享状态。
  // 默认构造的 future 没有关联状态；valid() 为 true 只表示有关联，不表示完成。
  // wait() 等待就绪但不取走结果；get() 等待并取出结果，之后 future 失效。
  // wait/get 都要求 future 有效；同一个 future 不能 get 两次。
  // future<void> 不传回数据，get() 只确认任务完成，也只能调用一次。
  // 共享状态负责结果发布与等待的同步，无需另加锁读取这个返回值。
  // 它不会自动保护任务访问的其他业务数据，也不会因为丢弃 future 而取消任务。
  // 接受任务会替换 output，通常应传入空 future，避免失去原任务的结果凭据。
  // 接口参考：https://learn.microsoft.com/en-us/cpp/standard-library/future-class
  template <class F>
  bool submit(F f, std::future<typename std::result_of<F()>::type>& output) {
    // F 是可调用对象的类型，f 是具体对象。result_of 在编译时推导返回类型：
    // F() 在这里表示查询无参数调用的结果类型，不会执行 f，也不是构造 F。
    // ::type 取得推导出的类型；typename 表明这个依赖 F 的名字是类型；
    // typedef 给它起别名 R。比如返回 5 的 lambda 对应 R=int，空任务对应 void。
    // 因此同一份 submit 可以接收返回 int、bool、结果结构体或 void 的任务。
    typedef typename std::result_of<F()>::type R;
    {
      std::lock_guard<std::mutex> lock(mutex_);
      if (stopped_) {
        return false;
      }
      // packaged_task<R()> 包装一个无参数、返回 R 的函数；创建时不执行函数，
      // 调用它才执行函数，并把返回值写入与 future 关联的共享状态。
      // 它自己不创建线程；本例由已有工作线程调用它，R=void 时只标记完成。
      // 每个任务在本实现中只执行一次，对它也只调用一次 get_future()。
      // 接口参考：https://learn.microsoft.com/en-us/cpp/standard-library/packaged-task-class
      //
      // packaged_task 不能复制，而 C++11 std::function 要求保存的对象可复制。
      // make_shared 创建由 shared_ptr 管理的任务；[task] 按值捕获这个指针，
      // 复制 lambda 时只复制指针，不复制底层任务。submit 返回后任务仍存活。
      // 不能用 [&task]：工作线程执行时，submit 的局部变量可能已经销毁。
      // shared_ptr 只管理生命周期；任务只执行一次仍靠持有互斥锁时取出任务。
      //
      // auto 从初始化表达式推导变量类型。std::move 本身不搬运数据，
      // 它允许后续构造或赋值选择移动操作，这里把 f 交给 packaged_task 保存。
      auto task = std::make_shared<std::packaged_task<R()>>(std::move(f));
      // 只取得关联的 future，此时没有执行计算，也不等待任务完成。
      auto result = task->get_future();
      // 队列中的外层 lambda 返回 void，但原函数的 R 类型结果没有丢失：
      // 外层 lambda -> packaged_task 执行原函数 -> 共享状态保存 R -> future.get()。
      tasks_.push([task] {
        (*task)();
      });
      // future 不可复制，只能移动；赋值后 output 接管关联，result 变为无效。
      output = std::move(result);
    }
    ready_.notify_one();
    return true;
  }

  // 可与 submit 同时调用，但多个线程不能同时执行 shutdown。
  void shutdown() {
    {
      std::lock_guard<std::mutex> lock(mutex_);
      stopped_ = true;
    }
    ready_.notify_all();
    for (auto& worker : workers_) {
      if (worker.joinable()) {
        worker.join();
      }
    }
  }

 private:
  // 只在等待和取任务时持有互斥锁，执行任务前必须释放锁。
  void work() {
    for (;;) {
      std::function<void()> task;
      {
        std::unique_lock<std::mutex> lock(mutex_);
        ready_.wait(lock, [this] {
          return stopped_ || !tasks_.empty();
        });
        // wait 返回时条件必为真，因此队列为空就说明 stopped_ 已为 true。
        // 等价于判断 stopped_ && tasks_.empty()；停止后仍先执行剩余任务。
        if (tasks_.empty()) {
          return;
        }
        task = std::move(tasks_.front());
        tasks_.pop();
      }
      task();
    }
  }

  bool stopped_;
  std::mutex mutex_;
  std::condition_variable ready_;
  // 不同 lambda 的类型不同，function<void()> 把它们统一为无参数调用接口。
  // 这叫类型擦除：队列不必知道原函数类型，工作线程只需调用 task()。
  std::queue<std::function<void()>> tasks_;
  std::vector<std::thread> workers_;
};

// 以下为验证代码：区分提交失败与任务返回的业务失败。
int main() {
  ThreadPool invalid(0);
  CHECK(!invalid.valid());
  std::future<int> rejected;
  CHECK(!invalid.submit(
      [] {
        return 1;
      },
      rejected));
  CHECK(!rejected.valid());

  ThreadPool pool(3);
  CHECK(pool.valid());
  std::atomic<int> calls(0);
  std::vector<std::future<int>> results(1000);
  for (int i = 0; i < 1000; ++i) {
    CHECK(pool.submit(
        [&, i] {
          calls.fetch_add(1, std::memory_order_relaxed);
          return i * i;
        },
        results[i]));
  }
  struct TaskResult {
    bool ok;
    int value;
  };
  std::future<TaskResult> failure;
  CHECK(pool.submit(
      [] {
        return TaskResult{false, 0};
      },
      failure));
  std::future<void> nothing;
  CHECK(pool.submit(
      [] {
      },
      nothing));
  pool.shutdown();
  pool.shutdown();
  CHECK(calls.load() == 1000);

  // 拒绝任务不能覆盖调用者已有的有效 future。
  CHECK(!pool.submit(
      [] {
        return -1;
      },
      results[0]));
  for (int i = 0; i < 1000; ++i) {
    CHECK(results[i].valid());
    CHECK(results[i].get() == i * i);
  }
  CHECK(!failure.get().ok);
  nothing.get();
  std::future<void> new_task;
  CHECK(!pool.submit(
      [] {
      },
      new_task));
  CHECK(!new_task.valid());

  std::future<int> after_destruction;
  {
    ThreadPool scoped(1);
    CHECK(scoped.submit(
        [] {
          return 42;
        },
        after_destruction));
  }
  CHECK(after_destruction.get() == 42);

  // 多提交者与关闭操作竞争：返回 true 的任务都必须执行完。
  ThreadPool racing(2);
  // promise 由代码显式 set_value() 发布结果；packaged_task 则在执行函数后
  // 自动发布函数返回值。这里用 promise<void> 发送“已经到达某一步”的通知。
  // ready_a/b 通知主线程首次提交已成功，go 通知两个提交线程可以继续。
  std::promise<void> ready_a;
  std::promise<void> ready_b;
  std::promise<void> go;
  // share() 把独占的 future 转为可复制的 shared_future，供多个等待者使用。
  // gate.wait() 等待同一份状态就绪；不像 future.get()，不会消费结果。
  // 这些同步对象只用于协调测试，不属于线程池实现所必需的组件。
  std::shared_future<void> gate = go.get_future().share();
  std::vector<std::future<int>> accepted[2];
  auto submitter = [&](int id, std::promise<void>& ready) {
    std::future<int> first;
    CHECK(racing.submit(
        [id] {
          return id;
        },
        first));
    accepted[id].push_back(std::move(first));
    ready.set_value();
    gate.wait();
    for (int i = 0; i < 100; ++i) {
      std::future<int> result;
      if (!racing.submit(
              [id] {
                return id;
              },
              result)) {
        break;
      }
      accepted[id].push_back(std::move(result));
    }
  };
  std::thread a(submitter, 0, std::ref(ready_a));
  std::thread b(submitter, 1, std::ref(ready_b));
  ready_a.get_future().wait();
  ready_b.get_future().wait();
  go.set_value();
  racing.shutdown();
  a.join();
  b.join();
  for (int id = 0; id < 2; ++id) {
    for (auto& result : accepted[id]) {
      CHECK(result.get() == id);
    }
  }

  // 一个工作线程按队列顺序执行，多个工作线程不保证完成顺序。
  std::vector<int> order;
  {
    ThreadPool one(1);
    for (int i = 0; i < 20; ++i) {
      std::future<void> done;
      CHECK(one.submit(
          [&, i] {
            order.push_back(i);
          },
          done));
    }
  }
  CHECK(order.size() == 20);
  for (int i = 0; i < 20; ++i) {
    CHECK(order[i] == i);
  }
}
