/*
 * @Author: zlm 
 * @Date: 2022-02-06 20:46:31 
 * @Last Modified by:   zlm 
 * @Last Modified time: 2022-02-06 20:46:31 
 */

#include "../head_file.h"
using namespace std;

/**
 * @problem: 153. Find Minimum in Rotated Sorted Array
 * @descr: 求旋转数组中的最小数字.
 * For example, the array nums = [0,1,2,4,5,6,7] might become:
 * [4,5,6,7,0,1,2] if it was rotated 4 times.
 * [0,1,2,4,5,6,7] if it was rotated 7 times.
 * @method: 
 * 方法一：nums是一个有序数组，旋转后分为左右另个子序列，两个子序列均递增且左边的子序列均大于右边的子序列
 * 则最小值一定是右边子序列的第一个值。可以直接从nums[0]开始扫描，直到nums[i] > nums[i+1]终止
 * 此时nums[i+1]即为最小值。
 * 时间复杂度O(n), 空间复杂度为 O(1)
 */

class Solution {
 public:
  int findMin(vector<int>& nums) {
    int len = nums.size();
    if (len == 0) {
      return 0;
    }
    for (int i = 0; i <= len-2; ++i) {
      if (nums[i] > nums[i+1]) {
          return nums[i+1];
      }
      // else 继续比较下一个数字
    }
    // 若for循环中没有return, 说明整个数组均为递增, 则最小值为nums[0]
    return nums[0]; 
  }
};


int main() {
  vector<int> nums = {4,5,6,7,0,1,2};
  int target = 3;
  Solution solution;
  int res = solution.findMin(nums);
  cout << "res: " << res << endl;

  system ("pause");
  return 0;
}