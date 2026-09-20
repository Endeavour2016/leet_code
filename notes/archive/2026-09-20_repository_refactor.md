# 2026-09-20 目录与构建入口重构

## 目录整理

- 所有算法分类、题集、公共头文件和历史聚合题解统一移入 `algo/`，保留原分类名和源码内容。
- 算法专题笔记移入 `notes/algo/`，刷题规范与构建指南移入 `notes/workflow/`。
- 旧笔记、首轮实施记录归入 `notes/archive/`，新题模板移入 `tools/templates/`。
- 根目录新增 README，集中提供目录导航、构建示例和新增题目流程；同步修正笔记源码链接。

## 构建入口

- 根 `CMakeLists.txt` 负责项目配置，使用 `add_subdirectory(algo)` 引入题目。
- `algo/CMakeLists.txt` 显式注册 7 道可独立运行的题目；公共函数按当前子目录解析源码路径。
- CMake 注册项按参数分行，采用四空格缩进。
- 手写的根 Makefile 封装 CMake 命令；CMake 自动生成的编译规则保存在构建目录中，两者职责不同。

| 命令 | 行为 |
|---|---|
| `make build` / `make debug` | 配置并编译 Debug |
| `make release` | 配置并编译 Release |
| `make build <题目>` | 配置并编译指定 Debug 题目 |
| `make run <题目>` | 编译并运行指定 Debug 题目 |
| `make test` | 编译 Debug 并运行全部已注册测试 |
| `make help` | 显示命令和模式差异 |

Debug 与 Release 分别输出到 `build/debug/bin/`、`build/release/bin/`。Debug 保留断言用于验证；Release 默认禁用断言且不注册测试。由于部分算法调用位于断言内部，Release 退出成功不代表题解正确。

保留原生 CMake 手动构建方式和可选 `tools/lc.sh`。最终采用 Make 作为日常入口，不保留 CMake 预设文件，避免重复维护配置。

## 验证范围

- 迁移前后 C++ 源码和头文件逐字节一致，本次不修改算法实现。
- 检查现行文档的相对链接、源码行号及本地 include 路径。
- 验证 Debug、Release 构建，单题运行及全部 7 项 Debug 断言测试。
- 检查错误题目名和缺少运行目标时返回失败。
- 历史聚合文件仍未全部拆分或验证，不参与默认构建；历史归档保留当时的记录。

使用入口：[仓库 README](../../README.md)；详细说明：[构建指南](../workflow/10_cmake_for_leetcode.md)。
