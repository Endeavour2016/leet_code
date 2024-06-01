/*
 * @Author: zlm 
 * @Date: 2021-07-17 17:42:02 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-07-17 17:46:11
 */

#include "../head_file.h"
#include "../data_structure.h"
using namespace std;

/**
 * @problem: 53. Maximum Subarray
 * @descr: 输入一个整型数组，数组中的一个或连续多个整数组成一个子数组, 
 * 求所有子数组的和的最大值, 要求时间复杂度为O(n)
 * @method: 
 * 1.从第一个元素开始，记录当前累加和 curSum, 如果curSum>0, 则继续+下一个元素
 * 2.若curSum < 0, 则即使加上下一个元素a[i]，结果肯定小于a[i]，故此时更新curSum = a[i], 继续向后遍历
 */

// 示例 1：
// 输入：nums = [-2, 1, -3, 4, -1, 2, 1, -5, 4]
// 输出：6
// 解释：连续子数组 [4,-1,2,1] 的和最大，为 6 。

class Solution {
public:
  int maxSubArray(vector<int>& nums) {
    if (nums.empty()) {
      return 0;
    }

    int len = nums.size();
    int curSum = 0;
    int maxSum = INT_MIN;

    for (int i = 0; i < len; i++) {
      if (curSum < 0) {
        curSum = nums[i];
      } else {
        curSum += nums[i];  // 记录当前累加和
      }
      if (curSum > maxSum) {
        maxSum = curSum;
      }
    }
    return maxSum;
  }
};

int main() {
  system ("pause");
  return 0;
}
