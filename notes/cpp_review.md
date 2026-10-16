# C++ 多线程、异步与语言机制复习

> 创建日期：2026-10-16  
> 修改日期：2026-10-16  
> AI辅助模型：GPT-6（未确认具体型号）

这份笔记用于 C++ 面试复习，重点是并发、异步任务、模板和对象布局。示例以 C++11 为主，较新特性会标明版本；完整工程实现见对应题解。

## 阅读路线

1. [并发基础与同步](#并发基础与同步先找共享数据再选工具)：线程生命周期、互斥锁、条件变量、原子操作。
2. [异步任务与并发设计](#异步任务与并发设计把执行结果和关闭分开)：future、async、线程池、取消与有界队列。
3. [模板编程](#模板编程先理解类型替换再看高级技巧)：推导、完美转发、特化和约束。
4. [对象内存模型](#对象内存模型区分语言保证与实现细节)：布局、继承、多态和生命周期。

时间有限时，先看前两章的例子，再用每章自检题练习口头回答。

## 标准版本速查

| 版本 | 与本专题最相关的增量 |
|---|---|
| C++11 | `thread`、`mutex`、`condition_variable`、`atomic`、`future`、`async`、lambda、移动语义、可变参数模板 |
| C++14 | 泛型 lambda、返回类型推导、`make_unique` |
| C++17 | `shared_mutex`、`scoped_lock`、`if constexpr`、类模板实参推导、`std::invoke` |
| C++20 | `jthread`、`stop_token`、`atomic::wait/notify`、信号量、锁存器/屏障、concepts、coroutine |
| C++23 | `std::expected`（显式表示结果或错误）；它本身不是异步机制 |

仓库工程题使用 **C++11，禁用异常**；较新特性只用于扩展复习。

## 和仓库现有代码的关系

- [多线程入门题](engineering/01_thread_basics.md)、[阻塞队列](engineering/02_blocking_queue.md)、[线程池](engineering/03_thread_pool.md)可用于把前两章的概念落实到 C++11 实现。
- [SPSC 队列](engineering/04_spsc_queue.md)和[索引快照](engineering/05_index_snapshot.md)适合在掌握原子操作的同步关系后阅读。
- 这里讨论语言与标准库的通用规则；工程题保留具体面试题目、测试和完整源码。

---

## 并发基础与同步：先找共享数据，再选工具

### 目标与直观例子

线程 A 写结果，线程 B 读结果，要保证三件事：**对象还活着、读写有同步、B 知道何时能读**。不能靠“通常 A 先运行”保证正确。

两个线程访问同一内存位置，至少一个在写，若不是都使用原子操作，也没有同步来安排先后，就可能产生**数据竞争**，程序行为未定义。普通共享变量通常用同一把互斥锁保护。`volatile` 不能解决线程同步。

### 1. 线程与对象生命周期（C++11）

`std::thread` 创建后启动线程。`join()` 等它结束，并保证能看到它写入的结果。线程引用的对象必须活到线程退出；仍可 `join()` 的线程对象若直接销毁，会触发 `std::terminate`，终止程序。线程不能 `join()` 自己。

```cpp
#include <thread>

int main() {
  int result = 0;
  std::thread worker([&result] {  // lambda：匿名函数；[&result] 按引用使用 result。
    result = 42;  // 只有 worker 写；主线程在 join 后才读取。
  });
  worker.join();  // 线程完成后再访问 result，并保证引用仍有效。
  return result == 42 ? 0 : 1;
}
```

主线程必须在 `join()` 后读取 `result`；提前轮询普通变量也会产生数据竞争。`detach()` 让线程独立运行，但不会延长局部变量的生命期。

### 2. 互斥锁与条件变量（C++11）

**锁保护数据，条件变量负责等待。** 检查和修改条件时用同一把锁；等待时用带谓词的 `wait`，即传入一个返回 bool 的函数，判断能否继续。

```cpp
#include <condition_variable>
#include <mutex>
#include <thread>

int main() {
  std::mutex mutex;
  std::condition_variable ready_condition;
  bool ready = false;
  int value = 0;
  int observed = 0;

  std::thread consumer([&] {  // [&]：按引用使用外部局部变量。
    std::unique_lock<std::mutex> lock(mutex);  // 管理锁，允许中途解锁再加锁。
    ready_condition.wait(lock, [&] {  // 不满足 ready 就继续等待。
      return ready;
    });
    observed = value;  // wait 返回时重新持锁，能安全读取 value。
  });

  {
    std::lock_guard<std::mutex> lock(mutex);  // 构造时加锁，离开作用域自动解锁。
    value = 42;
    ready = true;
  }
  ready_condition.notify_one();
  consumer.join();
  return observed == 42 ? 0 : 1;
}
```

`wait(lock, predicate)` 可以记成三步：

1. 持锁检查条件，满足就返回。
2. 不满足就原子地释放锁并等待，避免在解锁与等待之间漏掉通知。
3. 醒来后重新加锁、检查条件，不满足就继续等。

**通知不等于条件成立。** 线程可能没有收到通知也醒来，这叫“虚假唤醒”；也可能醒来时数据已被别的线程取走，所以必须重新检查。

生产者持锁时也能通知，但消费者要等锁释放后才能检查 `ready`。反过来，即使通知早于等待，只要 `ready` 已为 true，消费者就会直接继续。条件变量不保存通知，真正决定能否继续的是共享状态。

#### 等待队列任务

消费者应等待“有任务**或**已关闭”。只等有任务，关闭空队列时就可能一直睡眠。下面的 `tasks` 和 `closed` 都由 `mutex` 保护：

```cpp
std::unique_lock<std::mutex> lock(mutex);
condition.wait(lock, [&] {
  return !tasks.empty() || closed;
});
if (tasks.empty()) {
  // 谓词已成立，因此这里表示队列已关闭且没有剩余任务。
  return;
}
// std::move：允许移动队首任务的资源；具体含义见后文“值类别、移动和完美转发”。
Task task = std::move(tasks.front());
tasks.pop();
lock.unlock();
task();  // 执行任务时不持有保护队列的锁。
```

#### 通知谁、何时通知

| 操作 | 典型场景 | 注意点 |
|---|---|---|
| `notify_one()` | 加入一个任务，让一个等待任务的工作线程醒来 | 不保证唤醒哪一个线程，也不保证公平性 |
| `notify_all()` | 关闭队列、状态变化可能让所有等待者继续 | 每个线程醒来后仍要重新持锁检查条件 |

先持锁修改状态，再通知；通知可以放在解锁前或后。解锁后通知通常能减少等待锁的机会。若生产者等“队列未满”、消费者等“队列非空”，用两个条件变量更清楚。

#### 超时等待与关闭

`wait_for` 等一段时长，`wait_until` 等到指定时间。带谓词的版本返回 false，表示最后检查时条件仍未满足。若手写循环等待，要固定一个截止时间，避免每次醒来都重新计时；单次带谓词的 `wait_for` 已处理这个问题。

```cpp
// 片段：mutex、condition、ready、closed 由调用方持有；
// ready 和 closed 的所有访问都受 mutex 保护。
// steady_clock：单调时钟，不受系统校时影响；seconds(1) 表示 1 秒。
const std::chrono::steady_clock::time_point deadline =
    std::chrono::steady_clock::now() + std::chrono::seconds(1);
std::unique_lock<std::mutex> lock(mutex);
const bool woke_for_state = condition.wait_until(lock, deadline, [&] {
  return ready || closed;
});
if (!woke_for_state) {
  // 到截止时间仍未就绪、也未关闭：由调用方处理超时。
} else if (closed && !ready) {
  // 已关闭且没有结果：由调用方处理关闭。
} else {
  // ready 为 true，可在持锁状态下读取受保护的数据。
}
```

片段需要 `<chrono>`、`<condition_variable>` 和 `<mutex>`。这里约定已有结果优先于关闭。关闭方持锁设置 `closed = true`，再 `notify_all()`；等所有使用者退出后，才能销毁锁、条件变量和共享数据。`condition_variable` 使用 `unique_lock<mutex>`；其他锁类型可考虑 `condition_variable_any`。

死锁的典型例子：A 持锁 X 等 Y，B 持锁 Y 等 X。统一加锁顺序，或用 C++11 的 `std::lock` 协调多把锁。也不要持有工作线程退出所需的锁再 `join()`。

### 3. 原子操作与内存顺序（C++11）

**原子操作不会被其他线程观察成“做了一半”。** `std::atomic<T>` 提供原子的读、写和读改写，但不会自动保护旁边的普通变量。**内存顺序**决定这些普通数据何时能被另一线程安全读取。默认的 `std::memory_order_seq_cst` 规则最强；不确定时先用默认值。

| 内存顺序（省略 `std::memory_order_` 前缀） | 简单理解 | 常见用途 |
|---|---|---|
| `relaxed` | 只保证原子变量自身的操作，不负责同步其他数据 | 独立计数器 |
| `release` | 写方发布标志前，先把数据准备好 | 写入“已就绪”标志 |
| `acquire` | 读到对应的 release 发布后，能安全读取它之前写好的数据 | 检查“已就绪”标志 |
| `seq_cst` | 具备相应的 acquire/release 效果，且所有 seq_cst 操作有统一先后顺序 | 默认选择 |

下面只发布一次结果：生产者先写 `payload`，再发布 `ready`；消费者读到这次发布的 true 后，才读 `payload`。这建立了 **happens-before（先行发生）关系**，保证前面的写对后面的读可见。

```cpp
#include <atomic>
#include <thread>

int main() {
  int payload = 0;
  std::atomic<bool> ready(false);
  int observed = 0;

  std::thread producer([&] {
    payload = 42;
    // store 是原子写；release 发布之前写好的 payload。
    ready.store(true, std::memory_order_release);
  });
  std::thread consumer([&] {
    // load 是原子读；acquire 读到上面的 true 后，保证能看到 payload = 42。
    while (!ready.load(std::memory_order_acquire)) {
      // yield：建议调度器让其他线程先运行，也可能立即返回。
      // 它不保证切换线程，不负责同步数据；这里仍是循环轮询。
      std::this_thread::yield();
    }
    observed = payload;
  });

  producer.join();
  consumer.join();
  return observed == 42 ? 0 : 1;
}
```

若把上例的 release/acquire 都改成 relaxed，`ready` 自身仍安全，`payload` 的读写却失去了同步保证。`yield()` 不能补上这个保证；等待较久时应使用条件变量。

独立计数可用 `counter.fetch_add(1, std::memory_order_relaxed)`：原子地加 1，并返回旧值。但不能用这个计数推断其他数据已准备好。`is_lock_free()` 只查询该原子对象是否通过无锁方式实现，不能证明整个算法无锁（即某个线程停顿时，其他线程仍能推进操作）。

### 4. 决策与自检

| 需求 | 首选思路 |
|---|---|
| 一次启动、一次结束 | `thread` + 明确的 `join` 责任 |
| 多个字段要一起保持一致 | 一把 `mutex` 保护整个状态 |
| 等待队列非空或关闭 | `condition_variable` + 持锁检查谓词 |
| 一个独立计数器 | `atomic` 的读改写，按用途选内存顺序 |
| 发布普通对象给另一线程 | 互斥锁，或经过论证的 release/acquire 配对 |

自检：为什么 `if (!ready) wait(lock)` 不能替代谓词版本？关闭空队列时为什么通常要 `notify_all()`？超时等待为什么不能当作任务取消？为什么 `volatile int count` 不能安全完成两个线程的 `++count`？为什么 `detach()` 不能解决 lambda 引用局部变量的问题？

延伸阅读：[仓库线程入门题](engineering/01_thread_basics.md)、[阻塞队列](engineering/02_blocking_queue.md)、[WG21 条件变量章节](https://eel.is/c++draft/thread.condition)、[原子操作的内存顺序章节](https://eel.is/c++draft/atomics.order)。

---

## 异步任务与并发设计：把执行、结果和关闭分开

### 目标与直观例子

**同步调用要等任务完成才返回；异步调用允许先做别的事，再取结果。** `future` 用于获取结果，本身不代表一个线程。

要让两个任务有机会并发，先启动 A、B，再取结果。启动 A 后立即等待，等完才启动 B，就成了串行。

### 1. `promise`、`future` 与 `packaged_task`（C++11）

- `promise<T>`：生产者写入一次结果。
- `future<T>`：消费者等待并取出一次结果，调用 `get()` 后失效。
- `shared_future<T>`：允许多次读取同一结果，由有效 future 的 `share()` 获得；不自动保护结果的后续修改。
- `packaged_task`：包装函数，调用它时执行函数，并把返回值交给 future。

本仓库用结果结构体表示业务成功或失败。示例不恢复内存分配、线程创建等资源失败；调用 `get()` 前必须保证 future 有效。

```cpp
#include <future>
#include <thread>

struct TaskResult {
  bool ok_;
  int value_;
};

int main() {
  std::promise<TaskResult> promise;
  std::future<TaskResult> future = promise.get_future();  // 取得接收结果的一端。
  std::thread worker([&promise] {
    TaskResult result = {true, 42};
    promise.set_value(result);  // 每个 promise 只写入一次。
  });

  TaskResult result = future.get();  // get() 返回值，future 随后不再有效。
  worker.join();  // future 就绪不替代线程生命周期管理。
  return result.ok_ && result.value_ == 42 ? 0 : 1;
}
```

`future.get()` 返回什么，取决于 future 的类型：

| 类型 | `get()` 返回值 | 注意点 |
|---|---|---|
| `future<T>` | `T`，按值返回 | 只能取一次；按值返回不代表一定发生复制，也可能移动或省略复制 |
| `future<T&>` | `T&`，引用外部对象 | 只能取一次；外部对象须仍存活，访问须同步 |
| `shared_future<T>` | `const T&`，引用共享结果 | 可多次读取；须保证共享状态仍存活 |

上例直接写 `TaskResult result = future.get();`。`TaskResult&` 不能绑定这个临时结果；`const TaskResult&` 可以，并延长临时结果的生命期到引用的作用域结束，但这里按值接收更简单。

**取到结果不等于线程已经结束**，线程可能还在清理资源，所以仍需 `join()`。promise 销毁前若没有设置结果，future 会以异常报告 broken promise（结果提供者已退出）；禁用异常时，接口必须保证正常路径都设置结果。

### 2. `std::async` 的启动策略（C++11）

`std::async` 启动一个任务并返回 future。`std::launch::async` 在新线程执行；`std::launch::deferred` 等到调用 `get()` 或 `wait()` 时，由等待的线程执行。默认策略由实现选择。异步任务的 future 销毁时可能等待任务结束，所以应保存 future，避免丢弃临时对象导致串行。

```cpp
#include <future>

int main() {
  // []：lambda 不捕获外部局部变量。
  std::future<int> first = std::async(std::launch::async, [] {
    return 20;
  });
  std::future<int> second = std::async(std::launch::async, [] {
    return 22;
  });

  // 两个任务已启动；再分别获取结果。
  return first.get() + second.get() == 42 ? 0 : 1;
}
```

任务不保证同时占用不同 CPU 核。`future.wait_for()` 等待结果，超时不会取消任务；对于延迟执行的任务，它会返回 `future_status::deferred`，表示任务尚待执行。

### 3. 线程池与有界队列（C++11 可实现）

线程池用固定数量的线程反复从队列取任务，避免为每个请求创建线程：

```text
调用者 ──提交成功──> 待执行队列 ──工作线程取出──> 执行 ──> future 就绪
       └─提交失败──> 调用者处理拒绝，不生成“成功结果”
```

最小正确框架：

1. 提交者持锁检查是否关闭，成功时入队；返回值仅表示是否接受。
2. 工作线程持锁等待“有任务或已关闭”，取任务后释放锁，再执行任务。
3. 关闭时持锁停止接收新任务，通知所有工作线程；工作线程执行完已接受任务后退出，管理线程再 `join()`。

**有界队列限制待执行任务数量**，防止提交太快撑满内存。队列满时可等待空位或立即拒绝；若允许丢任务，要约定丢弃规则。需要区分“满了”和“关闭了”时用错误码，失败时保留输出参数。

两个常见错误：持锁执行慢任务，会阻塞其他线程取任务；只有一个工作线程时，父任务提交子任务后等待它，会死锁，因为没有空闲线程执行子任务。

完整实现与运行方法见[线程池题解](engineering/03_thread_pool.md)和[源码](../engineering/concurrency/03_thread_pool.cpp)。

### 4. 取消、超时与后续标准

| 能力 | 版本 | 复习重点 |
|---|---|---|
| `shared_mutex` | C++17 | 多个读者可并行持共享锁，写者独占；读多写少也未必总比普通互斥锁快 |
| `scoped_lock` | C++17 | 构造时协调多把锁，析构时自动解锁；这种资源管理方式叫 RAII |
| `jthread` | C++20 | 对象销毁时若仍可 join，会请求停止并等待线程结束；引用对象仍须保持存活到退出 |
| `stop_token` | C++20 | 协作式停止请求；任务必须主动检查，不能强制终止任意阻塞操作 |
| `atomic::wait/notify` | C++20 | 等待原子值改变；仍要循环确认业务条件 |
| `counting_semaphore` / `latch` / `barrier` | C++20 | 分别用于许可计数、一次性会合、可重复阶段会合 |
| coroutine | C++20 | 语言层面的挂起/恢复机制；调度器、I/O 完成与结果类型仍需库或应用提供 |

`jthread` 的最小用法：

```cpp
// C++20：需要 <thread>。工作线程自愿检查停止请求。
std::jthread worker([](std::stop_token token) {
  while (!token.stop_requested()) {  // 查询是否有人请求停止。
    DoOneBoundedStep();  // 应能及时返回；此函数由调用方实现。
  }
});
```

`DoOneBoundedStep()` 由调用方实现，必须能及时返回，否则销毁 `jthread` 时也可能一直等。**超时是等不下去了，取消是请求停下来，关闭是不再接新任务**；都不代表正在运行的任务已经退出。

### 5. 自检

- `submit()` 成功而任务返回 `ok_ == false`，分别发生了什么？
- `future.wait_for()` 超时后能否直接销毁它引用的数据？为什么？
- 只有一个工作线程的池中，父任务提交子任务后马上等子任务，会怎样？
- 为什么 `jthread` 自动 join 也不能拯救一个永远不检查停止请求的阻塞任务？

延伸阅读：[WG21 线程与 future 工作草案](https://eel.is/c++draft/thread)、[WG21 P0660R10：停止令牌与 jthread](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0660r10.pdf)。

---

## 模板编程：先理解类型替换，再看高级技巧

### 目标与直观例子

**模板让同一套逻辑适用于不同类型。** 例如 `Max(3, 5)` 使用整数版本，`Max(2.0, 4.0)` 使用浮点版本；编译器按具体类型生成代码，这叫实例化。读模板时先看 `T` 是什么，再看该类型是否支持函数里的操作。

### 1. 函数模板、类模板与推导（C++11）

```cpp
#include <string>

template <typename T>  // T 是类型参数，调用时可由实参推导。
T Max(const T& left, const T& right) {
  // 前提：T 支持 operator<，且返回值可以复制或移动。
  return left < right ? right : left;
}

template <typename T>
class Box {
 public:
  // explicit：禁止隐式转换成 Box；冒号后是成员初始化列表。
  explicit Box(const T& value) : value_(value) {
  }
  const T& value() const {  // 返回只读引用；末尾 const 表示不修改普通成员。
    return value_;
  }

 private:
  T value_;
};

int main() {
  Box<std::string> box("cpp");  // C++11 要显式写类模板实参。
  return Max(3, 5) == 5 && box.value() == "cpp" ? 0 : 1;
}
```

`Max(1, 2.0)` 会失败：两个参数推导出的 `T` 不一致。可写成 `Max<double>(1, 2.0)`。类模板在 C++11 中须写明类型，如 `Box<std::string>`；C++17 才支持类模板实参推导。

模板定义通常放在头文件，方便编译器生成具体类型的代码。若只支持固定几种类型，也可在实现文件显式实例化，即提前生成这些类型的版本。

### 2. 值类别、移动和完美转发（C++11）

#### 左值、右值与右值引用

先看“表达式”是什么意思：`Category(name)` 中，传进去的 `name` 就是一个表达式；`Category(std::move(name))` 中，表达式是 `std::move(name)`。这里判断的是**传参时这段写法属于左值还是右值**，不是给变量永久贴标签。

- **左值**：表示一个有身份、可继续访问的对象。最常见的是变量名，如 `name`；左值不一定能赋值，例如 const 变量。
- **右值**：常见的是临时结果，或被明确标记为可以移动资源的对象。
- **右值引用 `T&&`**：一种引用类型，通常用来接收右值，支持移动资源；`const T&` 也能接收右值，但通常不能用来转移资源。

假设已声明 `std::string name = "cpp";` 和 `std::string&& alias = std::string("temporary");`，下面表格对应紧接着的两个 `Category` 重载（同名、参数不同的函数）：

| 实际调用写法 | 传入的部分 | 左值还是右值 | 选中哪个重载 |
|---|---|---|---|
| `Category(name)` | 变量名 `name` | 左值 | `const std::string&`，返回 1 |
| `Category(std::string("tmp"))` | 临时字符串 | 右值，细分叫“纯右值” | `std::string&&`，返回 2 |
| `Category(std::move(name))` | 把 `name` 标记为允许移动 | 右值，细分叫“将亡值” | `std::string&&`，返回 2 |
| `Category(alias)` | 变量名 `alias` | 左值，即使声明用了 `&&` | `const std::string&`，返回 1 |

**`std::move` 只做转换，不会自己搬走资源。** 下例 `Category` 只返回数字，所以字符串没被移动；后面的 `unique_ptr` 移动初始化才真正转移所有权。

```cpp
#include <memory>
#include <string>
#include <utility>

int Category(const std::string&) {  // 参数不使用，可以省略参数名。
  return 1;
}
int Category(std::string&&) {  // 接收右值；这里不移动字符串，只返回分类编号。
  return 2;
}

int main() {
  std::string name = "cpp";
  // && 声明右值引用；绑定的临时字符串活到 alias 的作用域结束。
  std::string&& alias = std::string("temporary");
  if (Category(name) != 1 || Category(std::move(name)) != 2 ||
      Category(alias) != 1) {
    return 1;
  }

  // unique_ptr 独占对象，离开作用域自动释放；不能复制，可以移动。
  std::unique_ptr<int> source(new int(42));
  std::unique_ptr<int> destination = std::move(source);
  // unique_ptr 移动后，所有权属于 destination，source 为空。
  return source == nullptr && *destination == 42 ? 0 : 1;
}
```

移动后能做什么，要看类型约定：标准库字符串仍有效，但不保证为空；`unique_ptr` 明确保证源指针为空。对 const 对象调用 `std::move` 通常仍会复制，因为常见移动构造函数不接受 const 右值引用。按值返回局部变量时直接写 `return value;`，通常无需加 `std::move`。

#### 完美转发保留调用者的值类别

**完美转发就是：调用者传左值，就按左值转交；传右值，就按右值转交。**

普通参数 `std::string&&` 只能接右值。模板参数 `T&&` 在 `T` 由实参推导时叫**转发引用**，两种值都能接：左值使 `T` 推导为 `std::string&`，右值使 `T` 推导为 `std::string`。前一种情况下，组合出来的 `T& &&` 会合并为 `T&`，这叫引用折叠。

进入函数后，参数名 `value` 本身是左值；用 `std::forward<T>(value)` 才能恢复调用者传入时的值类别。

```cpp
#include <string>
#include <utility>

int Category(const std::string&) {
  return 1;
}
int Category(std::string&&) {
  return 2;
}

template <typename T>
int ForwardCategory(T&& value) {
  // forward<T>：传入时是左值就保持左值，是右值就恢复为右值。
  return Category(std::forward<T>(value));
}

int main() {
  std::string name = "cpp";
  const int from_lvalue = ForwardCategory(name);
  const int from_rvalue = ForwardCategory(std::string("temporary"));
  return from_lvalue == 1 && from_rvalue == 2 ? 0 : 1;
}
```

若直接调用 `Category(value)`，两次都会选左值重载。记住：`forward` 用于原样转交参数，`move` 用于允许转移资源。只读参数通常写 `const T&` 就够了。

一个常见用途是把构造参数传给目标类型：

```cpp
#include <memory>
#include <utility>

// typename... Args：一组类型参数；Args&&... args：对应的一组转发引用参数。
template <typename T, typename... Args>
std::unique_ptr<T> MakeObject(Args&&... args) {
  // 末尾的 ... 展开参数包：对每个参数调用 forward，再传给 T 的构造函数。
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
```

C++14 起可直接用 `std::make_unique<T>(...)` 创建独占指针。管理资源时，优先使用智能指针和容器；若需手写资源管理，要一起检查析构函数、复制构造、复制赋值、移动构造、移动赋值，这叫 Rule of Five（五法则）。C++14 还支持 `[](auto x)`，即参数类型可自动推导的泛型 lambda。

### 3. 特化、重载与 SFINAE

**特化就是为指定类型提供专门实现**：全特化指定全部模板参数，偏特化只限定部分参数或某类形式，如指针。类模板两者都支持；函数模板不能偏特化，通常用重载处理差异。

**SFINAE（替换失败不算错误）**：推导类型后，如果函数声明中的某些类型或表达式不成立，就不考虑这个模板候选。下面让 `IsEven` 只接收整数类型：

```cpp
#include <type_traits>

// is_integral<T>::value 判断 T 是否为整数类型。
// enable_if<条件, bool>::type：条件为真时得到 bool，否则没有 type。
// 开头的 typename 告诉编译器：后面的依赖名称是类型。
template <typename T>
typename std::enable_if<std::is_integral<T>::value, bool>::type
IsEven(T value) {
  return value % 2 == 0;
}

int main() {
  return IsEven(42) ? 0 : 1;
}
```

`IsEven(42)` 可以编译，`IsEven(3.14)` 找不到可用函数，会编译失败，不是运行时返回 false。相关工具：

- `static_assert`：编译期检查条件，失败时给出错误信息。
- `if constexpr`（C++17）：按编译期条件选择分支，实例化时丢弃未选分支；仍须满足基本语法等规则。
- concepts / `requires`（C++20）：直接声明模板参数需要满足的条件，比复杂的 `enable_if` 更易读。

### 4. 复习与取舍

| 机制 | 先想的问题 |
|---|---|
| 模板实参推导 | 每个参数推导出什么类型？`const` 与引用如何变化？ |
| 重载/特化 | 是“不同调用接口”，还是“同一个类模板的类型分支”？ |
| 转发引用 | `T` 是否在这个位置被推导？是否真的需要保留值类别？ |
| SFINAE/concepts | 约束能否用普通函数、重载或清晰的 `static_assert` 表达？ |
| 编译时间/代码体积 | 实例化了多少种类型？是否值得用非模板核心实现复用？ |

自检：`std::move` 为什么不等于移动构造已发生？一个声明为 `T&&` 的变量为什么作为表达式仍是左值？为什么 `template <typename T> void F(T&&)` 能接左值，而 `void G(std::string&&)` 不能？`enable_if` 失败是运行时失败还是编译期候选剔除？

延伸阅读：[WG21 工作草案模板章节](https://eel.is/c++draft/templates)、[模板实参推导章节](https://eel.is/c++draft/temp.deduct)、[约束与 concepts 章节](https://eel.is/c++draft/temp.constr)。

---

## 对象内存模型：区分语言保证与实现细节

### 目标与直观例子

**对象大小不仅是成员大小之和，还可能有对齐填充和实现所需的数据。** 面试时说明原因即可，具体字节数要看平台和编译器。

例如 `char` 占 1 字节、`int` 占 4 字节且要求 4 字节对齐时，两者之间可能补 3 字节，整个对象占 8 字节。这里的 8 是一种常见结果，不是所有平台的保证。

### 1. 对象、子对象、对齐与填充

`sizeof(T)` 是对象总大小，包含填充；`alignof(T)` 是对齐要求，即地址需要满足的字节倍数。普通成员和基类部分存放在对象里；静态成员和成员函数代码不会每个对象各存一份。对象末尾也可能补齐，保证数组中的下一个元素仍满足对齐要求。

```cpp
#include <cstddef>
#include <iostream>

struct Record {
  char tag_;
  int id_;
};

int main() {
  std::cout << "size=" << sizeof(Record)
            << " align=" << alignof(Record)
            // offsetof：成员相对对象起始地址的字节偏移。
            << " id_offset=" << offsetof(Record, id_) << '\n';
  // 观察当前平台布局；不把输出常量当成可移植保证。
  return 0;
}
```

这里的 `Record` 是标准布局类型，可安全用 `offsetof`；不要把它随意用在有虚函数等复杂布局的类上。调整成员顺序可能减少填充，但会影响 ABI（二进制接口，如编译后的模块如何访问对象）。

空类的大小也不能为 0，以便区分不同完整对象。空基类有时不额外占空间，叫空基类优化。C++20 的 `[[no_unique_address]]` 属性也允许成员与其他部分共用地址，常用于空类型成员。

### 2. 继承、虚函数与多态

派生类对象里包含基类部分。多重继承时，不同基类部分可能在不同地址，转成基类指针时编译器会调整地址。虚继承让多条继承路径共享同一份虚基类部分；不要自己计算偏移来转换指针。

**虚函数按对象实际类型选择实现。** 常见编译器用虚函数表和指向它的隐藏指针（常称 vptr）实现，但标准没有规定具体布局。通过基类指针删除派生对象时，基类应有虚析构函数。

```cpp
#include <memory>

class Shape {
 public:
  virtual ~Shape() = default;  // 虚析构；= default 让编译器生成实现。
  virtual int Area() const = 0;  // = 0 表示纯虚函数，Shape 不能直接实例化。
};

class Square : public Shape {
 public:
  explicit Square(int side) : side_(side) {
  }
  int Area() const override {  // override：让编译器检查确实覆盖了基类虚函数。
    return side_ * side_;
  }

 private:
  int side_;
};

int main() {
  std::unique_ptr<Shape> shape(new Square(3));
  return shape->Area() == 9 ? 0 : 1;
}
```

多态对象用指针或引用传递。若基类可以实例化，把派生对象按值赋给基类，会丢掉派生类特有的部分，叫**对象切片**。上例的 `Shape` 是抽象类，不能这样按值创建对象。

### 3. 构造、析构和生命周期

**先构造基类，再按声明顺序初始化成员，最后执行构造函数体；析构反过来。** 成员初始化顺序不由初始化列表的书写顺序决定。

构造或析构期间，虚函数不会调用尚未构造或已销毁的派生部分。也不要在构造完成前把 `this`（当前对象指针）交给其他线程。

```cpp
class Counter {
 public:
  Counter() : total_(0), step_(1) {
  }
  void Add() {
    total_ += step_;
  }
  int total() const {
    return total_;
  }

 private:
  int total_;  // 实际先于 step_ 初始化。
  int step_;
};
```

若改写为 `Counter() : step_(1), total_(step_) {}`，仍会先初始化 `total_`，导致读取未初始化的 `step_`。另外，成员函数标记 const 只限制自身修改对象，不保证读取时没有其他线程在写。

### 4. 指针、别名与二进制表示

`reinterpret_cast<T*>` 可以转换指针类型，但不保证能按 `T` 读取那片内存；还要满足对象生命期、对齐和类型访问规则。观察原始字节可用 `unsigned char`。`memcpy` 只复制字节，不会正确复制资源所有权，不能用它克隆带智能指针、虚函数等复杂对象。

文件和网络协议应逐字段编码，写清字段宽度、字节序和版本。不要直接保存整个对象的内存：填充、地址和平台布局都可能不同。`std::is_standard_layout<T>` 检查是否为标准布局类型，`std::is_trivially_copyable<T>` 检查是否可平凡复制；两者都不保证跨机器布局一致。

### 5. 自检

- `sizeof` 为什么可能包含尾部填充？为什么打印出的 `vptr` 偏移不能写入可移植代码？
- 构造函数初始化列表的顺序与成员实际初始化顺序不一致时，会出什么问题？
- 基类析构函数不为虚函数时，通过基类指针删除派生对象有什么风险？
- “对象地址可访问”是否等于“对象已完成构造、可以被另一个线程安全读取”？

延伸阅读：[WG21 工作草案对象章节](https://eel.is/c++draft/intro.object)、[类成员与布局章节](https://eel.is/c++draft/class.mem)、[继承与虚函数章节](https://eel.is/c++draft/class.derived)。
