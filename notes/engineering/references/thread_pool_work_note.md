# 工作笔记中的 C++11 线程池：原始代码副本

来源：`/Users/zhuliming/Documents/work_in_jd/projects/项目文档/20240726_工作笔记_typora/Tech/program/多线程编程.md`，`c++11线程池demo` 一节。

以下代码块原样复制，包含类实现和 main 示例，供与[仓库实现](../../../engineering/concurrency/03_thread_pool.cpp)对照。原版使用异常，并且没有检查线程数为 0 的情况；作为参考资料保存，不接入仓库禁用异常的工程目标。对比结论见[线程池题解](../03_thread_pool.md)。

原笔记正文称 std::async 和 std::future 在 C++17 引入，应为 C++11；此处仅复制实现代码。版本可查阅 [WG21 N3337 草案](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3337.pdf) 的 `[futures]` 章节。

```cpp
#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

class ThreadPool {
public:
    ThreadPool(size_t);
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
        -> std::future<typename std::result_of<F(Args...)>::type>;
    ~ThreadPool();

private:
    // 线程对象
    std::vector< std::thread > workers;
    // 任务队列
    std::queue< std::function<void()> > tasks;
    // 同步工具
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};


// 构造函数只启动一定数量的工作线程
ThreadPool::ThreadPool(size_t threads) : stop(false) {
  for(size_t i = 0; i < threads; ++i)
    workers.emplace_back(
        [this] {
            for(;;) {
              std::function<void()> task;
              {
                  std::unique_lock<std::mutex> lock(this->queue_mutex);
                  this->condition.wait(lock, [this]{ return this->stop || !this->tasks.empty(); });
                  if (this->stop && this->tasks.empty()) {
                      return;
                  }
                  task = std::move(this->tasks.front());
                  this->tasks.pop();
              }
              task();
            }
        }
    );
}

// 添加新的工作项到线程池中
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        // 不允许在停止的线程池中加入任务
        if (stop) {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }

        tasks.emplace([task](){ (*task)(); });
    }
    condition.notify_one();
    return res;
}

// 析构函数
ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for(std::thread &worker: workers)
        worker.join();
}

int main()
{
    ThreadPool pool(4);
    std::vector< std::future<int> > results;

    for(int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool.enqueue([i] {
                std::cout << "hello " << i << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << "world " << i << std::endl;
                return i*i;
            })
        );
    }

    for(auto && result: results)
        std::cout << result.get() << ' ';
    std::cout << std::endl;

    return 0;
}
```
