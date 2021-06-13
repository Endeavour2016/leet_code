/*
 * @Author: zlm 
 * @Date: 2021-03-13 23:44:25 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-03-14 10:57:25
 */

#include "../head_file.h"
using namespace std;

/**
 * @problem: 75. Sort Colors
 * @description:Given an array nums with n objects colored red, white, or blue,
 * sort them in-place so that objects of the same color are adjacent, 
 * with the colors in the order red, white, and blue.
 * We will use the integers 0, 1, and 2 to represent the color red, white, and blue, respectively.
 * 给定一个数组，包含n个对象，颜色为红色、白色和蓝色。将他们就地排序，使相同颜色的对象相邻，
 * 颜色顺序为红色、白色、蓝色。我们将使用整数0、1和2分别表示红、白、蓝。
 * 例子：Input: nums = [2,0,2,1,1,0]
 *       Output: [0,0,1,1,2,2]
 * @method: 本题直接排序即可，但题目要求单趟遍历，且空间复杂度O(1)
 * 方法：从左侧扫描对象，对于蓝色对象，转移到数组最后面，对于红色对象，转移到数组最前面
 * 注：先交换蓝色对象是因为当前蓝色与后面元素交换时可能会把后面的红色交换到前面
 * 遍历终止条件：当前i已经指向数组最后面的蓝色对象，遍历结束
 */
class Solution {
public:
  void sortColors(vector<int>& nums) {
    int traversed_index = -1;  // 已经遍历过的元素下标
    int red_cnt = 0, blue_cnt = 0;  // 已经遍历的对象中，红色和蓝色对象的个数
    int len = nums.size();
    for (int i = 0; i < len;) {
      if (nums[i] == 2) {   // 蓝色对象，转移到数组最后面
        // 此时i已经指向数组最后面的蓝色对象，遍历结束
        if (i == (len-blue_cnt-1) || i == (len-blue_cnt)) {
          break;
        }
        int temp = nums[len - blue_cnt - 1];
        if (temp == nums[i]) {
          ++blue_cnt;
          continue;
        }
        nums[len - blue_cnt - 1] = nums[i];
        nums[i] = temp;
        ++blue_cnt;
      }
      if (nums[i] == 0) {  // 红色对象，转移到数组最前面
        int temp = nums[red_cnt];
        nums[red_cnt] = nums[i];
        nums[i] = temp;
        ++red_cnt;
      }
      ++i;
    }  // end for
  }    // end func
};


int main() {
  Solution solution;
  // vector<int> nums = {2,0,2,1,1,0};
  vector<int> nums = {2,1,2};
  solution.sortColors(nums);
  for (auto i : nums) {
    cout << i << " ";
  }
  cout << endl;

  system ("pause");
  return 0;
}