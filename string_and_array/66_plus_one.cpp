/*
 * @Author: zlm 
 * @Date: 2021-07-17 12:53:30 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-07-17 13:15:30
 */

#include "../head_file.h"
#include "../data_structure.h"
using namespace std;

/**
 * @problem: 66. Plus One
 * @descr: 给一个非空的整型数组来表示一个非负整数（数组中每个元素都只有个位数），要求对该整数+1
 * 比如 [1,2,3]表示整数123，加1后得到124，对应输出数组为[1, 2, 4]
 * @method:
 */

class Solution {
 public:
  vector<int> plusOne(vector<int>& digits) {
    // 数组尾部加+1
    int len = digits.size();
    for (int i = len - 1; i >= 0; --i) {
      if (digits[i] + 1 != 10) {
        digits[i] += 1;
        return digits;
      }
      digits[i] = 0;  // 下次循环需要考虑进位
    }

    // 走到这里，说明第一位也发生了进位
    digits.reserve(len+1);
    digits.insert(digits.begin(), 1);
    return digits;
  }
};

int main() {
  system ("pause");
  return 0;
}