/*
 * @Author: zlm 
 * @Date: 2024-06-15 23:00
 * @Last Modified by: zlm
 * @Last Modified time: 2024-06-15 23:00
 */

#include "../head_file.h"
#include "../data_structure.h"
using namespace std;

/**
 * @brief: 69. x 的平方根 
 * 给你一个非负整数 `x` ，计算并返回 `x` 的 **算术平方根** 。
 * @method: 二分查找法
 * 假设sqrt(x) = k, 即k^2 = x, 若k取整数，则 k^2 <= x
 * 若x > 1, 则显然 k < x。因此可以在 0～x之间二分查找k
 */

class Solution {
 public:
  int mySqrt(int x) {
    if (x <= 1)
      return x;

    int low = 1, high = x;
    int mid;
    while (low <= high) {
      // mid = (low + high) / 2; // 这样写容易出现整数溢出
      mid = low + (high - low)/2;
                
      if (x/mid == mid)
        return mid;
      else if (x / mid > mid) {  // 使用mid*mid 与x进行判断会溢出int数据范围
        low = mid + 1;
      } else {
        high = mid - 1;
      }
    }
    return high; // 若循环内没有找到，则返回low和high中较小的那个
  }
};


