# C++11 与检索系统工程面试题库

面向具备 3 年以上 C++ 开发经验、长期从事广告检索或通用搜索的候选人。题库同时用于面试官复习：先理解问题与正确性要求，再阅读最小实现，不要求记住完整代码。

## 怎样复习和使用

每题分为**面试题目（提供给候选人）**和**面试官答案**。出题时只提供前者；答案中的代码、反例和评分依据留给面试官。全部实现使用 C++11、标准库和教学规模数据，无第三方运行依赖。

每次复习按三个层次进行：

1. **两分钟回忆**：读开头的复习主线，合上文档说出“解决什么问题、凭什么正确”。
2. **十分钟推导**：手动走一个小例子，画出状态/数据流，说出最危险的反例。
3. **按需看代码**：只看核心类/函数；main 是测试区，不需要背诵。再选一个资深追问，说明改变约束后哪些推导会失效。

源码刻意不堆叠完整生产框架。无锁题只实现单生产者单消费者队列（SPSC），Roaring 只实现两类容器，实时索引和分片题只实现关键内存机制。更复杂的使用条件和故障情况在题解中说明，不能把教学示例直接视为线上库。

## 题目导航

Bitmap 专题从[三题原理与实现导读](bitmap_basics.md)开始：普通 Bitmap → Roaring Bitmap → Bloom Filter，聚焦基础数据结构和 C++11 简单实现。

| 题目 | 先记住的原理 | 运行目标 |
|---|---|---|
| [01 C++11 多线程](01_thread_basics.md) | 线程退出前对象不能销毁，共享数据访问要同步，结果通过 future 获取 | `eng_thread_basics` |
| [02 有界阻塞队列](02_blocking_queue.md) | 两个等待条件；关闭后仍取完已有任务 | `eng_blocking_queue` |
| [03 极简线程池](03_thread_pool.md) | 持有互斥锁时取任务，释放锁后执行，future 获取结果 | `eng_thread_pool` |
| [04 SPSC 无锁队列](04_spsc_queue.md) | 写入后才能读取，读取后才能覆盖 | `eng_spsc_queue` |
| [05 索引快照](05_index_snapshot.md) | 不修改已供查询使用的版本，安全替换指针，旧查询结束后再释放旧版本 | `eng_index_snapshot` |
| [06 Bitmap](06_bitmap.md) | 空间取决于编号范围，集合运算按 uint64_t 数组元素执行 | `eng_bitmap` |
| [07 Roaring Bitmap](07_roaring_bitmap.md) | 每组编号分别根据元素数量选择数组或位图 | `eng_roaring_bitmap` |
| [08 Bloom Filter](08_bloom_filter.md) | 一个 0 即可排除，全为 1 时仍需精确查找 | `eng_bloom_filter` |
| [09 Posting List](09_posting_list.md) | 有序、去重、游标单调前进 | `eng_posting_list` |
| [10 Posting 压缩](10_posting_compression.md) | 保存相邻编号的差值，再用变长整数编码减少字节数 | `eng_posting_compression` |
| [11 最小倒排索引](11_inverted_index.md) | 按词找候选、求交、过滤删除 | `eng_inverted_index` |
| [12 Top K 剪枝](12_topk_pruning.md) | 估计的最高分不能低于实际最高分，同分时也要比较 docID | `eng_topk_pruning` |
| [13 实时更新与合并](13_realtime_segments.md) | 先版本后匹配，合并不改逻辑结果 | `eng_realtime_segments` |
| [14 分片合并](14_shard_merge.md) | 各分片使用相同排序规则，每次取各分片尚未输出的第一项进行比较 | `eng_shard_merge` |
| [15 延迟排障](15_latency_diagnosis.md) | 拆解时间，每个假设配证据 | `eng_latency_diagnosis` |
| [16 LRU Cache](16_lru_cache.md) | 哈希表定位节点，命中和更新移到表头，容量满时淘汰表尾 | `eng_lru_cache` |

建议学习顺序：并发 `01→02→03→04→05`；检索 `06→09→11→10→07→08→12`；系统 `05→13→14→15`。

缓存题 `16` 可独立学习，先掌握单线程实现，再追问并发访问、按字节限制容量与缓存污染。

## 两套可直接使用的面试组合

| 阶段 | 60 分钟：并发与检索基础 | 90 分钟：资深检索深度 |
|---|---|---|
| 目标确认 | 5 分钟：确认项目职责与实际负责模块 | 5 分钟：明确索引规模、更新延迟与本人贡献 |
| 编码 | 20 分钟：02 阻塞队列核心，追问关闭 | 25 分钟：03 线程池核心，追问失败状态、关闭前完成任务，以及等待子任务的风险 |
| 检索原理 | 15 分钟：09 AND/advance，追问 06/07 的选择 | 20 分钟：12 上界与同分反例，追问倒排列表及压缩 |
| 系统与排障 | 15 分钟：13 更新与旧版本失效 | 20 分钟：13 发布、删除、恢复；10 分钟：15 排障 |
| 收尾 | 5 分钟：候选人问题与记录 | 10 分钟：澄清薄弱点、候选人问题与记录 |

