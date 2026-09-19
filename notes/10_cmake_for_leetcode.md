# 用 CMake 管理 LeetCode 代码

## 三个角色

- **编译器 Clang**：把一个 `.cpp` 变成可执行程序。
- **Make**：按照依赖关系执行多条编译命令，只重建发生变化的目标。
- **CMake**：读取 `CMakeLists.txt`，为 Make 生成构建规则。

可以把 CMake 想成施工图设计师，Make 是现场调度，Clang 是实际加工机器。

```mermaid
flowchart LR
  A[CMakeLists.txt] -->|cmake -S -B| B[Makefiles]
  B -->|cmake --build| C[Clang 编译命令]
  C --> D[build/bin/题目程序]
  D -->|ctest| E[运行 main 中的断言]
```

## 安装与确认

当前 macOS 使用 Homebrew 安装：

```bash
brew install cmake
cmake --version
ctest --version
```

本仓库验证时安装的是 CMake 4.4.3；项目只要求 3.20 或更高版本。CMake 会自动使用系统已有的 Apple Clang 和 Unix Makefiles。

## 第一次配置

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
```

- `-S .`：源码目录是当前仓库。
- `-B build`：所有生成文件放进 `build/`，不污染源码目录。
- `Debug`：保留调试信息，适合刷题阶段。

配置只需在首次构建或 `CMakeLists.txt` 改变后运行；重复运行是安全的。`build/CMakeCache.txt` 保存本次配置。如果切换编译器或生成器，最稳妥的做法是删除整个 `build/` 后重新配置。

## 构建、运行与测试

```bash
cmake --build build --target lc_704_binary_search
./build/bin/lc_704_binary_search
ctest --test-dir build --output-on-failure
ctest --test-dir build -L binary_search --output-on-failure
```

`target` 是 CMake 的核心单位。本仓库中每道规范题都是一个 executable target；只构建一个 target 时，不会编译其他题目。

## 注册新题

在根目录 [CMakeLists.txt](../CMakeLists.txt#L12-L19) 增加：

```cmake
add_leetcode_problem(
  NAME lc_123_example
  SOURCE string_and_array/123_example.cpp
  CATEGORY array
)
```

辅助函数 [AddLeetCodeProblem.cmake](../cmake/AddLeetCodeProblem.cmake#L3-L37) 会检查三个参数和源文件、创建 executable、启用 C++17、添加警告选项、设置输出目录，并注册 CTest。

## 快捷脚本

```bash
./tools/lc.sh configure
./tools/lc.sh run lc_704_binary_search
./tools/lc.sh test
./tools/lc.sh test binary_search
./tools/lc.sh list
```

脚本只是对原生命令的薄封装：[lc.sh](../tools/lc.sh#L1-L61)。学习阶段建议先手动执行原生命令，再使用脚本提高日常速度。

## 常见错误

| 错误 | 原因与处理 |
|---|---|
| `Source file does not exist` | `SOURCE` 路径相对仓库根目录写错 |
| `Duplicate LeetCode target` | 两道题使用了相同 `NAME` |
| `Undefined symbols ... main` | 题目文件没有普通 `main` |
| `redefinition of Solution` | 一个源文件混入多道题或重复类名 |
| 找不到新 target | 修改 `CMakeLists.txt` 后尚未重新 configure |
| 缓存指向旧编译器 | 删除 `build/` 后重新 configure |

## VS Code

本项目设置了 `CMAKE_EXPORT_COMPILE_COMMANDS`，配置后会生成 `build/compile_commands.json`。C/C++ 或 clangd 扩展可读取它获得准确的标准、头文件路径和编译选项，无需提交包含个人绝对路径的 `.vscode` 配置。
