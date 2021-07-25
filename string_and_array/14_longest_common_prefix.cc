/*
 * @Author: zlm 
 * @Date: 2021-07-23 23:12:27 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-07-24 00:50:00
 */

#include "../head_file.h"
#include "../data_structure.h"
using namespace std;

/**
 * @problem: 14. Longest Common Prefix(Easy)
 * @descr: 给一个字符串数组，求出其中所有字符串的最长公共前缀
 * @method:
 * 方法一：以第一个字符串作为公共前缀，与第二字符串求公共前缀，得到的结果再与第三个字符串求公共前缀
 * 依次处理后续各个字符串，最终得到的即为所有字符串的公共前缀。如果处理某个字符串时，发现公共前缀变为
 * 空，则说明当前已处理的字符串不存在公共前缀，此时直接return空值
 * 时间复杂度:O(N) N为所有字符串的长度和
 */
class Solution {
public:
  string longestCommonPrefix(vector<string>& strs) {
    if (strs.size() == 0) {
        return "";
    }
    int num = strs.size();
    string prefix = strs[0];
    for (int i = 1; i < num; ++i) {
      int len = min(prefix.size(), strs[i].size());
      int index = 0;
      // 计算prefix与当前str[i]的公共前缀
      for (; index < len; ++index) {
        if (prefix[index] == strs[i][index]) {
          continue;
        } else {
          break;
        }
      }
      prefix = strs[i].substr(0, index);
      if (prefix.size() == 0) {
        break;
      }
    }
    return prefix;
  }
};

int main() {
  system ("pause");
  return 0;
}
