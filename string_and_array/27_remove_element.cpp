/*
 * @Author: zlm 
 * @Date: 2024-06-01 19:40
 * @Last Modified by: zlm
 * @Last Modified time: 2024-06-01 19:40
 */

#include <vector>
using namespace std:


/** 
 * @brief 27. Remove Element（easy）
 * 删除数组中所有指定的元素，数组是无序的，要求只能就地操作，空间复杂度为 O(1)
 * Example 2:
 * Input: nums = [0,1,2,2,3,0,4,2], val = 2
 * Output: 5, nums = [0,1,4,0,3,_,_,_]
 * 解释：你的函数应该返回 k = 5，并且 nums 中的前五个元素为 0,0,1,3,4。
 * 注意这五个元素可以任意顺序返回。
 * @method: 双指针。将不需要删除的元素均挪动到数组的左侧
 */
class Solution {
 public:
  int removeElement(vector<int>& nums, int val) {
      int n = nums.size();
      if (n == 0) {
          return 0;
      }
      int left = 0;  // 记录当前已扫描到的不需要删除的元素的下标
      for (int idx = 0; idx < n; idx++) {
          if (nums[idx] != val) {        // 没有找到val之前，两个指针都会同时移动
              nums[left++] = nums[idx];  // 把!=val的元素移动到左边
          } else {
            continue;   // 遇到val：left不动，idx继续扫描
          }
      }
      // end for时 !=val 的元素已经全部移动到左边，left指向的是这些元素的下一个位置。因此这些元素的数量=left
      return left;
    }
};

