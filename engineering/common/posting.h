/**
 * @file posting.h
 * @brief 提供有序文档编号列表、单向游标和集合运算。
 * @details 游标在剩余范围二分查找；交集遍历较短列表，在较长列表中查找匹配编号。
 * @note 输入必须升序且去重；游标使用期间底层列表不能被销毁或修改。
 * @see notes/engineering/09_posting_list.md
 */

#ifndef LEET_CODE_ENGINEERING_COMMON_POSTING_H_
#define LEET_CODE_ENGINEERING_COMMON_POSTING_H_

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <vector>

typedef std::vector<std::uint32_t> Posting;
// 列表必须有序且去重；游标只借用列表，使用期间列表必须一直有效。
class PostingCursor {
 public:
  explicit PostingCursor(const Posting& docs) : docs_(docs), position_(0) {
  }

  // 只搜索当前位置及之后的范围，返回 false 表示没有可读元素。
  bool advance(std::uint32_t target) {
    position_ = static_cast<std::size_t>(
        std::lower_bound(docs_.begin() + position_, docs_.end(), target) -
        docs_.begin());
    return position_ != docs_.size();  // 列表读完后，不会重新开始。
  }

  // 无当前元素时返回 false，并保留 output。
  bool value(std::uint32_t& output) const {
    if (position_ == docs_.size()) {
      return false;
    }
    output = docs_[position_];
    return true;
  }

 private:
  const Posting& docs_;
  std::size_t position_;
};

// 遍历短列表，借助长列表的单向游标寻找交集。
inline Posting posting_and(const Posting& a, const Posting& b) {
  const Posting& small = a.size() <= b.size() ? a : b;
  const Posting& large = a.size() <= b.size() ? b : a;
  Posting result;
  PostingCursor cursor(large);
  for (auto doc : small) {
    if (!cursor.advance(doc)) {
      break;
    }
    std::uint32_t matched = 0;
    if (cursor.value(matched) && matched == doc) {
      result.push_back(doc);
    }
  }

  return result;
}

// 合并两个有序列表，重复编号只输出一次。
inline Posting posting_or(const Posting& a, const Posting& b) {
  Posting result;
  std::set_union(
      a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(result));
  return result;
}

// 保留 a 中存在、b 中不存在的编号。
inline Posting posting_difference(const Posting& a, const Posting& b) {
  Posting result;
  std::set_difference(
      a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(result));
  return result;
}

#endif  // LEET_CODE_ENGINEERING_COMMON_POSTING_H_
