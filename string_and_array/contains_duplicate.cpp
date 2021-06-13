/*
 * @Author: zlm 
 * @Date: 2021-03-13 23:03:46 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-03-13 23:45:33
 */

#include "../head_file.h"
using namespace std;

/**
 * @problem: 217. Contains Duplicate
 * @description:Given an integer array nums, return true if any value appears at least twice 
 * in the array, and return false if every element is distinct.
 * 判断数组中是否包含重复的数字，有则返回true
 * @method: 先排序，然后遍历数组，检查相邻元素是否相同 O(logN) + O(N)
 * @result: Runtime: 20 ms, faster than 95.13% of C++ online submissions for Contains Duplicate.
 */
class Solution {
public:
    bool containsDuplicate(vector<int>& nums) {
        if (nums.empty()) {
          return false;
        }
        std::sort(nums.begin(), nums.end());
        int len = nums.size();
        for (int i = 0; i <= len - 2; ++i) {
          if (nums[i] == nums[i+1]) {
            return true;
          }
        }
        return false;
    }
};

int main() {
  Solution solution;
  vector<int> nums = {1,2,3,4};
  bool res = solution.containsDuplicate(nums);
  cout << "res:" << res << endl;

  system ("pause");
  return 0;
}