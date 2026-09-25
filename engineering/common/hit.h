/**
 * @file hit.h
 * @brief 定义检索结果及统一的排序规则。
 * @details 先按分数降序，同分时按全局文档编号升序，供 Top K 和分片合并共用。
 * @note 调用者须保证文档编号唯一；比较器返回 true 表示左侧结果更好。
 * @see notes/engineering/12_topk_pruning.md
 */

#ifndef LEET_CODE_ENGINEERING_COMMON_HIT_H_
#define LEET_CODE_ENGINEERING_COMMON_HIT_H_

#include <cstdint>

// 一条检索结果：文档编号与评分。
struct Hit {
  std::uint32_t doc;
  std::uint64_t score;
  bool operator==(const Hit& other) const {
    return doc == other.doc && score == other.score;
  }
};

// 统一排序规则：分数降序，同分时全局文档编号升序。
struct BetterHit {
  bool operator()(const Hit& a, const Hit& b) const {
    return a.score != b.score ? a.score > b.score : a.doc < b.doc;
  }
};

#endif  // LEET_CODE_ENGINEERING_COMMON_HIT_H_
