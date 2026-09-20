# 用 CMake 管理 LeetCode 代码

## 推荐：使用 Make 快捷入口

根目录 [Makefile](../../Makefile) 参考 VSAG 的方式，将配置和编译封装成短命令。需要 CMake 3.20+、Make 和 C++17 编译器，无需预先配置。

```bash
make build                         # 配置并编译 Debug
make debug                         # 同 make build
make release                       # 配置并编译 Release
make build lc_704_binary_search     # 只编译这道题
make run lc_704_binary_search       # 编译并运行这道题
make test                          # 编译并测试全部题目
make help
```

Debug 输出到 `build/debug/bin/`，Release 输出到 `build/release/bin/`。`make test` 使用 Debug 的断言测试。默认并行度为 4，可通过 `JOBS=8` 调整。每次调用指定一个操作，`build`、`debug`、`release`、`run` 后可跟一个题目目标名；不存在的题目会报错。

这里的 `build`、`debug` 是 Makefile 定义的目标，并非 CMake 自定义参数。下面保留原生 CMake 手动命令，日常直接使用上面的 Make 命令即可。

### make build 和 make release 的区别

`make build` 在本仓库固定等同于 `make debug`，不会根据上次构建自动切换模式。两者都只负责配置和编译，不会自动运行程序或测试。

| 对比项 | `make build` / `make debug` | `make release` |
|---|---|---|
| 构建类型 | Debug（调试版本） | Release（优化版本） |
| 编译器优化 | 当前默认不启用优化，便于逐行调试 | 当前启用 `-O3` 优化，运行通常更快，单步调试可能跳行或看不到变量 |
| 调试信息 | 当前带 `-g`，方便断点、查看变量和调用栈 | 当前默认不带 `-g` |
| `assert` 断言 | 保留，运行时会检查条件 | 定义 `NDEBUG`，断言及其中的表达式不执行 |
| CTest 注册 | 开启（`BUILD_TESTING=ON`） | 关闭（`BUILD_TESTING=OFF`） |
| 程序目录 | `build/debug/bin/` | `build/release/bin/` |
| 使用场景 | 日常刷题、排查错误、验证结果 | 需要优化版本时；性能测量需另写实际执行算法的入口 |

上面的编译参数来自当前 Apple Clang 构建配置；其他编译器或手动覆盖参数后可能不同。两个构建目录独立，切换模式无需清理，也不会互相覆盖。

**日常刷题用 `make run <题目>`，全部验证用 `make test`。** `make run` 始终编译并运行 Debug；`make test` 始终编译 Debug 后运行全部已注册测试。

本仓库许多题目把算法调用直接写在 `assert(...)` 内，Release 会连同这些调用一起移除。因此 Release 程序正常退出不代表题解验证通过，也不能直接用这样的入口比较算法性能。

```bash
make build lc_704_binary_search       # 只编译单题的 Debug 版本
make run lc_704_binary_search         # 编译并运行 Debug 断言检查
make test                            # 编译并检查全部已注册题目
make release lc_704_binary_search     # 只编译单题的 Release 版本
./build/release/bin/lc_704_binary_search # 手动运行 Release，断言不会执行
```

## 为什么不是执行一次 cmake 就完成编译？

可以直接使用 CMake 命令，无需脚本，但配置和编译是两个阶段。以下手动示例输出到 `build/bin/`，Make 快捷入口的 Debug 输出到 `build/debug/bin/`：

- `cmake -S . -B build`：读取 `CMakeLists.txt`，检测编译器并生成构建规则。
- `cmake --build build`：调用生成器对应的构建工具（如 Make 或 Ninja），让编译器生成程序。
- `ctest --test-dir build`：运行已注册测试，不负责提前编译题目。

只执行 `cmake` 不带参数不会构建项目；`cmake .` 会把构建文件写到源码目录，建议使用 `-S`、`-B` 分离源码与产物。

## 从零构建

需 CMake 3.20+ 和支持 C++17 的编译器。在 macOS 上可用 `brew install cmake` 安装 CMake；缺少 Apple Clang 时运行 `xcode-select --install` 安装命令行开发工具。

以下命令都在仓库根目录执行：

```bash
cmake --version
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

`-S .` 指定源码根目录，`-B build` 指定构建目录。`Debug` 保留调试信息，也避免 Release 配置中的 `NDEBUG` 禁用本仓库的断言测试。`--parallel` 允许并行编译。

希望一条命令完成配置和编译，可以串联两个阶段，前一步成功才运行后一步：

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build --parallel
```

## 日常只构建一道题

完成首次配置后执行：

