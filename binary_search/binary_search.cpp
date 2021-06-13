/*
 * @Author: zlm 
 * @Date: 2021-03-06 11:34:01 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-03-06 11:41:56
 */

#include "../head_file.h"
using namespace std;

/**
 * @problem: 704. Binary Search
 * @description: Given an array of integers nums which is sorted in ascending order, 
 * and an integer target, write a function to search target in nums. 
 * If target exists, then return its index. Otherwise, return -1.
 * @method: 二分查找
 */
class Solution {
public:
  int search(vector<int>& nums, int target) {
    if (nums.size() == 0) {
      return -1;
    }

    int left = 0, right = nums.size() - 1;
    while (left <= right) {
      int mid = left + (right-left) / 2;
      if (nums[mid] == target) {
        return mid;
      } else if (nums[mid] < target) {
        left = mid + 1;
      } else {
        right = mid - 1;
      }
    }
    return -1;
  }
};

int main() {
  Solution solution;
  vector<int> nums = {-1,0,3,5,9,12};
  int target = 18;
  int index = solution.search(nums, target);
  cout << index << endl;

  system ("pause");
  return 0;
}