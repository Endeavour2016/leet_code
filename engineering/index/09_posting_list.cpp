/**
 * @file 09_posting_list.cpp
 * @brief 验证倒排列表的集合运算和单向游标。
 * @details 将交集、并集、差集与集合结果对照，并检查游标不会后退。
 * @note 实现位于 common/posting.h；输入必须升序且没有重复编号。
 * @see notes/engineering/09_posting_list.md
 * @par 运行方式
 * 在仓库根目录执行：make run eng_posting_list
 */

#include <cstdint>
#include <random>
#include <set>
#include <vector>

#include "../common/check.h"
#include "../common/posting.h"

// 以下为验证代码；复习时先阅读上方实现，main 用于检查正常和边界情况。
int main() {
  Posting a = {0, 2, 7, UINT32_MAX};
  Posting b = {2, 3, 7};
  CHECK(posting_and(a, b) == Posting({2, 7}));
  CHECK(posting_or(a, b) == Posting({0, 2, 3, 7, UINT32_MAX}));
  CHECK(posting_difference(a, b) == Posting({0, UINT32_MAX}));
  CHECK(posting_and(a, Posting()).empty());
  PostingCursor cursor(a);
  std::uint32_t value = 0;
  CHECK(cursor.advance(1) && cursor.value(value) && value == 2);
  CHECK(cursor.advance(0) && cursor.value(value) &&
        value == 2);  // 目标小于当前位置时，游标也不能后退。
  CHECK(cursor.advance(UINT32_MAX) && cursor.value(value) &&
        value == UINT32_MAX);
  Posting empty;
  PostingCursor exhausted(empty);
  CHECK(!exhausted.advance(0));
  value = 42;
  CHECK(!exhausted.value(value));
  CHECK(value == 42);
  Posting short_list = {1};
  PostingCursor finished(short_list);
  CHECK(!finished.advance(2));
  CHECK(!finished.advance(0));
  CHECK(!finished.value(value));
  CHECK(value == 42);
  std::mt19937 rng(19);
  for (int round = 0; round < 100; ++round) {
    std::set<std::uint32_t> sa;
    std::set<std::uint32_t> sb;
    for (int i = 0; i < 100; ++i) {
      sa.insert(rng() % 200);
      sb.insert(rng() % 200);
    }
    Posting x(sa.begin(), sa.end());
    Posting y(sb.begin(), sb.end());
    Posting both;
    Posting either;
    Posting diff;
    for (std::uint32_t d = 0; d < 200; ++d) {
      if (sa.count(d) && sb.count(d)) {
        both.push_back(d);
      }
      if (sa.count(d) || sb.count(d)) {
        either.push_back(d);
      }
      if (sa.count(d) && !sb.count(d)) {
        diff.push_back(d);
      }
    }
    CHECK(posting_and(x, y) == both);
    CHECK(posting_or(x, y) == either);
    CHECK(posting_difference(x, y) == diff);
  }
}
