/*
 * @Author: zlm 
 * @Date: 2024-06-02
 * @Last Modified by:   zlm 
 * @Last Modified time: 2024-06-02
 */

#include <vector>
#include <algorithm>
using namespace std;

/**
 * @brief: 189. Rotate Array
 * 给定一个整数数组 nums，将数组中的元素向右轮转 k 个位置，其中 k 是非负数。
 * 输入: nums = [1,2,3,4, 5,6,7], k = 3
 * 输出: [5,6,7, 1,2,3,4]
 * @method:
 */

// 方法一：使用额外的数组
// 使用额外的数组来将每个元素放至正确的位置。用 n 表示数组的长度
// 遍历原数组，将原数组下标为 i 的元素放至新数组下标为 (i+k) % n 的位置，最后将新数组拷贝至原数组即可
// 时间复杂度：O(n)
// 空间复杂度：O(n)

void rotate(vector<int>& nums, int k) {
  int n = nums.size();
  vector<int> newArr(n);
  for (int i = 0; i < n; ++i) {
    newArr[(i + k) % n] = nums[i];
  }
  nums.assign(newArr.begin(), newArr.end());
}

// 方法二：三次反转
// 数组的元素向右移动 k 次后，尾部 k % n 个元素会移动至数组头部，其余元素向后移动 k%n 个位置。
// 时间复杂度：O(n)
// 空间复杂度：O(1

void rotate2(vector<int>& nums, int k) {
    if (nums.size() <= 1)
        return;
    
    int n = nums.size();
    k = k % n;
    std::reverse(nums.begin(), nums.end());
    std::reverse(nums.begin(), nums.begin() + k);
    std::reverse(nums.begin() + k, nums.end());
}