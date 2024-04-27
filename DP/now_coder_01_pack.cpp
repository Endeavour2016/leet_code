/**
 * @Author: zlm 
 * @Date: 2024-04-11
 * @Last Modified by: zlm
 * @Last Modified time: 2024-04-11
 */
#include <iostream>
#include <vector>

using namespace std;

// 来自牛客网：
// 已知一个背包最多能容纳体积之和为v的物品
// 现有 n 个物品，第 i 个物品的体积为 vi , 重量为 wi
// 求当前背包最多能装多大重量的物品?

/**
 * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
 *
 * 计算01背包问题的结果
 * @param V int整型 背包的体积
 * @param n int整型 物品的个数
 * @param vw int整型vector<vector<>> 第一维度为n,第二维度为2的二维数组,vw[i][0],vw[i][1]分别描述i+1个物品的vi,wi
 * @return int整型
 */

// 明确状态：dp[i][j]--把i个物品放入到总容量为j的背包中，能装下的的最大重量
// 对于第i个物品，若能放下，则可以选择放还是不放，若放则意味着需要从前i-1个物品中选择一组放入容量为j-wi的背包中，
// 使得总重量最大，即dp[i][j] = max{wi + dp[i-1][j-vi], dp[i-1][j]} 
// 若第i个物品放不下：则dp[i][j] = dp[i-1][j]
int knapsack(int V, int n, vector<vector<int> >& vw) {
    // 1.init dp array
    vector<vector<int> > dp(n+1, vector<int>(V+1, 0));

    // 2.calculate states
    for (int i = 1; i <= n; i++) {
      for (int j = 1; j<= V; j++) {
        int cur_voltage = vw[i-1][0];
        int cur_weight = vw[i-1][1];
        if (cur_voltage <= j) {
          dp[i][j] = std::max(cur_weight+ dp[i-1][j-cur_voltage], dp[i-1][j]);
        } else {
          dp[i][j] = dp[i-1][j];
        }
      }
    }

    // 3. return result
    return dp[n][V];
}

int main() {

  return 0;
}