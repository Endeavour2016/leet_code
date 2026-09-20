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


/**
 * @problem: 45. Jump GameII (求最少跳跃次数)
 * @descr: 给定一个长度为 n 的 整数数组 nums，初始位置为 nums[0]。
 * 每个元素 nums[i] 表示从索引 i 向前跳转的最大长度。
 * 返回到达 nums[n - 1] 的最小跳跃次数。
 * @method: 贪心策略：每一次跳跃的落点应该使得下一次跳跃到达的位置最远
 * 时间复杂度O(n), 空间复杂度 O(1)
 */

// 1. 假设当前位置为 i，则一次可跳跃的最远位置为 i+A[i], 则在该区间[i, i+A[i]]内的任意位置均可通过一次跳跃达到；
// 2. 假设落点为 x，则应该使得 x+A[x]最大，因此下一个落点的区间为 [x, x+A[x]], 之后重复上述过程；
// 3. 这里每一次跳跃的落点都要求下一次能跳跃至最远位置
int jump2(vector<int>& nums) {
    if (nums.size() <= 1) {
        return 0;
    }
    
    int n = nums.size();
    
    int step = 0;  // 记录最少跳跃次数
    int cur_pos = 0;  // 当前起跳位置
    int furthest = 0;  // 记录能跳跃的最远位置

    
    while (cur_pos < n) {
        step++;  // 至少需要一次跳跃
        
        furthest = cur_pos + nums[cur_pos];
        if (furthest >= n - 1) {
            return step;
        }

        // 在区间[cur, furthest]内找到下一次跳跃长度最大的落点
        int next = -1;
        int index = 0;
        for (int i = cur_pos + 1; i <= furthest; ++i) {
          if (i + nums[i] > next) {
            next = i + nums[i];
            index = i;  // 记录落点的位置
          }
        }        
        // 结束for循环后, 确定落点位置为 index <= furthest, 下一次跳跃可以到达最远位置为index+nums[index],
        // 因此后续的落点可以从区间[index, next]内确定
        cur_pos = index;
    }
    // 如果在while循环内没有return step，说明没法移动到最后位置
    return -1;
}
