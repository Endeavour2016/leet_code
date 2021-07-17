/*
 * @Author: zlm 
 * @Date: 2021-07-17 20:16:34 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-07-17 22:26:13
 */

#include "../head_file.h"
using namespace std;

/**
 * @problem: 316. Remove Duplicate Letters(Medium)
 * @descr: 给你一个字符串 s ，请你去除字符串中重复的字母，使得每个字母只出现一次。
 * 需保证返回结果的字典序最小（要求不能打乱其他字符的相对位置）。
 * @method: stack
 */

class Solution {
public:
    string removeDuplicateLetters(string s) {
        int len = s.length();
        string st;  // 用string对象模拟栈，这样返回结果时不需要做一次栈到string的转换了
        vector<bool> is_appeared(26, false);  // s中只会出现小写字母
        vector<int> last_index_of_char(26, 0);  // 每个字符最后一次出现的位置
        for (int i = 0; i < len; ++i) {
            last_index_of_char[s[i] - 'a'] = i;
        }
  
        // 从前向后遍历字符串s
        for (int i = 0; i < len; ++i) {
            if (is_appeared[s[i] - 'a']) {  // 如果当前栈中已经存在该元素，则跳过
                continue;
            }
            // s[i]入栈前先判断是否剔除栈顶元素
            // 按照字典序栈顶元素 > s[i], 且栈顶元素在i之后的位置也出现过
            while (!st.empty() && st.back() > s[i] &&
                last_index_of_char[st.back() - 'a'] > i) {
                is_appeared[st.back() - 'a'] = false;
                st.pop_back();
            }

            st.push_back(s[i]);
            is_appeared[s[i] - 'a'] = true;
        }
        // 此时栈内元素即为去重后满足最小字典序的子序列
        return st;
    }
};


int main() {
  Solution solution;
  std::string str = "cbacdcbc";
  std::string res = solution.removeDuplicateLetters(str);
  cout << "res: " << res << endl;

  system ("pause");
  return 0;
}
