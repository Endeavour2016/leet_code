/**
 * 122. Best Time to Buy and Sell Stock II
 *
 * Add every positive adjacent price difference. This is equivalent to buying
 * at each valley and selling at each peak.
 * Time: O(n), space: O(1).
 */

#include <algorithm>
#include <cassert>
#include <vector>

class Solution {
 public:
  int maxProfit(const std::vector<int>& prices) {
    int total_profit = 0;
    for (std::size_t day = 1; day < prices.size(); ++day) {
      total_profit += std::max(0, prices[day] - prices[day - 1]);
    }
    return total_profit;
  }
};

int main() {
  Solution solution;
  assert(solution.maxProfit({7, 1, 5, 3, 6, 4}) == 7);
  assert(solution.maxProfit({1, 2, 3, 4, 5}) == 4);
  assert(solution.maxProfit({7, 6, 4, 3, 1}) == 0);
  assert(solution.maxProfit({}) == 0);
  return 0;
}
