/*
 * @Author: zlm 
 * @Date: 2024-05-18
 * @Last Modified by: zlm
 * @Last Modified time: 2024-05-18
 */

#include <iostream>
#include <vector>

using namespace std;

/**
 * @problem: 58. Length of Last Word
 * @descr: Given a string s consisting of words and spaces, 
 * return the length of the last word in the string.
 * A word is a maximal substring consisting of non-space characters only.
 * @method: 从后向前遍历
 * 时间复杂度O(n), 空间复杂度 O(1)
 */
int lengthOfLastWord(string s) {
  int len = s.size();
  if (len == 0) {
    return 0;
  }

  // 1.从后向前扫描：找到最后一个单词的尾部
  // stl中的方法：str.find_last_not_of(' ')
  // end_idx = s.find_last_not_of(' ');
  int end_idx = len - 1;
  while (end_idx >= 0 && s[end_idx] == ' ') {
    end_idx--;
  }

  // 2.从单词尾部向前扫描，找到单词首部
  int start_idx = end_idx;
  while (start_idx >= 0 && s[start_idx] != ' ') {
    start_idx--;
  }

  return end_idx - start_idx;
}
