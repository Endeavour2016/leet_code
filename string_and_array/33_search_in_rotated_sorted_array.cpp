/*
 * @Author: zlm 
 * @Date: 2021-03-14 10:59:22 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-03-14 12:20:07
 */

#include "../head_file.h"
using namespace std;

/**
 * @problem: 33. Search in Rotated Sorted Array
 * @descr:Given the array nums after the rotation and an integer target, 
 * return the index of target if it is in nums, or -1 if it is not in nums.
 * 题目大意：给定一个旋转数组（升序数组，从下标k位置开始旋转），从中查找指定的元素
 * For example, [0,1,2, 4,5,6,7] might be rotated at pivot index 3 and become [4,5,6,7,0,1,2]
 * 即nums[k, k+1, ..., n-1, 0, 1, 2, ... k-1]
 * @method: 二分法
 */
class Solution {
public:
    int search(vector<int>& nums, int target) {
      if (nums.size() == 0) {
        return -1;
      }
      int len = nums.size();
      int left = 0, right = nums.size() - 1;
      while (left <= right) {
        int mid = left + (right-left) / 2;
        if (nums[mid] == target) {
          return mid;
        }
        if (nums[mid] >= nums[0]) {  // 此时mid处于左半部分
          if (nums[mid] < target || target < nums[0]) {  //  target在mid的右边
            left = mid + 1;
          } else {
            right = mid - 1;
          }
        } else {   // 此时mid处于右半部分
          if (target < nums[mid] || target > nums[len-1]) {  // target 在mid的左边
            right = mid - 1;
          } else {
            left = mid + 1;
          }
        }
      }  // end while
      return -1;
    }
};


int main() {
  vector<int> nums = {4,5,6,7,0,1,2};
  int target = 3;
  Solution solution;
  int res = solution.search(nums, target);

  cout << "res: " << res << endl;

  system ("pause");
  return 0;
}