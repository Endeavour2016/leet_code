/*
 * @Author: zlm 
 * @Date: 2024-06-22
 * @Last Modified by: zlm
 * @Last Modified time: 2024-06-22
 */

#include <iostream>
#include <string>
#include <vector>

using namespace std;


/**
 * @brief: 面试题 01.06. 字符串压缩（easy）
 * 字符串压缩。利用字符重复出现的次数，编写一种方法，实现基本的字符串压缩功能。
 * 比如，字符串aabcccccaaa会变为a2b1c5a3。若“压缩”后的字符串没有变短，则返回原先的字符串。
 * 你可以假设字符串中只包含大小写英文字母（a至z）
 * 示例1:
 * 输入："aabcccccaaa"
 * 输出："a2b1c5a3"
 * @method: 
 */

class Solution {
public:
    string compressString(string str) {
      if (str.size() <= 1) {
        return str;
      }
      std::string res = "";

      int idx = 0;
      while (idx < str.size()) {
        char ch = str[idx];
        int32_t count = 1;
        idx++;
        // 统计重复次数
        while (idx < str.size() && str[idx] == ch) {
          count++;
          ++idx;
        }
        // ch拼接次数
        res += std::string(1, ch) + std::to_string(count);
      }
    
      if (res.size() >= str.size()) {
        return str;
      }
      return res;
    }
};


// 类似题目：443. 压缩字符串

// 给你一个字符数组 chars ，请使用下述算法压缩：
// 从一个空字符串 s 开始。对于 chars 中的每组 连续重复字符 ：
// 如果这一组长度为 1 ，则将字符追加到 s 中。
// 否则，需要向 s 追加字符，后跟这一组的长度。
// 压缩后得到的字符串 s 不应该直接返回 ，需要转储到字符数组 chars 中
// 请在 修改完输入数组后 ，返回该数组的新长度。

// 你必须设计并实现一个只使用常量额外空间的算法来解决此问题。

// 示例 1：
// 输入：chars = ["a","a","b","b","c","c","c"]
// 输出：返回 6 ，输入数组的前 6 个字符应该是：["a","2","b","2","c","3"]
// 解释："aa" 被 "a2" 替代。"bb" 被 "b2" 替代。"ccc" 被 "c3" 替代。
// 示例 2：


int compress(vector<char>& chars) {
  if (chars.size() == 0) {
    return 0;
  }

  int char_nums = chars.size();
  int idx = 0;
  int out_idx = 0;
  while (idx < char_nums) {
    char ch = chars[idx];
    int count = 1;
    idx++;
    // 统计次数
    while (idx < char_nums && ch == chars[idx]) {
      count++;
      idx++;
    }
    // 将 "ch拼接count", 写到原始数组头部
    chars[out_idx++] = ch;
    if (count > 1) {
      std::string temp = std::to_string(count);
      for (const auto& val : temp) {
        chars[out_idx++] = val;
      }
    }
  }

  // chars[0~out_idx-1]即为压缩后的字符串
  return out_idx;
}
