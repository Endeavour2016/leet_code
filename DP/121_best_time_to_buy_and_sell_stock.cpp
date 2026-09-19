/**
 * 121. Best Time to Buy and Sell Stock
 *
 * Keep the lowest price seen before today. Selling today produces the profit
 * prices[today] - min_price; the best candidate is the answer.
 * Time: O(n), space: O(1).
 */

#include <algorithm>
#include <cassert>
#include <limits>
#include <vector>

class Solution {
 public:
  int maxProfit(const std::vector<int>& prices) {
    int min_price = std::numeric_limits<int>::max();
    int best_profit = 0;
    for (const int price : prices) {
      min_price = std::min(min_price, price);
      best_profit = std::max(best_profit, price - min_price);
    }
    return best_profit;
  }
};

int main() {
  Solution solution;
  assert(solution.maxProfit({7, 1, 5, 3, 6, 4}) == 5);
  assert(solution.maxProfit({7, 6, 4, 3, 1}) == 0);
  assert(solution.maxProfit({}) == 0);
  assert(solution.maxProfit({2}) == 0);
  return 0;
}
