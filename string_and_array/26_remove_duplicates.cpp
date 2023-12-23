/*
 * @Author: zlm 
 * @Date: 2022-04-10 15:22:37 
 * @Last Modified by: zlm
 * @Last Modified time: 2022-04-10 15:52:50
 */

#include "../head_file.h"
#include "../data_structure.h"
using namespace std;

/**
 * @problem: 26. 删除有序数组中的重复项
 * @descr: 给你一个 升序排列 的数组 nums，请你 原地 删除重复出现的元素
 * 使每个元素 只出现一次，返回删除后数组的新长度。元素的 相对顺序 应该保持 一致 
 * 由于在某些语言中不能改变数组的长度，所以必须将结果放在数组nums的第一部分
 * 更规范地说，如果在删除重复项之后有 k 个元素，那么 nums 的前 k 个元素应该保存最终结果
 * @method: 
 * 遍历数组元素，每找到一个不相同的元素，记录其应放置的下标pos
 * 直到确定最后一个不同元素的下标即可
 */
class Solution {
public:
  int removeDuplicates(vector<int>& nums) {
    int len = nums.size();
    if (len == 0) {
        return 0;
    }
    int pos = 0;
    // 寻找下一个不同的元素
    for (int i = 1; i <= len - 1; ++i) {
      if (nums[i] == nums[i-1]) {
          continue;
      }
      ++pos;
      nums[pos] = nums[i];
    }
    return pos + 1;
  }
};

// 80. 删除有序数组中的重复项 II
// 给你一个有序数组 nums ，请你原地删除重复出现的元素，使每个元素最多出现两次
// 返回删除后数组的新长度
// 不要使用额外的数组空间，你必须在 原地 修改输入数组 并在使用 O(1) 额外空间的条件下完成
// 遍历数组元素，每次找到一个不相同的元素时，记录从当前元素开始连续最多N个相同元素的位置
// 本题中N为2；直到确定最后一个不同元素的下标即可
class Solution_2 {
public:
    int removeDuplicates(vector<int>& nums) {
        int len = nums.size();
        if (len == 0) {
            return 0;
        }
        int pos = 0;  // 数组中第一个元素不需要移动，下标肯定为0
        int cur_duplicate_cnt = 1;  // 记录当前元素重复出现的个数
        for (int i = 1; i <= len - 1; ++i) {
            if (nums[i] == nums[i-1]) {
                if (cur_duplicate_cnt < 2) {
                    ++cur_duplicate_cnt;
                    ++pos;
                    nums[pos] = nums[i];
                } else {
                    continue;
                }
            } else {  // 走到这里，表示遇到了新的不同元素
                cur_duplicate_cnt = 1;
                ++pos;
                nums[pos] = nums[i];
            }
        }
        return pos + 1;
    }
};

int main() {
  system ("pause");
  return 0;
}