无锁岗位可用 04 替换并发编码；分布式岗位可用 14 替换系统题。不在一场面试里要求完成全部题目，也不把系统设计题的完整代码作为限时要求。

## 分层评价：记录证据，不数术语

每道题按 0～3 记录，同时独立记录提示：

| 层级 | 可观察证据 |
|---|---|
| 0：未形成可用方案 | 核心行为不符合题目要求，给出反例后仍无法修正 |
| 1：基础可用 | 能实现题目要求的核心功能，讲清输入输出与主要边界 |
| 2：资深理解 | 独立解释方案为何正确，能解释反例、成本与约束变化 |
| 3：专家加分 | 能把机制放回真实系统，提出可验证的取舍及失败恢复方案 |

提示记录用“无提示／方向提示／给出关键机制”，不要把提示后的复述等同于独立推导。先给出一个能使当前方案出错的例子，让候选人尝试修正，观察其定位和解释过程；普通 API 拼写错误与数据竞争、漏结果、错误声称数据在重启后仍能保留应分开评价。

总体判断按能力维度综合：C++ 正确性、索引理解、工程取舍、验证与排障。对于长期检索岗位，单靠并发题高分不能替代索引与查询原理证据；专家加分不是 3 年候选人的统一最低要求，也不凭单题打分自动决定录用。

面试记录模板：

```text
题号／实际约束：
候选人的核心思路与必须保证的条件：
独立找到的反例：
提示内容与修正过程：
基础／资深／专家证据：
尚未验证的能力：
```

## 编译、测试与复核

在仓库根目录：

```bash
make run eng_thread_pool
make test
ctest --test-dir build/debug -L engineering --output-on-failure
make release
```

新增工程目标明确使用 C++11；现有算法目标保留 C++17。CTest 给每道工程题 30 秒超时。`CHECK` 在 Release 仍执行，测试操作不依赖 assert；现有算法题的 Release 断言行为保持原样。

并发示例用线程间通知、join 和条件检查控制执行顺序，不用固定 sleep 推断正确性。随机对照固定种子；性能数据仅展示测量方法，不作为跨机器通过阈值。

### 可选 ThreadSanitizer（运行时数据竞争检测工具）

使用支持 TSan 的 Clang/GCC 与运行平台，在独立目录构建：

```bash
cmake -S . -B build/tsan -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON \
  -DCMAKE_CXX_FLAGS="-fsanitize=thread -g -O1" \
  -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=thread"
cmake --build build/tsan --target eng_thread_basics eng_blocking_queue eng_thread_pool eng_spsc_queue eng_index_snapshot eng_latency_diagnosis --parallel 4
ctest --test-dir build/tsan -R '^eng_(thread_basics|blocking_queue|thread_pool|spsc_queue|index_snapshot|latency_diagnosis)$' --output-on-failure
```

不支持 TSan 的环境不把失败当成算法结论；出现运行时或权限限制要单独说明。即使测试和 TSan 全通过，也不能证明所有调度下正确，仍需检查同步关系和对象生命周期。

实现入口：[engineering/README.md](../../engineering/README.md)。专业来源放在相关题目的解释处，参考代码与真实引擎实现明确区分。

### 本轮验证记录（2026-09-20）

- Apple Clang 14.0.3：15 个工程目标在 Debug/Release 的编译命令均包含 `-std=c++11` 和 `-fno-exceptions`。
- Debug：原有 7 个算法测试与新增 15 个工程测试全部通过；Release：构建成功，15 个工程程序的 CHECK 均通过。
- ThreadSanitizer：5 个并发专题及延迟诊断示例通过，包含线程池多提交者与关闭竞争场景。
- 文档本地链接、clang-format 格式检查和 5 个公共头文件的独立编译检查通过。历史算法 Release 的未使用变量警告仍保留，不属于新增工程目标。

## 后续文档的写法

- 先说明问题和处理步骤，再解释必要的专业术语。英文缩写首次出现时给出中文含义，每篇题解应能独立阅读。
- 写清执行者、操作对象和发生条件。例如写“释放保护任务队列的互斥锁后执行任务”，不用让读者猜测的简称。
- 用具体例子说明错误会怎样发生；保留正确性条件和复杂度，不用术语数量代替解释。
- 文档中的代码片段与参考代码使用相同格式。文件头与中文注释规范见 [代码目录说明](../../engineering/README.md)。

代码统一使用 C++11 并禁用异常，通过 bool、错误码或结果结构体报告失败；接口约定和资源失败边界见[代码说明](../../engineering/README.md#错误处理约定)。后续维护遵循根目录 [AGENTS.md](../../AGENTS.md)。
