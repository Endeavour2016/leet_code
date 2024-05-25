/*
 * @Author: zlm 
 * @Date: 2024-05-18
 * @Last Modified by: zlm
 * @Last Modified time: 2024-05-18
 */

#include <iostream>
#include <vector>

using namespace std;

/**
 * @problem: 55. Jump Game
 * @descr: You are given an integer array nums. You are initially positioned at the
 * array's first index, and each element in the array represents your maximum jump length
 * at that position.
 * Return true if you can reach the last index, or false otherwise.
 * @method: 贪心法
 * 时间复杂度O(n), 空间复杂度 O(1)
 */
bool canJump(vector<int>& nums) {
  int num = nums.size();
  if (num == 0 || num == 1) {
      return true;
  }
  // 记录当前可以跳跃的最远位置
  // 假设当前位置为idx，可以跳跃到的最远位置为 cur_distance, 则从 idx+1～cur_distance 之间某个位置起跳后
  // 计算出的最远位置一定得超过cur_distance，这样才能保证有机会跳到最后
  // 否则计算出的最远位置始终 <= cur_distance，则意味着下标idx能够追上 cur_distance 这个位置，这种情况下意味着无法
  // 跳得更远，即跳跃失败，return false
  int cur_distance = 0;
  for (int idx = 0; idx < num; ++idx) {
      cur_distance = std::max(idx + nums[idx], cur_distance);
      if (idx >= cur_distance && cur_distance != num - 1) {
          // 说明 idx～cur_distance之间始终无法找到可以跳的更远的点
          // cur_distance == num-1是个边界case，此时已经到达最后一步了，不能return false
          return false;
      }
  }
  return true;
}