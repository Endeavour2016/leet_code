# LeetCode 刷题与复习

算法代码放在 `algo/`，复习笔记从 [notes/README.md](notes/README.md) 开始阅读。

## 日常构建

在仓库根目录执行，需 CMake 3.20+、Make 和 C++17 编译器：

```bash
make build                         # 配置并编译 Debug
make debug                         # 同 make build
make release                       # 配置并编译 Release
make build lc_704_binary_search     # 只编译一道题
make run lc_704_binary_search       # 编译并运行一道题
make test                          # 编译并运行全部断言测试
make help                          # 查看帮助
```

首次使用也只需一条命令，Makefile 会自动执行 CMake 配置和编译。默认并行度为 4，可用 `make build JOBS=8` 调整。

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


当前只构建已注册的 7 道题目。原生 CMake 命令仍可使用，详见 [构建指南](notes/workflow/10_cmake_for_leetcode.md)。

## 目录约定

```text
leet_code/
├── Makefile                # 日常入口：make build / debug / release / test
├── CMakeLists.txt          # 项目配置与 algo 子目录入口
├── algo/                   # 题目代码
│   ├── CMakeLists.txt      # 可构建题目的注册清单
│   ├── binary_search/     # 按现有算法专题分类
│   ├── DP/
│   ├── string_and_array/
│   ├── ...                # 链表、树、栈、队列等其余专题
│   ├── LCR/               # 系列题集
│   ├── now_coder_top100/
│   ├── leet_code_top100.cpp # 历史聚合题解，暂不参与构建
│   ├── common/            # 旧公共代码
│   ├── head_file.h        # 旧题共用头文件
│   └── data_structure.h
├── notes/
│   ├── README.md          # 笔记导航
│   ├── algo/              # 算法模型与专题复习
│   ├── workflow/          # 刷题规范、CMake 构建指南
│   └── archive/           # 旧笔记和历史实施记录
├── tools/
│   ├── lc.sh              # 可选快捷脚本
│   └── templates/         # 新题模板
├── cmake/                 # CMake 公共辅助函数
└── build/                 # 本地构建产物，不提交 Git
```

题目分类目录整体移入 `algo/`，共用头文件随之移动，保留旧题的相对 include 路径。新题继续采用一题一文件；历史聚合题解在复习时逐步拆分。

## 新增一道题

```bash
cp tools/templates/code_template.cpp algo/string_and_array/123_example.cpp
```

写好题解和 `main` 中的断言后，在 [algo/CMakeLists.txt](algo/CMakeLists.txt) 中增加：

```cmake
add_leetcode_problem(
    NAME lc_123_example
    SOURCE string_and_array/123_example.cpp
    CATEGORY array
)
```

`SOURCE` 相对于 `algo/`；`NAME` 是构建目标名，`CATEGORY` 是测试标签。随后执行：

```bash
make run lc_123_example
```

## 可选快捷脚本

脚本仍可使用，默认输出到独立的 `build/bin/`（`make build` 输出到 `build/debug/bin/`）：

```bash
./tools/lc.sh --help
./tools/lc.sh build                      # 配置并编译全部已注册题目
./tools/lc.sh run lc_704_binary_search    # 配置、编译并运行单题
./tools/lc.sh test binary_search         # 编译后按标签测试
```
