/*
 * @Author: zlm 
 * @Date: 2021-07-17 13:18:49 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-07-17 13:51:34
 */


#include "../head_file.h"
#include "../data_structure.h"
using namespace std;

/**
 * @problem: 28. Implement strStr()
 * @descr: 给一个字符串和子串，要求返回子串第一次出现的下标
 * Input: haystack = "hello", needle = "ll"
 * Output: 2
 * @method: 方法一：暴力匹配
 * 时间复杂度 O(N*M)
 */
bool is_str(int start, const string& str, const string& sub_str) {
  int len = sub_str.size();
  for (int j = 0; j < len; ++j) {
    if (sub_str[j] != str[start + j]) {
      return false;
    }
  }
  return true;
}

int strStr(string haystack, string needle) {
  if (needle.empty()) {
    return 0;
  }
  int len = haystack.size();
  int sub_len = needle.size();

  int index = 0;
  for (; index <= len - sub_len; ++index) {
    if (haystack[index] != needle[0]) {
      continue;
    }
    if (is_str(index, haystack, needle)) {
      return index;
    }
  }
  return -1;
}


int main() {
  string str = "hello";
  string sub_str = "ll";
  int res = strStr(str, sub_str);

  cout << "res: " << res << endl;

  system ("pause");
  return 0;
}