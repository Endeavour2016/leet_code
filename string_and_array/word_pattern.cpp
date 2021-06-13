#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <string>
using namespace std;

/** 290. Word Pattern
 * @author: zlm
 * @date: 2021-02-21
 * @description: Given a pattern and a string s, find if s follows the same pattern.
 * Here follow means a full match, such that there is a 
 * bijection between a letter in pattern and a non-empty word in s.
 */

 // 题目大意：pattern中的字符和 s 中的单词满足一一映射 
 // 思路：s按照空格分割成多个字符串，跟pattern中的字符建立映射关系（借助map实现）

class Solution {
public:
  bool wordPattern(string pattern, string s) {
    if (pattern.empty() || s.empty()) {
      return false;
    }
    vector<string> str_vec;
    StringSplit(s, &str_vec);
    if (str_vec.size() == 0 || str_vec.size() != pattern.size()) {
      return false;
    }
    
    map<char, string> char_2_string;
    // 建立 ch->str 映射的过程中记录已经出现的str
    // 不同的ch不能映射到同一个str
    set<string> existed_str;
    for (int i = 0; i < pattern.size(); ++i) {
      char ch = pattern[i];
      if (char_2_string.find(ch) == char_2_string.end()) {
        char_2_string[ch] = str_vec[i];
        if (existed_str.find(str_vec[i]) != existed_str.end()) {
          return false;
        }
        existed_str.insert(str_vec[i]);
      } else {  // map中已经存在当前ch，检查map中的str与当前str_vec[i]是否匹配
        const auto& mapped_str = char_2_string[ch];
        if (mapped_str != str_vec[i]) {
          return false;
        }
      }
    }
    return true;
  }

private:
  void StringSplit(const string& str, vector<string>* out) {
    istringstream in_stream(str);
    string temp;
    while (in_stream >> temp) {
      out->push_back(temp);
    }
  }
};

// ================ main function =====================
int main(int argc, char* argv[]) {
  Solution solution;
  string pattern = "abba";
  string str = "dog dog dog dog";
  bool res = solution.wordPattern(pattern, str);
  cout << res << endl;
  return 0;
}