/**
 * @Author: zlm 
 * @Date: 2024-01-14
 * @Last Modified by: zlm
 * @Last Modified time: 2024-01-14
 */

#include "../head_file.h"
using namespace std;

/**
 * @problem: 392. Is Subsequence
 * @descr: Given two strings s and t, return true if s is a subsequence of t, or false otherwise.
 * A subsequence of a string is a new string that is formed from the original string 
 * by deleting some (can be none) of the characters without disturbing the relative
 * positions of the remaining characters.
 * (i.e., "ace" is a subsequence of "abcde" while "aec" is not).
 * @method: 双指针法：两个字符串从头部开始逐个比较，当前字符相同时两个指针均向后移动，
 * 不相同时只移动原始数组对应的指针
 * 时间复杂度：O(n+m)
 */

// 判断s是否为t的子序列
bool isSubsequence(string s, string t) {
    if (s.size() > t.size()) {
        return false;
    }
    if (s.empty()) {
        return true;
    }

    for (int i = 0, j = 0; i < s.size() && j < t.size();) {
        if (s[i] == t[j]) {
            ++i;
            ++j;
            if (i == s.size()) {  // 说明s已经扫描到最后一个元素且均匹配t中的字符
                return true;
            }
        } else {
            ++j;
        }
    }
    return false;
}

int main() {
  return 0;
}


