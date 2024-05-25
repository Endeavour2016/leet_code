/*
 * @Author: zlm 
 * @Date: 2024-05-25
 * @Last Modified by: zlm
 * @Last Modified time: 2024-05-25
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * @brief: 42. 接雨水（hard）
 * 给定 n 个非负整数 表示每个宽度为 1 的柱子的高度图，计算按此排列的柱子，下雨之后能接多少雨水。
 * @method: 逐个柱子判断是否能接水，计算当前柱子的接水量，然后累加求和
 * 求每一个柱子的水，我们只需要关注当前柱子，以及左边最高的墙，右边最高的墙就够了。
 * 假设当前柱子的高度记为 cur，左边最高的柱子高度记为left，右边的记为right，有两种情况：
 * 1. cur < left && cur < right，此时可以接住雨水，且雨水的高度为 min(left, right) - cur
 * 2. cur >= left || cur >= right, 此时接不住雨水，因为两边有更矮的柱子，意味着当前柱子的水会流下去
 * 时间复杂度O(n^2), 空间复杂度 O(1)
 */

int GetMaxHeight(int left, int right, const std::vector<int>& height) {
  int max_ht = 0;
  for (int idx = left; idx <= right; ++idx) {
    max_ht = height[idx] > max_ht ? height[idx] : max_ht; 
  }

  return max_ht;
}

// 遍历每个柱子，然后求出left和right，和当前列高度比较
int trap(vector<int>& height) {
  int sum = 0;
  int num = height.size();

  // 两端的柱子不可能接住水，因此只遍历下标 1～n-2 的元素
  for (int idx = 1; idx < num-1; ++idx) {
    int max_left = GetMaxHeight(0, idx-1, height);
    int max_right = GetMaxHeight(idx+1, num-1, height);

    // cur高度小于两边时才能接住雨水，其他情况无法接住
    if (height[idx] < max_left && height[idx] < max_right) {
      int min_ht = std::min(max_left, max_right);
      sum += (min_ht - height[idx]);
    }
  }

  return sum;
}

// 其他方法：动态规划



int main() {
  int a = std::atoi("123");
  return 0;
}