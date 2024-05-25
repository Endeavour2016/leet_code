/**
 * @Author: zlm 
 * @Date: 2024-01-14
 * @Last Modified by: zlm
 * @Last Modified time: 2024-01-14
 */

#include "../head_file.h"
#include <unistd.h>
using namespace std;

/**
 * @problem: 3. 219. Contains Duplicate II
 * @descr: Given an integer array nums and an integer k, return true if there are two distinct 
 * indices i and j in the array such that nums[i] == nums[j] and abs(i - j) <= k.
 * 给一个整数数组，判断是否存在重复元素，且要求重复元素下标相差为k
 * @method: 
 */

// 暴力法：会超时
class Solution {
 public:
  bool containsNearbyDuplicate(vector<int>& nums, int k) {
    if (nums.empty()) {
      return false;
    }
    for (int idx = 0; idx < nums.size(); ++idx) {
      for (int j = idx + 1; j <= idx+ k && j < nums.size(); j++) {
        if (nums[j] == nums[idx]) {
          return true;
        }
      }
    }
    return false;
  }
};

bool HasDuplicateNum(vector<int>& nums, int left, int right) {
  std::cout << "nums size: " << nums.size() << ", left: " << left << ", right: " << right << "\n";
  sleep(3);
  if (right >= nums.size()) {
    right = nums.size() - 1;
  }
  std::set<int> temp_set;
  for (int idx = left; idx <= right; ++idx) {
    if (temp_set.find(nums[idx]) != temp_set.end()) {
      return true;
    } else {
      temp_set.insert(nums[idx]);
    }
  }

  return false;
}

// 超时
bool containsNearbyDuplicate2(vector<int>& nums, int k) {
  int num_count = nums.size();
  if (num_count == 0 || k == 0) {
    return false;
  }

  for (int idx = 0; idx <= nums.size() - 1; ++idx) {
    bool is_duplicate = HasDuplicateNum(nums, idx, idx + k);
    if (is_duplicate) {
      return true;
    }
  }
  return false;
}


// 方法三：该问题实际上是要判断长度为k+1的连续子数组中是否存在重复元素，因此实现时可以维护一个最多只包含k个元素的集合
// 插入元素nums[idx]时，判断当前元素是否在集合中存在过，如果存在则两个重复元素的下标差肯定<=k；不存在时则插入当前元素至集合
// 中，如果集合大小超过k，则删除元素nums[idx-k]
bool containsNearbyDuplicate3(vector<int>& nums, int k) {
  int num_count = nums.size();
  if (num_count == 0 || k == 0) {
    return false;
  }
  std::set<int> temp_set;
  for (int idx = 0; idx <= num_count - 1; ++idx) {
    auto iter = temp_set.find(nums[idx]);
    if (iter != temp_set.end()) {
      return true;
    }
    // 插入不重复的元素
    temp_set.insert(nums[idx]);
    if (temp_set.size() == k + 1) {
      temp_set.erase(nums[idx - k]);
    }
  }
  return false;
}

// 官方题解：


int main() {
  std::vector<int> nums = {1};
  int temp = nums.size() - 1;  // -1
  // nums.size() - 1: 显示为 18446744073709551615，原因是 nums.size() 为无符号类型，-1会被当作无符号类型处理
  std::cout << "temp: " << temp  << ", nums.size() - 1: " << nums.size() - 1 << std::endl;
  int k = 1;
  bool res = containsNearbyDuplicate2(nums, k);
  std::cout << "Res: " << res << std::endl;
  return 0;
}


