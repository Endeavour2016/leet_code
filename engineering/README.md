# 工程面试参考实现（C++11）

从[中文题库导航](../notes/engineering/README.md)开始复习，先读题意、实现思路和正确性要求，再读代码。

Bitmap 相关三题见[原理与简单实现导读](../notes/engineering/bitmap_basics.md)，对应 `eng_bitmap`、`eng_roaring_bitmap`、`eng_bloom_filter`。

- `concurrency/`：线程基础、阻塞队列、线程池、SPSC、快照。
- `index/`：Bitmap、Roaring、Bloom、Posting、压缩、最小倒排。
- `retrieval/`：Top K、实时段、分片归并和延迟诊断。
- `cache/`：LRU 缓存，使用哈希表和双向链表维护最近使用顺序。
- `common/`：被示例复用的简短组件与始终执行的 CHECK。

每个 `.cpp` 是独立目标，文件前部是核心实现，`main` 是验证区。队列、位图和 Posting 的共享实现放在对应头文件，题解直接链接其定义。系统设计示例是内存模型，不是生产服务框架。

```bash
make run eng_spsc_queue
make test
```

所有工程目标通过独立注册函数要求 C++11、关闭扩展、禁用异常并链接系统线程库。测试按 `engineering` 及 `concurrency/index/retrieval/cache` 标签注册；单题超时 30 秒。可选 TSan 命令、题目列表和面试评分在题库导航中。

## 代码格式与中文注释

代码布局以 [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html) 为基础，配置保存在 [.clang-format](.clang-format)：两空格缩进、每条语句单独一行、控制语句使用完整花括号，避免将多层条件或多项操作挤在一行。示例继续使用题库要求的 C++11。

文件头采用适合 C++ 的 [Doxygen 文档注释](https://www.doxygen.nl/manual/docblocks.html)。这种 `/** ... */` 写法与 JavaDoc 相近，统一说明用途、核心思路和使用限制；可运行文件另列运行方式。示例：

```cpp
/**
 * @file 03_thread_pool.cpp
 * @brief 实现固定线程数的 C++11 线程池。
 * @details 持有互斥锁时取出任务，释放锁后执行，通过 future 获取结果。
 * @note 关闭后不再接收新任务，已接受的任务仍需执行完。
 * @see notes/engineering/03_thread_pool.md
 * @par 运行方式
 * 在仓库根目录执行：make run eng_thread_pool
 */
```

函数和具体步骤使用中文 `//` 注释，优先解释调用条件、返回值含义、数据如何在线程间传递，以及为什么需要某个边界检查。简单赋值不逐行重复解释。头文件的防重复包含宏以完整路径命名，并在结尾标注对应宏名。

使用 clang-format 15 或更新版本，在仓库根目录执行：

```bash
# 自动整理 C++ 源文件和头文件。
rg --files engineering -g '*.cpp' -g '*.h' | xargs clang-format -i

# 只检查格式，不修改文件。
rg --files engineering -g '*.cpp' -g '*.h' | xargs clang-format --dry-run --Werror
```

本次使用 clang-format 21.1.4 完成整理。`InsertBraces` 会补全控制语句花括号，修改后仍应编译并运行 `make test`，检查结果是否正确。

## 错误处理约定

通用约束见根目录 [AGENTS.md](../AGENTS.md)。示例不使用异常处理：简单失败返回 bool，需要区分原因时返回错误码，业务任务通过结果结构体报告失败。失败时输出参数保持不变，调用者检查成功状态后再使用输出。

GCC/Clang 工程目标增加 `-fno-exceptions`；MSVC 使用 `/EHs-c-` 和 `_HAS_EXCEPTIONS=0`。历史算法目标的编译选项不受影响。保留 promise/future 传递任务结果，但不传递异常。

这些选项不会把标准库的内存分配失败或线程创建失败变成可恢复错误码；当前教学实现不提供这类资源失败的恢复能力。`CHECK` 仅用于测试，Debug 和 Release 都会执行检查。
