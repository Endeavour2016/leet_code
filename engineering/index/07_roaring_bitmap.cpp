/**
 * @file 07_roaring_bitmap.cpp
 * @brief 实现仅包含数组和位图容器的教学版 Roaring Bitmap。
 * @details 按高 16 位分组，根据每组元素数选择存储方式，并按容器类型计算交集。
 * @note 不实现连续区间容器、删除或序列化，不兼容完整 Roaring 文件格式。
 * @see notes/engineering/07_roaring_bitmap.md
 * @par 运行方式
 * 在仓库根目录执行：make run eng_roaring_bitmap
 */

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <map>
#include <random>
#include <set>
#include <utility>
#include <vector>

#include "../common/check.h"

// 一个容器保存高 16 位相同的编号，内部只记录低 16 位。
class Container {
 public:
  // 数组保持有序且去重，超过 4096 个元素时转换为固定大小位图。
  void add(std::uint16_t value) {
    if (!bits_.empty()) {
      bits_[value / 64] |= std::uint64_t(1) << (value % 64);
      return;
    }
    auto it = std::lower_bound(array_.begin(), array_.end(), value);
    if (it == array_.end() || *it != value) {
      array_.insert(it, value);
    }
    if (array_.size() > 4096) {
      // 将每个低 16 位值映射到对应的位，全部转换完再释放数组。
      std::vector<std::uint64_t> bits(1024, 0);
      for (auto x : array_) {
        bits[x / 64] |= std::uint64_t(1) << (x % 64);
      }
      bits_.swap(bits);
      std::vector<std::uint16_t>().swap(array_);
    }
  }

  bool contains(std::uint16_t value) const {
    return bits_.empty()
               ? std::binary_search(array_.begin(), array_.end(), value)
               : ((bits_[value / 64] >> (value % 64)) & 1) != 0;
  }

  bool dense() const {
    return !bits_.empty();
  }

  // 根据两侧类型分别使用双指针、位查询或按位与。
  Container intersect(const Container& other) const {
    Container result;
    if (!dense() && !other.dense()) {
      std::set_intersection(array_.begin(),
                            array_.end(),
                            other.array_.begin(),
                            other.array_.end(),
                            std::back_inserter(result.array_));
    } else if (!dense() || !other.dense()) {
      const Container& sparse = dense() ? other : *this;
      const Container& bitmap = dense() ? *this : other;
      for (auto x : sparse.array_) {
        if (bitmap.contains(x)) {
          result.array_.push_back(x);
        }
      }
    } else {
      result.bits_.resize(1024);
      std::size_t count = 0;
      for (std::size_t i = 0; i < 1024; ++i) {
        result.bits_[i] = bits_[i] & other.bits_[i];
        auto word = result.bits_[i];
        while (word) {
          word &= word - 1;
          ++count;
        }
      }
      // 交集可能变稀疏，转回数组以减少存储空间。
      if (count <= 4096) {
        for (std::uint32_t x = 0; x < 65536; ++x) {
          if (result.contains(static_cast<std::uint16_t>(x))) {
            result.array_.push_back(static_cast<std::uint16_t>(x));
          }
        }
        std::vector<std::uint64_t>().swap(result.bits_);
      }
    }
    return result;
  }

 private:
  std::vector<std::uint16_t> array_;
  std::vector<std::uint64_t> bits_;
};

// 高 16 位决定容器，低 16 位是容器内部的成员值。
class RoaringBitmap {
 public:
  void add(std::uint32_t doc) {
    buckets_[static_cast<std::uint16_t>(doc >> 16)].add(
        static_cast<std::uint16_t>(doc));
  }

  bool contains(std::uint32_t doc) const {
    auto it = buckets_.find(static_cast<std::uint16_t>(doc >> 16));
    return it != buckets_.end() &&
           it->second.contains(static_cast<std::uint16_t>(doc));
  }

  RoaringBitmap intersect(const RoaringBitmap& other) const {
    RoaringBitmap result;
    for (const auto& pair : buckets_) {
      auto it = other.buckets_.find(pair.first);
      if (it != other.buckets_.end()) {
        result.buckets_[pair.first] = pair.second.intersect(it->second);
      }
    }
    return result;
  }

 private:
  std::map<std::uint16_t, Container> buckets_;
};

// 以下为验证代码；复习时先阅读上方实现，main 用于检查正常和边界情况。
int main() {
  Container c;
  for (unsigned i = 0; i < 4096; ++i) {
    c.add(static_cast<std::uint16_t>(i));
  }

  CHECK(!c.dense());
  c.add(0);
  CHECK(!c.dense());
  c.add(4096);
  CHECK(c.dense());
  Container small;
  small.add(1);
  small.add(65000);
  CHECK(c.intersect(small).contains(1));
  CHECK(!small.intersect(c).contains(65000));
  Container disjoint;
  for (unsigned i = 10000; i < 15000; ++i) {
    disjoint.add(static_cast<std::uint16_t>(i));
  }

  CHECK(!c.intersect(disjoint).dense());
  CHECK(c.intersect(c).dense());
  RoaringBitmap a;
  RoaringBitmap b;
  std::set<std::uint32_t> sa;
  std::set<std::uint32_t> sb;
  for (std::uint32_t i = 0; i < 7000; ++i) {
    a.add(i);
    sa.insert(i);
    b.add(i + 3000);
    sb.insert(i + 3000);
  }

  std::mt19937 rng(7);
  for (int i = 0; i < 1000; ++i) {
    auto x = rng();
    a.add(x);
    sa.insert(x);
    if (i % 2) {
      b.add(x);
      sb.insert(x);
    }
  }

  for (std::uint32_t x : {65535u, 65536u, UINT32_MAX}) {
    a.add(x);
    b.add(x);
    sa.insert(x);
    sb.insert(x);
  }

  auto common = a.intersect(b);
  for (auto x : sa) {
    CHECK(common.contains(x) == (sb.count(x) != 0));
  }

  for (auto x : sb) {
    CHECK(common.contains(x) == (sa.count(x) != 0));
  }

  for (std::uint32_t x = 0; x < 16000; ++x) {
    CHECK(common.contains(x) == (sa.count(x) && sb.count(x)));
  }

  CHECK(!RoaringBitmap().contains(0));
}
