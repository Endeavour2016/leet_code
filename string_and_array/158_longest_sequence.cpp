#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/**
 * @brief: 128. Longest Consecutive Sequence（最长的连续序列）
 * 给定一个未排序的整数数组 nums ，找出数字连续的最长序列（不要求序列元素在原数组中连续）的长度。
 * 请你设计并实现时间复杂度为 O(n) 的算法解决此问题。
 * @method: 利用hash表记录每个元素出否访问过，对于每个元素i, 分别从右边和左边查找其相邻元素（i-1或j+1）是否在数组中,
 * 若在，则将连续序列的长度 len++; 这样可以求出当前元素 i 所在的连续序列的长度，取出其中最大的那个
 * 时间复杂度O(n), 空间复杂度 O(n)
 */
// Input: nums = [100,4,200,1,3,2]
// Output: 4
// Explanation: The longest consecutive elements sequence is [1, 2, 3, 4]. Therefore its length is 4.

int longestConsecutive(vector<int>& nums) {
  if (nums.empty()) {
    return 0;
  }
  unordered_map<int, bool> used;  // 用来表示数组中的元素是否被访问过
  for (auto v : nums) {
    used[v] = false;
  }

  int maxLength = 0;

  for (auto i : nums) {
    if (used[i]) {  // 如果元素已经使用过，则没必要再次使用
      continue;
    }

    int len = 1;   // 记录当前查找的序列的长度
    used[i] = true;

    // 向右查找与元素i相邻的元素
    for (int j = i + 1; used.find(j) != used.end(); j++) {
      used[j] = true;
      len++;
    }

    // 向左查找与元素i相邻的元素
    for (int j = i - 1; used.find(j) != used.end(); j--) {
      used[j] = true;
      len++;
    }

    if (len > maxLength) {
      maxLength = len;
    }
  }
  
  return maxLength;
}