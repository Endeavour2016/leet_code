/**
 * @Author: zlm 
 * @Date: 2024-05-25 11:00
 * @Last Modified by: zlm
 * @Last Modified time: 2024-05-25 11:00
 */

#include "../head_file.h"
#include <queue>
using namespace std;

/**
 * @brief: 227. 基本计算器 II
 * 给你一个字符串表达式 s ，请你实现一个基本计算器来计算并返回它的值。
 * 整数除法仅保留整数部分。你可以假设给定的表达式总是有效的。
 * 注：本题的表达式中不包含括号
 * @method: 栈
 * 时间复杂度 O(n), 空间 O(n)
 */

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

// 乘法和除法优先级高，因此考虑先进行表达式中的所有乘除运算，把运算结果放在原表达式中的位置。
// 这样表达式中就只剩下正数和负数，类加求和即可
// 具体实现：借助栈
// 遍历string, 将字符串转换成数字，并用变量 operation 表示数字前的符号；
// 若扫描到 +xxx 或者 -xxx，则把+/-xxx作为整体存入到栈中；
// 若扫描到 *xxx 或者 /xxx，由于*/优先级高，因此先和当前栈顶元素运算，再把栈顶元素替换为运算结果
// 最终累加栈中所有元素，即为表达式的值
// ex : s = "3 + 2 * 2"
// 入栈顺序：+3，+2，之后扫描到 '*2'，此时跟栈顶元素运算的到 +2*2 = 4，4入栈并替换原有栈顶
// 即 [+3, 4]，最终结果 = +3+4 = 7

int calculate(string s) {
  int length = s.size();
  if (length == 0) {
    return 0;
  }
  int cur_num = 0;
  char operation = '+';  // 记录已经扫描到某个数字之前的符号，默认第一个数字前都是加号
  stack<int> data_stack;

  for (int i = 0; i < length; ++i) {
    char cur_ch = s[i];
    if (IsDigit(cur_ch)) {
      cur_num = cur_num * 10 + (cur_ch - '0');
    }

    // 扫描到当期数字后面的运算符时，先把
    // 此前扫描的 运算符(operation)和数字(cur_num) 保存起来
    if (!IsDigit(cur_ch) && !IsBlankSpace(cur_ch) || i == length - 1) {
      if (operation == '+') {
        data_stack.push(cur_num);
      } else if (operation == '-') {
        data_stack.push(-cur_num);
      } else if (operation == '*') {
        // 当前的数和栈里面的数字要在此处进行运算
        int stack_pop = data_stack.top();
        data_stack.pop();
        data_stack.push(stack_pop * cur_num);
      } else if (operation == '/') {
        int stack_pop = data_stack.top();
        data_stack.pop();
        data_stack.push(stack_pop / cur_num);
      }
      // 记录当前的运算符，下一次循环时扫描下一个数字
      operation = cur_ch;
      cur_num = 0;
    }
  }   // end for

  // 依次累加栈中的数字，即为最终结果
  int result = 0;
  while (data_stack.size() != 0) {
    result += data_stack.top();
    data_stack.pop();
  }
  return result;
}



// 224. 基本计算器（hard）
// 注：跟227题的主要区别在于本题的表达式带有括号
// 给你一个字符串表达式 s ，请你实现一个基本计算器来计算并返回它的值。

// s 由数字、+'、'-'、'('、')' 和 ' ' 组成
// 输入：s = "(1+(4+5+2)-3)+(6+8)"
// 输出：23

// method：括号展开 + 栈
// 由于字符串除了数字与括号外，只有加号和减号两种运算符。
// 因此，如果展开表达式中所有的括号，则得到的新表达式中，数字本身不会发生变化，只是每个数字前面的符号会发生变化。
// 具体实现：
// 1、用sign表示【当前位置前面的】符号，根据括号表达式的性质，它的取值：
//   1. 与字符串中当前位置的运算符有关；
//   2. 如果当前位置处于一系列括号之内，则也与这些括号前面的运算符有关：每当遇到一个以 − 号开头的括号，
//   则意味着此后的符号都要被「翻转」
// 2、考虑上面第二点，需要维护一个符号栈ops，其中栈顶元素记录了当前位置所处的每个括号所「共同形成」的符号。
// 例如，对于字符串 1 + 2 + (3 - (4 + 5))，当扫描到 '4' 时，当前位置被两个括号所包含，分别对应着 + 号和 − 号，
// + - 合并后的结果为-，因此栈顶元素变为 -1
// 3、在得到栈 ops 之后， sign 的取值就能够确定了：如果当前遇到了 + 号，则更新 sign=ops.top()；
// 如果遇到了遇到了 − 号，则更新 sign = −ops.top()；
// 4、然后每当遇到 '(' 时，把当前的sign存入ops栈；每当遇到 ')' 时，都从栈中弹出一个元素。
// 这样，我们能够在扫描字符串时即时地更新 ops 中的元素。

int calculate_1(string s) {
  stack<int> ops;  // 当前括号所对应的符号
  ops.push(1);     // 初始值为+1
  int sign = 1;

  int ret = 0;
  int length = s.length();
  int i = 0;
  for (int idx = 0; idx < length; ++idx) {
      // 当前字符为数字，需要转换成整数
      if (IsDigit(s[idx])) {
          int num = 0;
          while (idx < length && IsDigit(s[idx])) {
              num = num * 10 + s[idx] - '0';
              idx++;
          }
          // 此时idx指向当前整数的下一个字符，这里需要减1，保证下次for循环从正确的位置继续扫描
          idx--;
          // 因本题只会出现+-符号，因此可以直接累加当前结果（如果其中有*/符号，需要先把数字存入栈中）
          ret += sign * num;

          // test
          std::cout << "idx: " << idx << ", sign: " << sign << ", num: " << num
                    << ", ret: " << ret << endl;
      } else if (s[idx] == '+') {
        // 更新sign的值
        sign = ops.top();
      } else if (s[idx] == '-') {
        sign = -ops.top();
      } else if (s[idx] == '(') {  // 遇到了新括号，需要把当前括号对应的符号存入ops栈
        ops.push(sign);
      } else if (s[idx] == ')') {
        ops.pop();
      }
  }

  return ret;
}

int main() {
  std::string s = " 2-1 + 2 ";
  int ret = calculate_1(s);
  std::cout << "ret: " << ret << std::endl;
  return 0;
}

// 作者：力扣官方题解
// 链接：https://leetcode.cn/problems/basic-calculator/solutions/646369/ji-ben-ji-suan-qi-by-leetcode-solution-jvir/
