/*
 * @Author: zlm 
 * @Date: 2022-02-05 16:38:03 
 * @Last Modified by: zlm
 * @Last Modified time: 2022-02-05 20:47:18
 */
#include "../head_file.h"
#include "../data_structure.h"
using namespace std;


/**
 * @problem: 8. String to Integer (atoi)
 * @descr: 请你来实现一个 myAtoi(string s) 函数，
 * 使其能将字符串转换成一个 32 位有符号整数（类似 C/C++ 中的 atoi 函数）
 * @method: 扫描字符，逐个处理。属于细节题，需要充分考虑各种约束条件
 */
class Solution {
public:
    int myAtoi(string s) {
        int str_size = s.size();
        if (str_size == 0) {
            return 0;
        }
        char sign = '+';
        int first_digit_index = 0;  // 记录第一个数字字符
        // 1、先跳过空格符
        while (first_digit_index < str_size) {
            if (s[first_digit_index] != ' ') {
                break;
            }
            ++first_digit_index;
        }
        // 2、异常情况处理
        if (first_digit_index == str_size) {
            return 0;
        }

        // 3、检查正负号
        if (s[first_digit_index] == '-' || s[first_digit_index] == '+') {
            sign = s[first_digit_index++];
        }
        // 4、此时first_digit_inde应该为第一个有效数字
        bool is_digit = IsDigit(s[first_digit_index]);
        if (!is_digit) {
            return 0;
        }

        // 5、开始处理数字
        int num = 0;
        int idx = first_digit_index;
        while (idx < str_size) {
            int cur_digit = s[idx] - '0';
            // 判断是否溢出
            // num * 10 + cur_digit > INT_MAX;
            if (num > (INT_MAX - cur_digit) / 10) {
                return sign == '+' ? INT_MAX : INT_MIN;
            }
            num = num * 10 + cur_digit;
            if (!IsDigit(s[idx + 1])) {
                break;
            }
            ++idx;
        }
        return sign == '+' ? num : -num;
    }

    bool IsDigit(char ch) {
        return (ch >= '0' && ch <= '9');
    }
};

int main() {
  system("pause");
  return 0;
}

