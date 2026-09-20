/*
 * @Author: zlm 
 * @Date: 2022-02-06 20:46:35 
 * @Last Modified by: zlm
 * @Last Modified time: 2022-02-06 21:28:32
 */

#include "../head_file.h"
using namespace std;

/**
 * @problem: 5. Longest Palindromic Substring
 * @descr: 给你一个字符串 s，找到 s 中最长的回文子串
 * @method: 动态规划
 * 明确状态：某个子串是否为回文子串
 * 定义dp数组，明确其含义： dp[i][j] 表示字符串区间[i, j] 是否为回文串
 * 确定状态转移方程：
 * 当i=j时，只有一个字符，肯定是回文串：即dp[i][i] = true 
 * 若i+1=j，说明俩字符相邻，则dp[i][j] == (s[i]==s[j])
 */
class Solution {
 public:
  string longestPalindrome(string s) {
    int len = s.size();
    if (len == 0) {
      return "";
    }
    int max_len = 0;
    int start_index = 0;

    bool dp[len][len];
    // 逐个填充dp数组元素
    for (int i = 0; i < len; ++i) {
      for (int j = 0; j <= i; ++j) {
        if (i - j <= 1) {  // 同一个字符或者相邻字符的情况
          dp[j][i] = (s[i] == s[j]);
        } else {  // 状态转移方程
          dp[j][i] = (s[j] == s[i] && dp[j+1][i-1]);  
        }
        // 更新子串最大长度
        if (dp[j][i] && max_len < i -j + 1) {
          max_len = i - j + 1;
          start_index = j;
        }
      }
    }
    return s.substr(start_index, max_len);
  }
};

// 更优的方法：扩展回文子串的方法
// 对于s中的每个字符s[i],以其为中心向左右扩展，计算出扩展出的回文子串长度
// 遍历一次字符串，计算出每个字符扩展后的回文子串，取其最大值即可
// 时间复杂度为O(n^2)，空间为O(1)
// 注：回文串的长度可能是奇数也可能是偶数, 比如acca这种情况, 只通过一个字符作为中心无法扩展出回文子串
// 因此还需要考虑以两个字符作为中心，向左右扩展以找出回文子串

// 扩展回文子串的方法
int ExpandSubstring(const string& s, int left, int right) {
    while (left >= 0 && right < s.length() && s[left] == s[right]) {
        --left;
        ++right;
    }
    return right - left - 1;  // 退出循环时，s[left]！=s[right]，或者到达左右边界
}

string longestPalindrome(string s) {
    int len = s.size();
    if (len == 0) {
      return "";
    }

    int max_len = 0;
    int start = 0, end = 0;  // 子串的起点和终点下标

    for (int i = 0; i < len; i++) {
        int len1 = ExpandSubstring(s, i, i); // 扩展子串的字符个数为奇数
        int len2 = ExpandSubstring(s, i, i+1); // 以s[i]和s[i+1]为中心进行扩展，字符个数为偶数
        int cur_len = max(len1, len2);
        
        if (cur_len >= max_len) {
            max_len = cur_len;
            // 由于start+end = 2i, end - start = curLen-1
            // 故可以计算出start
            start = i - (cur_len - 1) / 2;
            end = i + (cur_len - 1) / 2;
        }

    }
    return s.substr(start, max_len);
}


int main() {
  Solution solution;

  system ("pause");
  return 0;
}