/*
 * @Author: zlm 
 * @Date: 2021-07-10 13:44:15 
 * @Last Modified by: zlm
 * @Last Modified time: 2024-06-15 10:00
 */

#include "../head_file.h"
#include "../data_structure.h"
using namespace std;

/**
 * @brief: 20. Valid Parentheses 有效的括号
 * Given a string s containing just the characters '(', ')', '{', '}', '[' and ']', 
 * determine if the input string is valid.
 * @method: 使用栈。对于s中的字符，从左到右扫描，如果是左括号，则入栈；若遇到右括号，不入栈，只将其跟栈顶字符匹配；
 * 若相同，则继续扫描下一个字符；否则，不匹配，直接 return false;
 * 时间复杂度: O(n)
 * 空间复杂度: O(n)
 */

bool IsLeftBracket(char ch) {
  return ch == '(' || ch == '[' || ch == '{';
}

bool isValid(string s) {
    if (s.empty()) {
        return true;
    }
    stack<char> st;
    for (auto ch : s) {
        if (IsLeftBracket(ch)) {  // 左括号入栈
            st.push(ch);
        } else {
            if (st.empty()) {  // 说明此前没有扫描到对应的左括号，或者扫描到的左括号已经被右括号抵消了
               return false;
            }
            char top_ch = st.top();
            st.pop();
            if (top_ch == '(' && ch != ')' || top_ch == '[' && ch != ']' || top_ch == '{' && ch != '}') {
                return false;
            }
        }
    }
    return st.empty();
}