```bash
cmake --build build --target lc_704_binary_search
./build/bin/lc_704_binary_search
ctest --test-dir build -R '^lc_704_binary_search$' --output-on-failure
```

`target` 是构建目标。本仓库一道已注册题目对应一个可执行程序；构建单个 target 不会编译其他题目。程序使用断言检查结果，无输出且退出码为 0 通常表示通过。

修改 `.cpp` 后直接再次执行构建即可。新增或修改 CMake 注册项后，可以显式重新配置；常用的 Make/Ninja 生成器也会在构建时自动检查 CMake 文件并重新生成规则。

## 按类别测试与查看题目清单

```bash
# 测试不会自动编译，所以先构建
cmake --build build --parallel
ctest --test-dir build -L '^binary_search$' --output-on-failure

# 查看所有已注册测试，无需先编译
ctest --test-dir build -N
```

可构建题目的权威清单是 [algo/CMakeLists.txt](../../algo/CMakeLists.txt)。当前有 7 道；未注册的历史代码不参与默认构建。

## CMake 文件如何分工

- [根 CMakeLists.txt](../../CMakeLists.txt)：项目、C++17、CTest，以及 `add_subdirectory(algo)`。
- [algo/CMakeLists.txt](../../algo/CMakeLists.txt)：逐题列出名称、源码路径和分类。
- [公共函数](../../cmake/AddLeetCodeProblem.cmake)：检查参数、创建可执行目标、设置警告和输出目录、注册测试。

多参数调用采用每个参数组一行、四空格缩进，题目之间留空行。原来把参数写在一行是合法语法，但较长的注册项不便阅读和审查。

新增题目时复制 [模板](../../tools/templates/code_template.cpp)，在 `algo/CMakeLists.txt` 注册：

```cmake
add_leetcode_problem(
    NAME lc_123_example
    SOURCE string_and_array/123_example.cpp
    CATEGORY array
)
```

`SOURCE` 相对于当前 `CMakeLists.txt` 所在目录，此处为 `algo/`，所以不用再写 `algo/` 前缀。不要自动扫描并编译所有 `.cpp`：历史聚合文件仍存在重复定义等问题。

## 可选的 lc.sh

[tools/lc.sh](../../tools/lc.sh) 只是便捷入口。它会先配置，再执行相应操作；从任意工作目录调用均可。默认构建目录为仓库内的 `build/`，默认配置为 Debug。

| 快捷命令 | 实际操作 |
|---|---|
| `./tools/lc.sh configure` | `cmake -S <仓库> -B <构建目录> -DCMAKE_BUILD_TYPE=Debug` |
| `./tools/lc.sh build` | 配置，然后 `cmake --build <构建目录>` |
| `./tools/lc.sh build lc_704_binary_search` | 配置，然后构建指定 target |
| `./tools/lc.sh run lc_704_binary_search` | 配置、构建指定 target，再执行程序 |
| `./tools/lc.sh test` | 配置、构建全部、运行全部 CTest 测试 |
| `./tools/lc.sh test binary_search` | 配置、构建全部、按 CTest 标签筛选测试 |
| `./tools/lc.sh list` | 配置，再用 `ctest -N` 列出题目测试 |

运行 `./tools/lc.sh --help` 可查看帮助。可用 `BUILD_DIR` 和 `BUILD_TYPE` 覆盖默认值；建议 `BUILD_DIR` 使用绝对路径。

## 常见问题

| 现象 | 原因与处理 |
|---|---|
| 配置成功却没有可执行文件 | 还需执行 `cmake --build build` |
| CTest 报找不到程序 | 先构建相应 target；运行全量测试前先构建全部 |
| `Source file does not exist` | 检查相对于 `algo/` 的 `SOURCE` 路径 |
| `Duplicate LeetCode target` | 两道题用了相同 `NAME` |
| 找不到 `main` 或出现重定义 | 题目还不是可独立编译的一题一文件 |
| 找不到新 target | 检查注册项并重新执行配置命令 |
| 切换编译器或生成器后缓存冲突 | 指定一个新的构建目录，再配置和编译 |
| 目录迁移后使用旧规则 | 在根目录重新执行配置命令，刷新 `build/` 中的路径 |

以上示例面向默认 Make/Ninja 这类单配置生成器。若显式选择 Xcode 或 Ninja Multi-Config，构建和测试分别加 `--config Debug`、`-C Debug`，程序位于 `build/bin/Debug/`；快捷脚本面向单配置生成器。

项目开启 `CMAKE_EXPORT_COMPILE_COMMANDS`，Make/Ninja 配置后会生成 `build/compile_commands.json`，供编辑器读取编译参数。
