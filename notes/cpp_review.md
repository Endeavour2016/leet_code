# C++ 多线程、异步与语言机制复习

> 创建日期：2026-10-16  
> 更新日期：2026-10-16  
> 修改日期：2026-10-16  
> AI辅助模型：GPT-6（未确认具体型号）

面向做过几年 C++ 开发、近两年较少写 C++ 的读者。主线是重新建立对**对象生命周期、线程间可见性、任务结果传递**的判断能力，再补上模板与对象内存布局。示例以 C++11 为基线；用到较新标准的地方会标明版本。代码片段遵循仓库的 Google 风格和两空格缩进。本文是复习提纲，不把示例当作完整生产组件。

## 阅读路线

1. [并发基础与同步](#并发基础与同步先找共享数据再选工具)：线程生命周期、互斥锁、条件变量、原子操作。
2. [异步任务与并发设计](#异步任务与并发设计把执行结果和关闭分开)：future、async、线程池、取消与有界队列。
3. [模板编程](#模板编程先理解类型替换再看高级技巧)：推导、完美转发、特化和约束。
4. [对象内存模型](#对象内存模型区分语言保证与实现细节)：布局、继承、多态和生命周期。

如果只有半天，先读前两章的最小例子和易错点，再回看后两章的表格与代码。能解释“为什么正确”和“何时失效”，比记住 API 名称更重要。

## 标准版本速查

| 版本 | 与本专题最相关的增量 |
|---|---|
| C++11 | `thread`、`mutex`、`condition_variable`、`atomic`、`future`、`async`、lambda、移动语义、可变参数模板 |
| C++14 | 泛型 lambda、返回类型推导、`make_unique` |
| C++17 | `shared_mutex`、`scoped_lock`、`if constexpr`、类模板实参推导、`std::invoke` |
| C++20 | `jthread`、`stop_token`、`atomic::wait/notify`、信号量、锁存器/屏障、concepts、coroutine |
| C++23 | `std::expected`（显式表示结果或错误）；它本身不是异步机制 |

仓库的工程题仍以 **C++11、禁用异常** 为目标；较新特性用于了解现代代码和面试追问。未来新标准可能继续增加库能力，本文不推断未核对的特性。

## 和仓库现有代码的关系

- [多线程入门题](engineering/01_thread_basics.md)、[阻塞队列](engineering/02_blocking_queue.md)、[线程池](engineering/03_thread_pool.md)可用于把前两章的概念落实到 C++11 实现。
- [SPSC 队列](engineering/04_spsc_queue.md)和[索引快照](engineering/05_index_snapshot.md)适合在掌握原子操作的同步关系后阅读。
- 这里讨论语言与标准库的通用规则；工程题保留具体面试题目、测试和完整源码。


---

## 并发基础与同步：先找共享数据，再选工具

### 目标与直观例子

假设线程 A 产生一个结果，线程 B 读取它。先问：结果对象在 B 读取时还活着吗？A 写入与 B 读取之间是否有同步关系？B 怎样知道结果已准备好？这三个问题分别对应**生命周期、数据竞争、等待条件**。仅仅“通常 A 先运行”不能构成正确性依据。

两个线程可能同时访问同一内存位置，至少一个在写，且访问之间没有合适的同步关系，就构成数据竞争；程序行为未定义。`volatile` 不能代替 `mutex` 或 `atomic`。普通变量只在所有访问都受同一把锁保护，或者线程间有其他合法的先后关系时，才能安全共享。

### 1. 线程与对象生命周期（C++11）

`std::thread` 构造后开始执行。可 `join()` 的线程对象被销毁时，程序会调用 `std::terminate`，所以应明确谁负责等待结束。`join()` 阻塞调用线程，直到目标线程完成；不能让线程继续访问已经销毁的引用，也不能由线程等待自己。启动线程前准备好它要访问的对象，销毁对象前完成 `join()`。

```cpp
#include <thread>

int main() {
  int result = 0;
  std::thread worker([&result] {
    result = 42;  // 只有 worker 写；主线程在 join 后才读取。
  });
  worker.join();  // 线程完成后再访问 result，并保证引用仍有效。
  return result == 42 ? 0 : 1;
}
```

这里的 `join()` 同时解决了等待结束和结果可见性。若主线程在 `join()` 前读取 `result`，即使循环等待到它“看起来已经变成 42”，仍然是无同步的访问。`detach()` 只分离线程对象的管理责任，不延长引用和局部变量的生命期。

### 2. 互斥锁与条件变量（C++11）

互斥锁保护共享状态；条件变量让线程在条件不满足时睡眠。等待者检查状态和生产者修改状态时，必须持有**同一把互斥锁**。通知只是提醒重新检查状态；虚假唤醒也可能发生，因此用带谓词的 `wait`。

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

  std::thread consumer([&] {
    std::unique_lock<std::mutex> lock(mutex);
    ready_condition.wait(lock, [&] {
      return ready;
    });
    observed = value;  // wait 返回时重新持锁，能安全读取 value。
  });

  {
    std::lock_guard<std::mutex> lock(mutex);
    value = 42;
    ready = true;
  }
  ready_condition.notify_one();
  consumer.join();
  return observed == 42 ? 0 : 1;
}
```

如果通知先发生，消费者获得锁后会发现 `ready == true`，不会睡眠；如果消费者先睡眠，生产者修改状态后通知它。`wait` 在睡眠时释放锁、醒来后重新获得锁。执行耗时任务前应释放只负责保护队列的锁，否则其他线程连取任务或提交任务都做不了。`lock_guard` 用于整个作用域持锁；需要 `wait` 或中途解锁时用 `unique_lock`。

#### 条件变量等待的到底是什么

条件变量本身不保存“已通知一次”的状态。真正的状态是受互斥锁保护的 `ready`、队列是否为空、是否关闭等。每次等待都按同一顺序处理：

1. 持锁检查条件；若已满足，直接继续。
2. 若不满足，`wait` 以不可分割的方式释放锁并进入等待；被通知或意外唤醒后，先重新获得锁。
3. 重新检查条件；不满足就继续等待。`wait(lock, predicate)` 等价于按此方式循环检查谓词。

例如队列暂时为空时，消费者等待的是“队列有数据**或**队列已关闭”，不是等待一次 `notify_one()`。只等待“有数据”会使关闭后的空队列消费者一直睡眠。下列片段假设 `tasks` 和 `closed` 都由 `mutex` 保护：

```cpp
std::unique_lock<std::mutex> lock(mutex);
condition.wait(lock, [&] {
  return !tasks.empty() || closed;
});
if (tasks.empty()) {
  // 谓词已成立，因此这里表示队列已关闭且没有剩余任务。
  return;
}
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

先持锁修改共享状态，再通知。通知可以在释放锁前或后调用；上面的完整例子在释放锁后通知，让醒来的线程更容易立即获得锁。通知本身不替代持锁修改状态，也不保证等待者在通知返回时已经处理任务。如果生产者和消费者等待的条件不同，用两个条件变量分别表示“队列非空”和“队列未满”通常更清晰；每一类等待者都检查自己的谓词。

#### 超时等待与关闭

`wait_for` 指定相对时长，`wait_until` 指定绝对截止时间。带谓词的重载会在醒来后重新检查条件；返回 `false` 表示在截止时间到达时条件仍不满足，而不是任务已被取消。需要对整个操作限制总时长时，先算一次 `steady_clock` 截止时间，再用 `wait_until`，避免每次虚假唤醒都重新给一段完整时长。

```cpp
// 片段：mutex、condition、ready、closed 由调用方持有；
// ready 和 closed 的所有访问都受 mutex 保护。
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

该片段需包含 `<chrono>`、`<condition_variable>` 和 `<mutex>`。如果关闭时还要处理已经就绪的数据，应像上例一样先判断 `ready` 再决定是否退出；具体优先级由接口约定。关闭方持锁设置 `closed = true`，解锁后 `notify_all()`，最后等待所有使用条件变量的线程退出，才能销毁条件变量、互斥锁和共享状态。C++11 的 `condition_variable` 配合 `unique_lock<mutex>`；若确实要配合其他锁类型，可使用 `condition_variable_any`。

两把以上的锁要注意死锁：线程 A 持锁 X 等 Y，线程 B 持锁 Y 等 X。统一加锁顺序；C++11 可用 `std::lock` 协调多把锁，C++17 可用 `std::scoped_lock`。不要持有工作线程退出所需的锁再 `join()`。

### 3. 原子操作与内存顺序（C++11）

`std::atomic<T>` 让其支持的读、写、读改写操作成为原子操作；它不会自动把其他普通对象也变安全。**内存顺序（memory ordering）**是原子操作的一项规则：它约束多个线程观察读写的顺序，以及某次原子读能否用于确认另一线程写入的普通数据已经可见。默认使用 `seq_cst`（顺序一致），便于首先写对程序；选择更弱的规则前，应说明需要哪条线程间同步关系。

| 内存顺序 | 适用意图 | 关键限制 |
|---|---|---|
| `relaxed` | 独立计数、统计；只要求该原子对象不丢更新 | 不发布其他普通数据 |
| `release` + `acquire` | 写方先准备数据，再发布标志；读方看到该发布后读取数据 | acquire 必须读到对应 release 写入的值或其释放序列中的值 |
| `seq_cst` | 在 acquire/release 基础上提供所有顺序一致操作的单一总顺序 | 易理解，但不是“所有对象都自动线程安全” |

下面的 `payload` 只由生产者写一次；消费者只有在 acquire 读到 `ready == true` 后才读它。release/acquire 建立跨线程的 *happens-before*（先行发生）关系。若把两个操作都改成 `relaxed`，对 `payload` 的普通读写就失去这条保证。

```cpp
#include <atomic>
#include <thread>

int main() {
  int payload = 0;
  std::atomic<bool> ready(false);
  int observed = 0;

  std::thread producer([&] {
    payload = 42;
    ready.store(true, std::memory_order_release);
  });
  std::thread consumer([&] {
    while (!ready.load(std::memory_order_acquire)) {
      std::this_thread::yield();  // 教学示例；实际等待宜用条件变量。
    }
    observed = payload;
  });

  producer.join();
  consumer.join();
  return observed == 42 ? 0 : 1;
}
```

单纯计数可用 `counter.fetch_add(1, std::memory_order_relaxed)`：计数不丢，但不能凭计数值推断另一份数据已经准备好。一个无锁结构还必须处理对象生命周期、覆盖顺序、多个生产者/消费者等问题；`atomic` 本身不等于“无锁”。`is_lock_free()` 可以查询某个原子对象在该实现上的情况，但不能据此证明整个算法无锁。

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

同步调用在当前线程完成任务并返回；异步任务允许调用者先做别的事，再取结果。复习时分清三件事：**任务在哪里执行、结果如何交付、等待和关闭由谁负责**。`future` 表示将来可读取的结果，不等于“每个 future 都有独立线程”。

例如收到两个独立请求，可以先发起两个任务，再等待结果。若先启动 A 并立即 `get()`，然后才启动 B，就没有给两者重叠执行的机会。

### 1. `promise`、`future` 与 `packaged_task`（C++11）

`promise<T>` 由生产者写入一次结果，`future<T>` 由消费者等候并获取一次结果；`get()` 会消耗普通 future 的共享状态。多个读者需要同一份结果时，可在有效的 future 上调用 `share()` 得到 `shared_future<T>`，但这并不自动保护结果对象后续的可变访问。`packaged_task` 包装可调用对象，执行后把返回值写入相应的 future。

仓库工程题禁用异常，因此示例用结果结构体区分**任务执行失败**与结果数值；`get()` 仍应满足 future 有效、只调用一次等前提。标准库资源失败（例如内存分配或线程创建）不在本教学接口可恢复范围内。

```cpp
#include <future>
#include <thread>

struct TaskResult {
  bool ok_;
  int value_;
};

int main() {
  std::promise<TaskResult> promise;
  std::future<TaskResult> future = promise.get_future();
  std::thread worker([&promise] {
    TaskResult result = {true, 42};
    promise.set_value(result);  // 每个 promise 只写入一次。
  });

  TaskResult result = future.get();  // get() 返回值，future 随后不再有效。
  worker.join();  // future 就绪不替代线程生命周期管理。
  return result.ok_ && result.value_ == 42 ? 0 : 1;
}
```

这里的类型是 `std::future<TaskResult>`，所以 `get()` 的返回类型是 `TaskResult`，即**按值返回**；返回对象可被移动或省略中间复制，不应把“按值返回”理解成“一定复制一次”。`TaskResult& result = future.get();` 不合法，因为非 `const` 左值引用不能绑定到这个临时结果。推荐写 `TaskResult result = future.get();`。`const TaskResult& result = future.get();` 可以绑定临时结果并将其生命期延长到该引用的作用域结束，但这里没有必要增加引用。

这与另外两种 future 不同：`std::future<T&>::get()` 返回 `T&`，引用的是外部对象，调用方必须保证对象仍存活且访问已同步；`std::shared_future<T>::get()` 返回 `const T&`，可多次读取，共享状态存活期间引用才有效。普通 `std::future<T>::get()` 只能调用一次，调用后该 future 不再有效。

`future.get()` 可以比 `join()` 更早拿到结果，但工作线程此后可能还有清理工作；仍要管理线程结束。若生产者销毁 promise 而未设置结果，标准库会使 future 就绪并以异常报告 broken promise；在禁用异常的工程题中，应通过接口约定确保生产者设置结果，不能用空值假装成功。

### 2. `std::async` 的启动策略（C++11）

`std::launch::async` 请求异步执行；`std::launch::deferred` 把调用延迟到 `get()` 或 `wait()`，并在执行这些等待操作的线程中运行。默认策略允许实现选择，不能假定必开新线程。显式 `async` 返回的 future 在某些销毁场景会等待任务完成，所以丢弃临时 future 可能让循环看起来串行。

```cpp
#include <future>

int main() {
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

这里的“异步”不保证任务一定同时占用两个 CPU 核，也不保证固定的线程池实现。`future.wait_for()` 只检查或等待结果的就绪状态；超时不代表任务已取消。对 deferred 任务，它可能返回 `future_status::deferred`。

### 3. 线程池与有界队列（C++11 可实现）

当请求数量多于适合创建的线程数量时，固定数量工作线程反复从队列取任务。提交、执行、关闭分别有自己的状态与责任：

```text
调用者 ──提交成功──> 待执行队列 ──工作线程取出──> 执行 ──> future 就绪
       └─提交失败──> 调用者处理拒绝，不生成“成功结果”
```

最小正确框架：

1. 提交者持锁检查是否关闭，成功时入队；返回值仅表示是否接受。
2. 工作线程持锁等待“有任务或已关闭”，取任务后释放锁，再执行任务。
3. 关闭时持锁停止接收新任务，通知所有工作线程；工作线程执行完已接受任务后退出，管理线程再 `join()`。

若队列没有容量限制，提交任务的速度长期大于执行速度时，内存会持续增长。**有界队列**规定最多存放多少个待执行任务；队列满时，可以让提交者等待空位，也可以立即返回“队列已满”。如果业务允许丢弃任务，应明确规定丢弃条件和处理方式。队列满与已关闭属于不同失败原因时用错误码区分；失败时应保留输出参数原值。不能持有保护任务队列的锁执行用户任务，也不能让池内工作线程同步等待同池唯一工作线程尚未执行的子任务：后者会死锁。

完整 C++11 示例与运行方法见[线程池题解](engineering/03_thread_pool.md)及[源文件](../engineering/concurrency/03_thread_pool.cpp)。本章保留原理与接口取舍，避免复制一份容易失同步的实现。

### 4. 取消、超时与后续标准

| 能力 | 版本 | 复习重点 |
|---|---|---|
| `shared_mutex` | C++17 | 多个读者可并行持共享锁，写者独占；读多写少也未必总比普通互斥锁快 |
| `scoped_lock` | C++17 | 用 RAII 同时锁住多把互斥锁，避免手写相反顺序 |
| `jthread` | C++20 | 对象销毁时若仍可 join，会请求停止并等待线程结束；引用对象仍须保持存活到退出 |
| `stop_token` | C++20 | 协作式停止请求；任务必须主动检查，不能强制终止任意阻塞操作 |
| `atomic::wait/notify` | C++20 | 等待原子值改变；仍要循环确认业务条件 |
| `counting_semaphore` / `latch` / `barrier` | C++20 | 分别用于许可计数、一次性会合、可重复阶段会合 |
| coroutine | C++20 | 语言层面的挂起/恢复机制；调度器、I/O 完成与结果类型仍需库或应用提供 |

`jthread` 的最小用法：

```cpp
// C++20：需要 <thread>。工作线程自愿检查停止请求。
std::jthread worker([](std::stop_token token) {
  while (!token.stop_requested()) {
    DoOneBoundedStep();  // 应能及时返回；此函数由调用方实现。
  }
});
```

这段是接口片段；`DoOneBoundedStep()` 不是标准库函数。若它无限阻塞，析构函数等待 `join()` 仍可能无限期等待。超时、取消、关闭是不同状态：超时只说明等候期限已过；请求停止只表达意愿；只有任务确认退出并完成清理，线程才真正结束。

### 5. 自检

- `submit()` 成功而任务返回 `ok_ == false`，分别发生了什么？
- `future.wait_for()` 超时后能否直接销毁它引用的数据？为什么？
- 只有一个工作线程的池中，父任务提交子任务后马上等子任务，会怎样？
- 为什么 `jthread` 自动 join 也不能拯救一个永远不检查停止请求的阻塞任务？

延伸阅读：[WG21 线程与 future 工作草案](https://eel.is/c++draft/thread)、[WG21 P0660R10：停止令牌与 jthread](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0660r10.pdf)。

---

## 模板编程：先理解类型替换，再看高级技巧

### 目标与直观例子

模板把“算法相同、类型不同”的代码写成一个定义，编译器在使用处用具体类型进行实例化。比如求两个值的较大者：`Max(3, 5)` 和 `Max(2.0, 4.0)` 共享逻辑，但会形成适合不同类型的实例。模板能减少重复，也会让错误出现在实例化点；阅读时先找清楚类型参数被推导成什么、有哪些操作必须可用。

### 1. 函数模板、类模板与推导（C++11）

```cpp
#include <string>

template <typename T>
T Max(const T& left, const T& right) {
  // 前提：T 支持 operator<，且返回值可以复制或移动。
  return left < right ? right : left;
}

template <typename T>
class Box {
 public:
  explicit Box(const T& value) : value_(value) {
  }
  const T& value() const {
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

`Max(1, 2.0)` 无法从两个实参同时推导出同一个 `T`；可显式写 `Max<double>(1, 2.0)`，或设计允许不同类型的接口。C++17 才提供类模板实参推导，且推导规则不总等同函数模板。

模板定义通常放在头文件，因为实例化点需要看见完整定义；若只允许少数类型，也可在实现文件显式实例化。头文件仍按仓库规范加防重复包含宏。

### 2. 值类别、移动和完美转发（C++11）

#### 左值、右值与右值引用

复习时先区分**表达式的值类别**和**变量声明的类型**。有名字的变量表达式是左值，即使变量类型为 `T&&`；临时构造的 `T(...)` 是纯右值；`std::move(variable)` 是将亡值。后两类都属于右值。`T&&` 是右值引用类型，可绑定右值；`const T&` 也可绑定右值，但通常不能通过它转移资源。

| 表达式 | 值类别 | 可用于理解什么 |
|---|---|---|
| `name`，其中 `name` 是 `std::string` 变量 | 左值 | 有名字，可继续使用 |
| `std::string("tmp")` | 纯右值 | 临时创建的结果 |
| `std::move(name)` | 将亡值 | 明确允许后续操作把 `name` 当作可转移资源的对象 |
| `alias`，其中 `alias` 声明为 `std::string&&` | 左值 | **有名字的右值引用变量仍是左值表达式** |

`std::move(x)` 本身只作类型转换，不执行移动构造或移动赋值；后续操作若选择相应的移动函数，资源才可能转移。下面 `Category` 的右值重载只返回数字，所以即使传入 `std::move(name)`，也没有实际移动字符串。`std::unique_ptr` 的例子才真正转移所有权。

```cpp
#include <memory>
#include <string>
#include <utility>

int Category(const std::string&) {
  return 1;
}
int Category(std::string&&) {
  return 2;
}

int main() {
  std::string name = "cpp";
  std::string&& alias = std::string("temporary");
  if (Category(name) != 1 || Category(std::move(name)) != 2 ||
      Category(alias) != 1) {
    return 1;
  }

  std::unique_ptr<int> source(new int(42));
  std::unique_ptr<int> destination = std::move(source);
  // unique_ptr 移动后，所有权属于 destination，source 为空。
  return source == nullptr && *destination == 42 ? 0 : 1;
}
```

调用移动操作后，原对象仍有效，但其值通常由类型自身的约定决定；可以销毁或重新赋值，不能擅自假定 `std::string` 一定变为空。`unique_ptr` 则明确保证移动后源指针为空。`std::move` 作用于 `const T` 会产生 `const T&&`，通常无法绑定接受 `T&&` 的移动构造函数，若有可用的复制构造函数就会复制。局部变量按值返回时通常直接写 `return value;`，不要机械写 `return std::move(value);`，以免妨碍某些复制省略优化。

#### 完美转发保留调用者的值类别

普通函数的 `std::string&&` 参数只能接收右值。函数模板中的 `T&&` 若 `T` 由该实参推导，则称为**转发引用**：传入左值时，`T` 推导为 `std::string&`；传入右值时，`T` 推导为 `std::string`。引用折叠规则使 `T& &&` 成为 `T&`，所以两种调用都能成立。进入函数后，参数 `value` 有名字，是左值表达式；必须写 `std::forward<T>(value)` 才能按调用者原本的左值或右值属性转交。

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
  return Category(std::forward<T>(value));
}

int main() {
  std::string name = "cpp";
  const int from_lvalue = ForwardCategory(name);
  const int from_rvalue = ForwardCategory(std::string("temporary"));
  return from_lvalue == 1 && from_rvalue == 2 ? 0 : 1;
}
```

如果模板内部直接写 `Category(value)`，两次调用都会选择左值重载。`std::forward<T>` 只适用于这种需要保留调用者值类别的转发路径；要明确转移一个已拥有对象的资源时用 `std::move`。泛型接口如果只读参数，写 `const T&` 往往更清楚。

一个常见用途是把构造参数传给目标类型：

```cpp
#include <memory>
#include <utility>

template <typename T, typename... Args>
std::unique_ptr<T> MakeObject(Args&&... args) {
  // 保留每个参数原本的左值或右值属性，转交给 T 的构造函数。
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
```

这是 C++11 写法；C++14 起优先使用标准库 `std::make_unique<T>(...)`。类若持有资源，优先交给 `unique_ptr`、容器等成员管理，让编译器生成或删除合适的复制、移动操作；确需手写析构、复制或移动函数时，要成组审查其所有权语义（常称为 Rule of Five）。C++11 的 lambda 不能写 `[](auto x)`，泛型 lambda 从 C++14 开始支持。

### 3. 特化、重载与 SFINAE

类模板可做全特化或偏特化；函数模板可重载，也可全特化，但**函数模板不能偏特化**。优先用重载表达普通的函数行为差异。SFINAE（替换失败不算错误）让不满足表达式要求的模板候选退出重载集合；这会增加诊断难度，C++20 可用 concepts 直接写约束。

```cpp
#include <type_traits>

template <typename T>
typename std::enable_if<std::is_integral<T>::value, bool>::type
IsEven(T value) {
  return value % 2 == 0;
}

int main() {
  return IsEven(42) ? 0 : 1;
}
```

`enable_if` 放在返回类型里时，若 `T` 不是整数类型，该候选会退出；如果没有其他重载，调用会产生编译错误。这里不是运行时返回 false。`static_assert` 适合在模板内部给出清晰的编译期诊断。C++17 的 `if constexpr` 可在模板内部按类型选择分支，未选分支不必对该实例合法；C++20 的 `requires`/concepts 可直接约束模板参数。

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

看到 `sizeof(MyClass)`、虚函数、继承和强制类型转换时，先分开两个层次：**标准保证对象有哪些子对象及其生命周期**；编译器为当前平台选择的**具体地址、填充、虚表表示与调用约定**。后者常见但不能作为跨编译器的语言规则。

例如一个对象有 `char` 和 `int` 两个成员，`sizeof` 往往大于二者大小之和，因为 `int` 可能要求更高对齐，类末尾也可能填充，以便数组中每个元素都满足对齐。但不要把某一台机器上打印出的 8 字节背成标准答案。

### 1. 对象、子对象、对齐与填充

完整对象可以包含基类子对象和非静态数据成员子对象。`sizeof(T)` 包含对象所占的字节数及必要的填充；`alignof(T)` 给出类型对齐要求。非静态成员通常属于每个对象；静态数据成员不作为每个实例的普通成员存储。成员函数的代码也不是每个对象都复制一份。

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
            << " id_offset=" << offsetof(Record, id_) << '\n';
  // 观察当前平台布局；不把输出常量当成可移植保证。
  return 0;
}
```

`offsetof` 只在满足其适用条件的类型上使用；这里的 `Record` 是标准布局类型。给成员换顺序可能改变填充，却也会改变二进制接口（ABI）和序列化假设。空类对象仍需要可区分的地址，`sizeof(Empty)` 不能是 0；空基类子对象可享受空基类优化，但具体大小仍需测量。C++20 的 `[[no_unique_address]]` 允许特定成员与其他子对象共享地址，不能直接套用普通成员的大小直觉。

### 2. 继承、虚函数与多态

非虚基类通常作为派生类对象的子对象存在；多重继承时，不同基类子对象可能位于不同地址，因此从派生类指针转换到基类指针可能调整指针值。虚继承使一份虚基类子对象由相关路径共享。不要用 `reinterpret_cast` 手算基类偏移。

虚函数调用按对象实际的动态类型选择最终覆盖函数。常见实现借助虚函数表和隐藏指针，但标准没有规定必须有名为 `vptr` 的字段或固定的虚表布局，也没有规定“有虚函数就一定多一个指针大小”。通过基类指针删除派生类对象时，基类析构函数应为虚函数。

```cpp
#include <memory>

class Shape {
 public:
  virtual ~Shape() = default;
  virtual int Area() const = 0;
};

class Square : public Shape {
 public:
  explicit Square(int side) : side_(side) {
  }
  int Area() const override {
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

`override` 从 C++11 起可让编译器检查确实覆盖了基类虚函数；若参数或 `const` 写错，会在编译时报错。按值把 `Square` 复制进 `Shape` 会发生对象切片，但上面 `Shape` 是抽象类，本身不能直接实例化；实际项目中若基类可实例化，按值传递基类就需要留意切片。多态对象用指针或引用传递，所有权优先用智能指针表达。

### 3. 构造、析构和生命周期

构造按“基类 → 成员声明顺序 → 构造函数体”进行，析构大体反向。成员**声明顺序**决定初始化顺序，不是初始化列表的书写顺序。对象构造或析构期间，虚函数调用不会按一个尚未构造好或已经销毁的派生部分进行动态分派。把 `this` 提前发布给别的线程，可能让它看到尚未完成初始化的对象。

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

此例初始化列表顺序恰好与声明顺序一致。若写成 `Counter() : step_(1), total_(step_) {}`，`total_` 实际会先读取尚未初始化的 `step_`，属于错误。把类用于多线程时，`const` 成员函数也不天然线程安全：若它读取其他线程正在修改的普通成员，仍需同步。

### 4. 指针、别名与二进制表示

指针值不保证总能直接按某个整数格式解释。不同类型通过 `reinterpret_cast` 指向同一片字节，并不自动赋予按新类型读取对象的权利；还要考虑对象生命期、对齐和类型访问规则。检查对象的字节表示可使用 `unsigned char` 等标准允许的字符类型；不要用 `memcpy` 为含指针、虚函数或其他非平凡资源的类“复制对象语义”。

跨进程文件格式和网络协议不应直接写入 `sizeof(T)` 个对象字节：填充、字节序、指针值和 ABI 都可能变化。明确字段宽度、编码、字节序与版本，再逐字段序列化。对于 C++11，可用 `std::is_standard_layout<T>`、`std::is_trivially_copyable<T>` 等类型特征检查部分性质，但它们不是“任何机器都具有相同二进制布局”的保证。

### 5. 自检

- `sizeof` 为什么可能包含尾部填充？为什么打印出的 `vptr` 偏移不能写入可移植代码？
- 构造函数初始化列表的顺序与成员实际初始化顺序不一致时，会出什么问题？
- 基类析构函数不为虚函数时，通过基类指针删除派生对象有什么风险？
- “对象地址可访问”是否等于“对象已完成构造、可以被另一个线程安全读取”？

延伸阅读：[WG21 工作草案对象章节](https://eel.is/c++draft/intro.object)、[类成员与布局章节](https://eel.is/c++draft/class.mem)、[继承与虚函数章节](https://eel.is/c++draft/class.derived)。
