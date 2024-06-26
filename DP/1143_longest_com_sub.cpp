/*
 * @Author: zlm 
 * @Date: 2024-03-09
 * @Last Modified by: zlm
 * @Last Modified time: 2024-03-09
 */

#include <iostream>
#include <vector>
#include "../data_structure.h"

using namespace std;

/**
 * @problem: 1143. Longest Common Subsequence
 * @descr: Given two strings text1 and text2, return the length of their longest common subsequence.
 * If there is no common subsequence, return 0.
 * For example, "ace" is a subsequence of "abcde".
 * @method: 动态规划
 * 时间复杂度O(n^2), 空间复杂度 O(n^2)
 **/
class Solution {
public:
  int longestCommonSubsequence(string text1, string text2) {
    int len1 = text1.size();
    int len2 = text2.size();
    // dp[len1][len1] 状态定义
    // dp[i][j]表示第一个字符串中的子串text1[1~i]，跟第二个字符串的子串text2[1-j] 之间的最长公共子序列长度
    // 注：为了方便状态转移的代码实现，上述字符串的下标从1开始表示
    vector<vector<int> > sub_len(len1 + 1, vector<int>(len2 + 1, 0));
    for (int i = 1; i <= len1; ++i) {
      for (int j = 1; j <= len2; ++j) {
        if (text1[i-1] == text2[j-1]) {
          sub_len[i][j] = sub_len[i-1][j-1] + 1;
        } else {
          sub_len[i][j] = std::max(sub_len[i-1][j], sub_len[i][j-1]);
        }
      }
    }
  
    for (auto& vec : sub_len) {
      cout << endl;
      for (auto val : vec) {
        cout << val << ", ";
      }
      cout << endl;
    }
    return sub_len[len1][len2];
  }

private:
  // 单个字符是否存在于字符串中
  int IsInString(const char& ch, const std::string& str, int start_pos = 0) const {
    int pos = str.find(ch, start_pos);
    if (pos == std::string::npos) {
      return str.size();
    } else {
      return pos;
    }
  }
  
};


// nowcoder BM66 最长公共子串
// 注：子串时连续的
// 输入： "1AB2345CD","12345EF"
// 返回值："2345"
string LCS(string str1, string str2) {
  // dp[i][j]表示在str1中以第i个字符结尾，在str2中以第j个字符结尾时的公共子串长度
  // 即表示 str1[0~i-1] 和 str2[0～j-1] 之间的公共子串长度
  // 且公共子串是以str[i-1]为结尾字符（str1[i-1] == str2[j-1])
  vector<vector<int> > dp(str1.length() + 1, vector<int>(str2.length() + 1, 0));
  int max = 0;
  int pos = 0;
  for (int i = 1; i <= str1.length(); i++) {
      for (int j = 1; j <= str2.length(); j++) {
          // 如果当前位置两个字符相等：
          if (str1[i - 1] == str2[j - 1]) {
              // 则增加长度
              dp[i][j] = dp[i - 1][j - 1] + 1;
          } else {
              // 该位置为0: 因为这是子串，必须连续相等，断开要重新开始。
              dp[i][j] = 0;
          }
          //更新最大长度
          if (dp[i][j] > max) {
              max = dp[i][j];
              pos = i - 1;  // 记录当前公共子串的最后一个字符的下标
          }
      }
  }
  return str1.substr(pos - max + 1, max);
}

int main() {
  Solution solution;
  string text1 = "abcde";
  string text2 = "ace";

  int res = solution.longestCommonSubsequence(text1, text2);

  cout << "res: " << res << endl;
  return 0;
}