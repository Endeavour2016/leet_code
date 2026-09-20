/*
 * @Author: zlm 
 * @Date: 2021-07-25 10:33:49 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-07-25 11:02:41
 */


#include "../head_file.h"
#include "../data_structure.h"
using namespace std;

/**
 * @problem: 387. First Unique Character in a String(Easy)
 * @descr: 给一个字符串，找到一个没有重复出现过的字符，并返回其下标
 * Input: s = "leetcode"
 * Output: 0 (l、t、c等都是非重复出现的字符，但是l排在第一个，所以返回其下标0)
 * @method:
 */

// 方法一：使用map记录各个字符的出现次数，然后遍历字符串，遇到第一个出现次数为1的字符，返回结果
// 空间复杂度 O(n), 时间复杂度 O(nlogn)
class Solution {
public:
  int firstUniqChar(string s) {
    if (s.empty()) {
      return -1;
    }

    std::unordered_map<char, int> counter;
    int len = s.size();
    for (int i = 0; i < len; ++i) {
      if (counter.find(s[i]) == counter.end()) {
        counter[s[i]] = 1;
      } else {
        counter[s[i]] += 1;
      }
    }

    for (int i = 0; i < len; ++i) {
      if (counter[s[i]] == 1) {
        return i;
      }
    }
    return -1;
  }
};


int main() {
  system ("pause");
  return 0;
}
