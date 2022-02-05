/*
 * @Author: zlm 
 * @Date: 2021-06-12 13:58:29 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-06-12 14:34:27
 */

#include <iostream>
#include <vector>
#include "../data_structure.h"

using namespace std;

/**
 * 70. Climbing Stairs
 * @descri:
 * You are climbing a staircase. It takes n steps to reach the top.
 * Each time you can either climb 1 or 2 steps. In how many distinct ways can you climb to the top?
 * @method: 动态规划
 * 用dp[n]表示爬n阶楼梯的方案数量，每一步只能爬1阶或者两阶，所以最终的状态递推式为：
 * dp[n] = dp[n-1] + dp[n-2]
 * 时间复杂度O(n), 空间复杂度 O(n)
 **/

class Solution {
public:
  int climbStairs(int n) {
    if (n <= 2) {
      return n;
    }
    vector<int> dp(n+1, 0);
    dp[1] = 1;
    dp[2] = 2;
    for (int i = 3; i <= n; ++i) {
      dp[i] = dp[i-1] + dp[i-2];
    }
    return dp[n];
  }

  // 优化空间复杂度
  // 用first、second两个变量表示前两个状态
  int climbStairsOpti(int n) {
    if (n <= 2) {
      return n;
    }
    int first = 1;
    int second = 2;
    int res = 0;
    for (int i = 3; i <= n; ++i) {
      res = first + second;
      first = second;
      second = res;
    }
    return res;
  }
};

int main() {
  Solution solution;
  int n = 3;
  int res = solution.climbStairs(n);
  cout << "res: " << res << endl;
  
  system ("pause");
  return 0;
}