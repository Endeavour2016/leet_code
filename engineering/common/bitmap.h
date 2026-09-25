/**
 * @file bitmap.h
 * @brief 提供基于 uint64_t 数组的动态分配位图。
 * @details 文档编号定位数组元素和二进制位，集合运算按数组元素执行。
 * @note 构造后编号范围固定；不支持并发修改；修改失败返回 false，且保持输出不变。
 * @see notes/engineering/06_bitmap.md
 */

#ifndef LEET_CODE_ENGINEERING_COMMON_BITMAP_H_
#define LEET_CODE_ENGINEERING_COMMON_BITMAP_H_

#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>

// 编号范围为 [0, size)，每个文档占一个二进制位，空范围也合法。
class Bitmap {
 public:
  explicit Bitmap(std::size_t size = 0)
      : size_(size), words_(size / 64 + (size % 64 != 0), 0) {
  }

  // 越界返回 false，不修改原有位图。
  bool set(std::size_t bit) {
    if (bit >= size_) {
      return false;
    }
    words_[bit / 64] |= std::uint64_t(1) << (bit % 64);
    return true;
  }

  // 清除对应位；越界返回 false，即使该位原本为 0 也可成功清除。
  bool reset(std::size_t bit) {
    if (bit >= size_) {
      return false;
    }
    words_[bit / 64] &= ~(std::uint64_t(1) << (bit % 64));
    return true;
  }

  // 范围外的编号不属于本集合，直接返回 false。
  bool contains(std::size_t bit) const {
    return bit < size_ && ((words_[bit / 64] >> (bit % 64)) & 1) != 0;
  }

  // 每次去掉最低的一个 1，累加得到成员数。
  std::size_t count() const {
    std::size_t result = 0;
    for (auto word : words_) {
      while (word) {
        word &= word - 1;
        ++result;
      }
    }
    return result;
  }

  // 范围不同时返回 false，并保持 output 不变；允许 output 与输入是同一个对象。
  bool intersect(const Bitmap& other, Bitmap& output) const {
    return combine(other, '&', output);
  }

  bool unite(const Bitmap& other, Bitmap& output) const {
    return combine(other, '|', output);
  }

  bool difference(const Bitmap& other, Bitmap& output) const {
    return combine(other, '-', output);
  }

 private:
  // 先构建临时结果，成功后再更新输出，避免失败时留下部分结果。
  bool combine(const Bitmap& other, char op, Bitmap& output) const {
    if (size_ != other.size_) {
      return false;
    }
    Bitmap result(size_);
    for (std::size_t i = 0; i < words_.size(); ++i) {
      if (op == '&') {
        result.words_[i] = words_[i] & other.words_[i];
      } else if (op == '|') {
        result.words_[i] = words_[i] | other.words_[i];
      } else {
        result.words_[i] = words_[i] & ~other.words_[i];
      }
    }
    output = std::move(result);
    return true;
  }

  std::size_t size_;
  std::vector<std::uint64_t> words_;
};

#endif  // LEET_CODE_ENGINEERING_COMMON_BITMAP_H_
