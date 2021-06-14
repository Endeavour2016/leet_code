/**
 * @Author: zlm 
 * @Date: 2021-06-14 21:12:58 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-06-14 22:28:57
 */

#include "../head_file.h"
using namespace std;

/**
 * @problem: 3. Longest Substring Without Repeating Characters
 * @descr: Given a string s, find the length of the longest substring 
 * without repeating characters.
 * 给一个字符串，找出最长的不含重复字符的子串的长度（子串是连续的字符）
 * @method: 滑动窗口、双指针
 * 1、用两个变量start、end表示不重复子串的起始位置
 * 时间复杂度 O(n), 空间 O(n)
 */

int lengthOfLongestSubstring(string s) {
  int len = s.size();
  int max_length = 0;
  int start = 0, end = 0;
  unordered_map<char, int> char_2_index;
  for (; end < len; ++end) {  // end 先后逐个字符扫描
    char ch = s[end];
    if (char_2_index.find(ch) != char_2_index.end()) {
      // 当期已扫描的连续子串为[start, end-1]
      // 其中跟ch重复的字符的位置记为x，则下一个连续的子串应该从x之后开始计算
      // 因为x之前到end-1位置的子串已经被扫描过，其中连续子串的长度已经记录并更新了max_length
      // 这里取max是因为重复的字符有可能在start之前，之前的字符已经被扫描过
      start = std::max(char_2_index[ch] + 1, start);
    }
    // 记录已扫描字符的位置，同时更新已扫描的连续子串的最大长度
    char_2_index[ch] = end;
    int cur_len = end - start + 1;
    max_length = std::max(cur_len, max_length);
  }
  return max_length;
}


int main() {
  system ("pause");
  return 0;
}