/**
 * 27. Remove Element
 *
 * `read` scans every element; `write` marks the next position that should keep
 * a value. The prefix [0, write) always contains exactly the retained values.
 * Time: O(n), space: O(1).
 */

#include <algorithm>
#include <cassert>
#include <vector>

class Solution {
 public:
  int removeElement(std::vector<int>& nums, int value) {
    int write = 0;
    for (const int current : nums) {
      if (current != value) {
        nums[write++] = current;
      }
    }
    return write;
  }
};

int main() {
  Solution solution;
  std::vector<int> nums{0, 1, 2, 2, 3, 0, 4, 2};
  const int size = solution.removeElement(nums, 2);
  assert(size == 5);
  std::sort(nums.begin(), nums.begin() + size);
  assert(std::vector<int>(nums.begin(), nums.begin() + size) ==
         std::vector<int>({0, 0, 1, 3, 4}));

  std::vector<int> empty;
  assert(solution.removeElement(empty, 1) == 0);
  std::vector<int> all_removed{2, 2};
  assert(solution.removeElement(all_removed, 2) == 0);
  return 0;
}
