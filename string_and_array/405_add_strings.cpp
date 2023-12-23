/*
 * @Author: zlm 
 * @Date: 2022-02-04 16:48:35 
 * @Last Modified by: zlm
 * @Last Modified time: 2022-02-04 17:33:23
 */
#include "../head_file.h"
#include "../data_structure.h"
using namespace std;

/**
 * @problem: 415. Add Strings
 * @descr: Given two non-negative integers, num1 and num2 represented as string,
 * return the sum of num1 and num2 as a string.
 * @method: 遍历两个字符串，逐个字符相加
 */

class Solution {
public:
  string addStrings(string num1, string num2) {
    if (num1.empty()) {
      return num2;
    }
    if (num2.empty()) {
      return num1;
    }
    int len1 = num1.size();
    int len2 = num2.size();
    int len = len1 < len2 ? len1 : len2;
    int in_num = 0;  // 用于记录进位
    int i = len1 - 1;
    int j = len2 - 1;
    string res;
    while (i >= 0 && j >=0) {
      int d1 = num1[i] - '0';
      int d2 = num2[j] - '0';
      int sum =  d1 + d2 + in_num;
      char cur_digit = sum % 10 + '0';
      in_num = sum / 10;
      res.insert(0, 1, cur_digit);
      --i;
      --j;
    }
    
    while (i >= 0) {
      int d1 = num1[i] - '0';
      int sum = d1 + in_num;
      char cur_digit = sum % 10 + '0';
      in_num = sum / 10;
      res.insert(0, 1, cur_digit);
      --i;
    }
    while (j >= 0) {
      int d2 = num2[j] - '0';
      int sum = d2 + in_num;
      char cur_digit = sum % 10 + '0';
      in_num = sum / 10;
      res.insert(0, 1, cur_digit);
      --j;
    }

    if (in_num > 0) {
      res.insert(0, 1, in_num + '0');
    }
    return res;
  }
};

// 使用 push_back + reverse
// 执行速度更快
string addStrings(string num1, string num2) {
    if (num1.empty()) {
      return num2;
    }
    if (num2.empty()) {
      return num1;
    }
    int len1 = num1.size();
    int len2 = num2.size();
    int len = len1 < len2 ? len1 : len2;
    int in_num = 0;  // 用于记录进位
    int i = len1 - 1;
    int j = len2 - 1;
    string res;
    while (i >= 0 && j >=0) {
      int d1 = num1[i] - '0';
      int d2 = num2[j] - '0';
      int sum =  d1 + d2 + in_num;
      char cur_digit = sum % 10 + '0';
      in_num = sum / 10;
      res.push_back(cur_digit);
      --i;
      --j;
    }
    
    while (i >= 0) {
      int d1 = num1[i] - '0';
      int sum = d1 + in_num;
      char cur_digit = sum % 10 + '0';
      in_num = sum / 10;
      res.push_back(cur_digit);
      --i;
    }
    while (j >= 0) {
      int d2 = num2[j] - '0';
      int sum = d2 + in_num;
      char cur_digit = sum % 10 + '0';
      in_num = sum / 10;
      res.push_back(cur_digit);
      --j;
    }

    if (in_num > 0) {
      res.push_back(in_num + '0');
    }
  
    std::reverse(res.begin(), res.end());
    return res;
  }

int main() {
  system("pause");
  return 0;
}

// Tips
// ctrl + alt + i 自动插入日期信息