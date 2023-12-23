/*
 * @Author: zlm 
 * @Date: 2021-02-27 20:46:20 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-02-27 21:12:20
 */

#include <iostream>
using namespace std;

/**
 * 190. Reverse Bits
 * @description: Reverse bits of a given 32 bits unsigned integer.
 * @method: 要求反转 32 位的⼆进制位, 比如 10110->01101
 * 把 num 往右移动，不断的消灭右边最低位的 1，将这个 1 给 res， res 不断的左移即可实现反转⼆
 * 进制位的⽬的。
 */

class Solution {
public:
  uint32_t reverseBits(uint32_t n) {
    uint32_t result = 0;
    for (int i = 0; i < 32; i++) {
      result = result << 1 | (n & 1);
      n >>= 1;
    }
    return result;
  }
};

int main() {
  Solution solution;
}