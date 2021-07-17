/*
 * @Author: zlm 
 * @Date: 2021-07-17 19:51:21 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-07-17 20:07:31
 */
#include "../head_file.h"
#include "../data_structure.h"
using namespace std;

/**
 * @problem: 242. Valid Anagram(Easy)
 * @descr: 判断两个字符串包含的字符是否完全相同
 * @method: hash/array
 * 利用一个数组统计字符串中每个字符出现的频次，如果相同字符出现频次相同，则两个字符串满足条件
 */
class Solution {
public:
  bool isAnagram(string s, string t) {
    if (s.length() != t.length()) {
      return false;
    }
    vector<int> cnt(26, 0);
    int len = s.length();
    for (int i = 0; i < len; ++i) {
      ++cnt[s[i] - 'a'];
      --cnt[t[i] - 'a'];
    }

    for (int n : cnt) {
      if (n != 0) {
        return false;
      }
    }

    return true;
  }
};

int main() {
  system ("pause");
  return 0;
}