# LeetCode 笔记、CMake 工程化与渐进式整理实施计划

> 日期：2026-09-19
> 状态：首轮实施已完成

## 1. 目标

- 使用 CMake 管理“一题一个 `.cpp`、每题自带 `main`”的可执行程序。
- 使用 Apple Clang、Unix Makefiles 和 C++17，不引入第三方单元测试框架。
- 将算法笔记拆分成可维护的专题文档，并通过源码链接关联仓库内代表题目。
- 保留现有算法分类目录，采用渐进式迁移，避免一次性重写历史代码。
- 只把已迁移、可独立运行的题目加入默认构建，保证默认构建始终可用。

## 2. CMake 构建设计

根目录使用 `CMakeLists.txt` 管理已迁移题目，公共注册逻辑位于 `cmake/AddLeetCodeProblem.cmake`。

每道题注册为一个独立 executable target：

```cmake
add_leetcode_problem(
  NAME lc_704_binary_search
  SOURCE binary_search/704_binary_search.cpp
  CATEGORY binary_search
)
```

`add_leetcode_problem` 负责：

- 检查 `NAME`、`SOURCE`、`CATEGORY` 参数和源文件是否存在。
- 创建独立可执行程序并启用 C++17。
- 添加 `-Wall -Wextra -Wpedantic` 编译警告。
- 将程序输出到 `build/bin/`。
- 把题目程序注册为 CTest 测试。
- 使用算法类别作为 CTest 标签。

原生 CMake 操作：

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target lc_704_binary_search
./build/bin/lc_704_binary_search
ctest --test-dir build --output-on-failure
ctest --test-dir build -L binary_search --output-on-failure
```

日常快捷入口：

```bash
./tools/lc.sh configure
./tools/lc.sh run lc_704_binary_search
./tools/lc.sh test
./tools/lc.sh test binary_search
./tools/lc.sh list
```

快捷脚本只封装原生 CMake 命令，CMake 教程仍以理解 configure、build、target 和 test 为重点。

## 3. 题目文件规范

- 文件名采用 `<题号>_<snake_case_题名>.cpp`。
- 一个文件只对应一道题，可以包含多种解法，但类名或函数名不得重复。
- 每个题目文件直接包含普通 `main`，不使用 `LOCAL_TEST` 条件宏。
- `main` 至少覆盖官方示例、关键边界和一个容易出错的反例，并使用标准 `assert`。
- 提交 LeetCode 时只复制解答类或函数，不复制 `main`。
- 文件头记录核心思路、不变量、时间复杂度、空间复杂度和复盘要点。
- 新文件只包含实际需要的标准头文件，优先使用 `const T&`、`nullptr` 和有语义的变量名。

## 4. 第一批迁移

- 将二分查找规范为 `binary_search/704_binary_search.cpp`，作为完整示例。
- 将 `DP/all_dp_solution.cc` 拆分为：
  - `DP/121_best_time_to_buy_and_sell_stock.cpp`
  - `DP/122_best_time_to_buy_and_sell_stock_ii.cpp`
  - `DP/bm74_restore_ip_addresses.cpp`
- 修复 `string_and_array/27_remove_element.cpp` 的语法错误并补充测试。
- 整理 `string_and_array/3sum.cpp`，使多种解法使用不同名称。
- 将 `others/shuffle.cpp` 从 `random_shuffle` 更新为 C++17 的 `std::shuffle`。
- 更新 `code_template.cpp`，移除 Windows 专用 `system("pause")`，改用断言测试。
- 不修改已有未提交内容 `tree/binary_tree.cpp` 和未跟踪目录 `LCR/`。
- 暂不迁移 `leet_code_top100.cpp`、`linked_list/linked_list.cpp`、`tree/binary_tree.cpp` 等大型聚合文件。

## 5. 仓库卫生

- 在 `.gitignore` 中忽略 `build/`、`.build/`、可执行文件和调试产物。
- 从版本控制中移除可由源码重新生成的历史二进制文件。
- 不提交包含个人绝对路径的 VS Code 配置。
- 通过 `CMAKE_EXPORT_COMPILE_COMMANDS` 生成 `build/compile_commands.json`，供 VS Code 或 clangd 使用。
- 历史聚合文件不进入默认 CMake 构建，避免影响日常练习。

## 6. 笔记结构

笔记入口为 `notes/README.md`，专题包括：

1. `01_array_string_hash.md`
2. `02_two_pointers_window.md`
3. `03_stack_queue_heap.md`
4. `04_linked_list.md`
5. `05_tree_trie_search.md`
6. `06_binary_search.md`
7. `07_dynamic_programming_greedy.md`
8. `08_bit_math_misc.md`
9. `09_repository_review_and_workflow.md`
10. `10_cmake_for_leetcode.md`

每章统一包含：

- 一句话核心思想和适用信号。
- 解题时必须维护的不变量。
- 可默写的 C++17 模板。
- 时间、空间复杂度。
- 必要的 Mermaid、ASCII 图或状态转移表。
- 仓库内代表题目及源码位置。
- 常见错误、边界条件和面试口述步骤。
- 从基础题到变式题的复习清单。
- 对未执行验证的历史代码明确标记为“待验证”。

## 7. 推荐刷题流程

1. 从 `code_template.cpp` 复制新题文件。
2. 先写官方示例、边界和反例断言。
3. 实现 LeetCode 风格的解答类或函数。
4. 在 `CMakeLists.txt` 中注册 target。
5. 使用 `./tools/lc.sh run <target>` 编译并运行。
6. 提交 LeetCode 时只复制题解主体。
7. 将平台发现的失败用例补回本地 `main`。
8. 通过后补充不变量、复杂度和首次错误原因。
9. 复习旧聚合文件时，顺手将当前题迁移为独立文件。

## 8. 验收标准

- CMake 可以从空的 `build/` 完成配置。
- 默认构建中的所有 target 必须零编译错误。
- 每个 target 可单独构建和运行。
- 全部已注册 CTest 测试通过。
- 可以使用算法类别标签筛选测试。
- 不存在的 target 必须返回非零状态和清晰错误。
- `build/compile_commands.json` 正常生成。
- 笔记中的源码路径和行号有效。
- 笔记不含未完成占位符，Mermaid 代码块完整闭合。
- 不覆盖用户已有的 `tree/binary_tree.cpp` 和 `LCR/` 修改。

## 9. 首轮实施结果

- 已安装并验证 CMake 4.4.3 与 CTest 4.4.3。
- 7 个题目 target 构建成功，CTest 结果为 7/7 通过。
- 按 `binary_search` 标签执行分类测试通过。
- 全仓库 C++17 语法检查从 70 个通过、8 个失败改善为 76 个通过、4 个失败。
- 剩余失败均为暂缓迁移的历史聚合文件。
- 11 份 Markdown 笔记的导航、源码链接和行号检查通过。
- 17 个已跟踪的生成二进制文件已从工作树移除。

## 10. 后续迭代

后续不进行大爆炸式重构，而是按复习顺序逐批推进：

1. 拆分 `linked_list/linked_list.cpp`。
2. 在不覆盖当前修改的前提下拆分 `tree/binary_tree.cpp`。
3. 整理 `LCR/lcr_solution.cc` 中的同名函数。
4. 最后迁移 `leet_code_top100.cpp`，将其逐步降级为题目索引或历史归档。
5. 每迁移一道题，同步添加 CMake target、断言测试和笔记链接。
