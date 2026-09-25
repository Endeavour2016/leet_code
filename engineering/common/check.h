/**
 * @file check.h
 * @brief 提供 Debug 和 Release 都执行的测试检查工具。
 * @details CHECK 在条件不成立时报告表达式和位置，然后终止测试进程。
 * @note 仅用于示例测试；不会像 assert 一样在 Release 中被移除。
 */

#ifndef LEET_CODE_ENGINEERING_COMMON_CHECK_H_
#define LEET_CODE_ENGINEERING_COMMON_CHECK_H_

#include <cstdio>
#include <cstdlib>

// 仅供测试：失败时输出表达式和位置后终止，Release 中也始终执行检查。
// 普通业务失败应返回状态，不能借 CHECK 终止程序来代替错误处理。
#define CHECK(expr)                                                          \
  do {                                                                       \
    if (!(expr)) {                                                           \
      std::fprintf(                                                          \
          stderr, "检查失败：%s，位置：%s:%d\n", #expr, __FILE__, __LINE__); \
      std::abort();                                                          \
    }                                                                        \
  } while (false)

#endif  // LEET_CODE_ENGINEERING_COMMON_CHECK_H_
