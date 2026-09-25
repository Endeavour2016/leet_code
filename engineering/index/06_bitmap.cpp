/**
 * @file 06_bitmap.cpp
 * @brief 验证位图的成员操作和集合运算。
 * @details 将位图结果与 std::set 对照，覆盖跨整数边界、尾部未使用位和空集合。
 * @note 实现位于 common/bitmap.h；集合运算要求两侧编号范围相同。
 * @see notes/engineering/06_bitmap.md
 * @par 运行方式
 * 在仓库根目录执行：make run eng_bitmap
 */

#include <cstddef>
#include <random>
#include <set>

#include "../common/bitmap.h"
#include "../common/check.h"

// 以下为验证代码；核心实现见 ../common/bitmap.h，main 检查正常和边界情况。
int main() {
  Bitmap empty(0);
  CHECK(empty.count() == 0);
  CHECK(!empty.set(0));
  Bitmap a(130);
  Bitmap b(130);
  std::set<std::size_t> sa;
  std::set<std::size_t> sb;
  std::mt19937 rng(42);
  for (int i = 0; i < 400; ++i) {
    std::size_t x = rng() % 130;
    std::size_t y = rng() % 130;
    CHECK(a.set(x));
    sa.insert(x);
    CHECK(b.set(y));
    sb.insert(y);
  }

  for (auto bit :
       {std::size_t(0), std::size_t(63), std::size_t(64), std::size_t(129)}) {
    CHECK(a.reset(bit));
    sa.erase(bit);
  }

  CHECK(a.count() == sa.size());
  CHECK(b.count() == sb.size());
  Bitmap intersection;
  Bitmap combined;
  Bitmap diff;
  CHECK(a.intersect(b, intersection));
  CHECK(a.unite(b, combined));
  CHECK(a.difference(b, diff));
  for (std::size_t i = 0; i < 130; ++i) {
    bool x = sa.count(i) != 0;
    bool y = sb.count(i) != 0;
    CHECK(intersection.contains(i) == (x && y));
    CHECK(combined.contains(i) == (x || y));
    CHECK(diff.contains(i) == (x && !y));
  }

  const auto count = a.count();
  CHECK(!a.set(130));
  CHECK(!a.reset(130));
  CHECK(!a.contains(130));
  CHECK(a.count() == count);
  Bitmap output(1);
  CHECK(output.set(0));
  CHECK(!a.intersect(Bitmap(129), output));
  CHECK(!a.unite(Bitmap(129), output));
  CHECK(!a.difference(Bitmap(129), output));
  CHECK(output.count() == 1 && output.contains(0));
  CHECK(a.intersect(a, a));
  CHECK(a.count() == count);
}
