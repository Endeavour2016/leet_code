/**
 * <problem id>. <problem title>
 *
 * Core idea:
 * Invariant:
 * Time: O(...), space: O(...).
 * First mistake / review note:
 */

#include <cassert>
#include <vector>

class Solution {
 public:
  int solve(const std::vector<int>& nums) {
    // Replace this example implementation with the LeetCode solution.
    return static_cast<int>(nums.size());
  }
};

int main() {
  Solution solution;
  // Keep an example, a boundary case and a counterexample here.
  assert(solution.solve({1, 2, 3}) == 3);
  assert(solution.solve({}) == 0);
  return 0;
}
