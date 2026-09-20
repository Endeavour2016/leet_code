/*
 * @Author: zlm 
 * @Date: 2024-05-18
 * @Last Modified by: zlm
 * @Last Modified time: 2024-05-18
 */

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

/**
 * @problem: 151. Reverse Words in a String
 * @descr: 给你一个字符串 s ，请你反转字符串中 单词 的顺序。
 * 单词 是由非空格字符组成的字符串。s 中使用至少一个空格将字符串中的 单词 分隔开。
 * 返回 单词 顺序颠倒且 单词 之间用单个空格连接的结果字符串。
 * @method: 利用stringstream：从前向后遍历字符串，提取单词，并插入到 res 的首部，即可实现单词的翻转
 * 时间复杂度O(n), 空间复杂度 O(n)
 */

string reverseWords(string s) {
    stringstream ss(s);
    vector<string> str_vec;
    string tmp;
    while (ss >> tmp) {
      str_vec.emplace_back(std::move(tmp));
    }

    std::reverse(str_vec.begin(), str_vec.end());
    
    // 拼接单词
    std::string res;
    int num = str_vec.size();
    for (int idx = 0; idx < num; ++idx) {
      if (idx == num - 1) {
        res += str_vec[idx];
      } else {
        res += str_vec[idx] + " ";
      }
    }

    return res;
}

// 其它解法：双指针
/*
def reverseWords(self, s: str) -> str:
    s = s.strip()                            # 删除首尾空格
    i = j = len(s) - 1
    res = []
    while i >= 0:
        while i >= 0 and s[i] != ' ': i -= 1 # 搜索首个空格
        res.append(s[i + 1: j + 1])          # 添加单词
        while i >= 0 and s[i] == ' ': i -= 1 # 跳过单词间空格
        j = i                                # j 指向下个单词的尾字符
    return ' '.join(res)                     # 拼接并返回

链接：https://leetcode.cn/problems/reverse-words-in-a-string/solutions/2361551/151-fan-zhuan-zi-fu-chuan-zhong-de-dan-c-yb1r/
*/