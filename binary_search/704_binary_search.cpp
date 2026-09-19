/**
 * 704. Binary Search
 *
 * Core idea: the target, if it exists, always remains in the closed interval
 * [left, right]. Compare it with the middle element and discard half of the
 * interval on every iteration.
 *
 * Time: O(log n), space: O(1).
 * Common mistake: mixing the update rules for closed and half-open intervals.
 */

#include <cassert>
#include <vector>

class Solution {
 public:
  int search(const std::vector<int>& nums, int target) {
    int left = 0;
    int right = static_cast<int>(nums.size()) - 1;
    while (left <= right) {
      const int middle = left + (right - left) / 2;
      if (nums[middle] == target) {
        return middle;
      }
      if (nums[middle] < target) {
        left = middle + 1;
      } else {
        right = middle - 1;
      }
    }
    return -1;
  }
};

int main() {
  Solution solution;
  assert(solution.search({-1, 0, 3, 5, 9, 12}, 9) == 4);
  assert(solution.search({-1, 0, 3, 5, 9, 12}, 2) == -1);
  assert(solution.search({}, 1) == -1);
  assert(solution.search({5}, 5) == 0);
  return 0;
}
