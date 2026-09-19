/**
 * 384. Shuffle an Array
 *
 * `std::shuffle` implements a uniform permutation using the supplied random
 * engine. Keep the original array unchanged so reset() is reliable.
 * Time: O(n), extra space: O(n) for the returned copy.
 */

#include <algorithm>
#include <cassert>
#include <random>
#include <utility>
#include <vector>

class Solution {
 public:
  explicit Solution(std::vector<int> nums)
      : original_(std::move(nums)), engine_(std::random_device{}()) {}

  std::vector<int> reset() const { return original_; }

  std::vector<int> shuffle() {
    std::vector<int> result = original_;
    std::shuffle(result.begin(), result.end(), engine_);
    return result;
  }

 private:
  std::vector<int> original_;
  std::mt19937 engine_;
};

int main() {
  Solution solution({1, 2, 3, 4});
  assert(solution.reset() == std::vector<int>({1, 2, 3, 4}));
  auto shuffled = solution.shuffle();
  std::sort(shuffled.begin(), shuffled.end());
  assert(shuffled == std::vector<int>({1, 2, 3, 4}));
  assert(solution.reset() == std::vector<int>({1, 2, 3, 4}));
  return 0;
}
