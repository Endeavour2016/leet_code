#include <iostream>
#include <stack>
#include <vector>
#include <string>
using namespace std;

/** 227. Basic Calculator II
 * @author: zlm
 * @date: 2021-02-21
 * @description: Given a string s which represents an expression,
 * evaluate this expression and return its value. 
 * Input: s = "3+2*2"
 * Output: 7
 */

// 思路：遍历string, 字符转换成数字，根据运算符优先级决定是否立刻进行运算
// 如果当期数字不能立刻运算，则先用stack保存数字
class Solution {
public:
  int calculate(string s) {
    int length = s.size();
    if (length == 0) {
      return 0;
    }
    int cur_num = 0;
    char operation = '+';
    stack<int> data_stack;

    for (int i = 0; i < length; ++i) {
      char cur_ch = s[i];
      if (IsDigit(cur_ch)) {
        cur_num = cur_num * 10 + (cur_ch - '0');
      }

      // 扫描到当前数字后面的运算符时，先把
      // 此前扫描的数字(cur_num)和运算符(operation)保存起来
      if (!IsDigit(cur_ch) && !IsBlankSpace(cur_ch) || i == length - 1) {
        if (operation == '+') {
          data_stack.push(cur_num);
        } else if (operation == '-') {
          data_stack.push(-cur_num);
        } else if (operation == '*') {
          // 当期的数和栈里面的数字要在此处进行运算
          int stack_pop = data_stack.top();
          data_stack.pop();
          data_stack.push(stack_pop * cur_num);
        } else if (operation == '/') {
          int stack_pop = data_stack.top();
          data_stack.pop();
          data_stack.push(stack_pop / cur_num);
        }
        // 记录当前的运算符
        operation = cur_ch;
        cur_num = 0;
      }
    }  // end for
  
    // 依次累加栈中的数字，即为最终结果
    int result = 0;
    while (data_stack.size() != 0) {
      result += data_stack.top();
      data_stack.pop();
    }
    return result;
  }      

private:
  bool IsDigit(char ch) {
    if (ch >= '0' && ch <= '9') {
      return true;
    }
    return false;
  }

  bool IsBlankSpace(char ch) {
    if (ch == ' ') {
      return true;
    }
    return false;
  }
};

int main() {
  string s = " 3/2 ";
  Solution solution;
  int result = solution.calculate(s);
  cout << result << endl;

  system("pause");
  return 0;
}