/**
 * @file 08_bloom_filter.cpp
 * @brief 实现只插入、不删除的布隆过滤器。
 * @details 根据预计元素数和误判率计算大小，再用两个哈希值生成多个位位置。
 * @note 查询为真时仍需精确查找；示例不支持并发插入，也不使用加密哈希。
 * @see notes/engineering/08_bloom_filter.md
 * @par 运行方式
 * 在仓库根目录执行：make run eng_bloom_filter
 */

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

#include "../common/check.h"

// 查询成功后，结果为 false 表示不存在，为 true 表示仍需精确查找。
class BloomFilter {
 public:
  BloomFilter(std::size_t expected, double false_positive_rate)
      : size_(0), hashes_(0) {
    if (!expected || !(false_positive_rate > 0 && false_positive_rate < 1)) {
      return;
    }
    const double ln2 = std::log(2.0);
    const double bits = std::ceil(-static_cast<double>(expected) *
                                  std::log(false_positive_rate) / (ln2 * ln2));
    // 限制教学示例的分配大小，也保证后续整数转换不越界。
    if (bits > 100000000.0) {
      return;
    }
    size_ = static_cast<std::size_t>(bits);
    hashes_ = static_cast<std::size_t>(
        std::max(1.0, std::round(bits / expected * ln2)));
    words_.assign(size_ / 64 + (size_ % 64 != 0), 0);
  }

  // 参数无效或超过教学规模限制时返回 false。
  bool valid() const {
    return size_ != 0;
  }

  // 只将位设为 1；不能直接清位删除，否则可能误删其他元素的信息。
  bool add(std::uint64_t key) {
    if (!valid()) {
      return false;
    }
    visit(key, [this](std::size_t bit) {
      words_[bit / 64] |= std::uint64_t(1) << (bit % 64);
      return true;
    });
    return true;
  }

  // 返回值表示操作成功；output 表示可能存在。失败时不修改 output。
  bool may_contain(std::uint64_t key, bool& output) const {
    if (!valid()) {
      return false;
    }
    output = visit(key, [this](std::size_t bit) {
      return ((words_[bit / 64] >> (bit % 64)) & 1) != 0;
    });
    return true;
  }

 private:
  static std::uint64_t mix(std::uint64_t x) {
    x ^= x >> 30;
    x *= UINT64_C(0xbf58476d1ce4e5b9);
    x ^= x >> 27;
    x *= UINT64_C(0x94d049bb133111eb);
    return x ^ (x >> 31);
  }

  // 插入与查询共用位置计算方式，避免同一 key 对应不同的位。
  template <class F>
  bool visit(std::uint64_t key, F f) const {
    const auto h1 = mix(key);
    const auto h2 = mix(key ^ UINT64_C(0x9e3779b97f4a7c15)) | 1;
    for (std::size_t i = 0; i < hashes_; ++i) {
      if (!f((h1 + i * h2) % size_)) {
        return false;
      }
    }
    return true;
  }

  std::size_t size_;
  std::size_t hashes_;
  std::vector<std::uint64_t> words_;
};

// 以下为验证代码；复习时先阅读上方实现，main 用于检查正常和边界情况。
int main() {
  CHECK(!BloomFilter(0, 0.01).valid());
  CHECK(!BloomFilter(10, 1.0).valid());
  CHECK(!BloomFilter(10, 0.0).valid());
  CHECK(!BloomFilter(10, std::numeric_limits<double>::quiet_NaN()).valid());
  CHECK(!BloomFilter(100000000, 0.01).valid());
  BloomFilter invalid(0, 0.01);
  bool result = true;
  CHECK(!invalid.add(1));
  CHECK(!invalid.may_contain(1, result));
  CHECK(result);  // 失败时保持输出，不能把无效过滤器误当作“确定不存在”。
  BloomFilter filter(10000, 0.01);
  CHECK(filter.valid());
  for (std::uint64_t i = 0; i < 10000; ++i) {
    CHECK(filter.add(i));
  }

  for (std::uint64_t i = 0; i < 10000; ++i) {
    CHECK(filter.may_contain(i, result));
    CHECK(result);
  }

  CHECK(filter.add(0));
  CHECK(filter.may_contain(0, result));
  CHECK(result);
  std::mt19937_64 rng(42);
  std::size_t positives = 0;
  for (int i = 0; i < 100000; ++i) {
    auto absent = rng() | (UINT64_C(1) << 63);
    CHECK(filter.may_contain(absent, result));
    positives += result;
  }

  std::cout << "observed false positive rate: " << positives / 100000.0 << '\n';
  // 只记录实测误判率，不要求它恰好等于理论估算值。
}
