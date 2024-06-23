
/*
 * @Author: zlm 
 * @Date: 2024-06-09 15:30
 * @Last Modified by: zlm
 * @Last Modified time: 2024-06-09 15:30
 */

#include "../head_file.h"
#include "../data_structure.h"
using namespace std;

/**
 * @brief: 121. Best Time to Buy and Sell Stock(求股票的最大利润)
 * 给定一个数组 prices ，它的第 i 个元素 prices[i] 表示一支给定股票第 i 天的价格。
 * 你只能选择 某一天 买入这只股票，并选择在 未来的某一个不同的日子 卖出该股票。设计一个算法来计算你所能获取的最大利润。
 * 
 * 题目解读：本质上是从数组先后选取两个元素，使其差值最大(按照先买入后卖出的原则，差值=后选的数字-先选的数字) 
 */

 // method 1: 暴力搜索法：计算每一对数字的差值，取最大值！
 // 也可以 Accepted，但是耗时356ms，时间复杂度O(n^2)

int maxProfit(vector<int>& prices) {
    if (prices.empty()) {
        return 0;
    }
    
    int maxPro = 0;
    int len = prices.size();
    for (int i = 0; i < len-1; i++) {  // 第一个选择的数字为prices[i]
        for (int j = i+1; j < len; j++) {
            if (maxPro < prices[j] - prices[i]) {
                maxPro = prices[j] - prices[i];
            }
        }
    }
    return maxPro;
}

// 改进算法：一次遍历, 遍历过程中，用minNum记录当前已经扫描到的最小值; curMax记录当前最大利润
// 时间复杂度O(n), 只需要4ms
int maxProfit(vector<int>& prices) {
    if (prices.size() <= 1)
        return 0;
    
    int len = prices.size();
    int minNum = INT_MAX;
    int maxPro = 0;
    
    int tmp; 
    for (int i = 0; i < len; i++) {
        if (prices[i] < minNum) { // 当前元素price[i]为最小值，则在当前价位不能卖出
            minNum = prices[i];  // 以当前价格作为新的买入点
        } else {
            int curMax = prices[i] - minNum;
            if (curMax > maxPro) {
                maxPro = curMax;
            }
        }
    }
    return maxPro;
}

/**
 * @brief: 122.股票的最大利润II，可以买、卖多次，不限制交易次数
 * @method: 贪心策略
 * 遍历数组，若发现price[i+1]>price[i]，则把price[i+1]-price[i]（即第i天买，i+1天卖）累加计入总利润中；
 * 在代码实现中，从头扫描数组，判断相邻两点价格是否呈上升趋势，若是，则将其差值加入利润中；否则继续扫描其余的点！
 */
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.size() <= 1) {
            return 0;
        }
        
        int len = prices.size();
        int total_pro = 0;
        
        for (int i = 0; i < len-1; i++) {
            if (prices[i+1] > prices[i]) {
                total_pro += prices[i+1] - prices[i];
            }
        }
        return total_pro;
    }
};



/**
 * @brief: BM74 数字字符串转化成IP地址（hard）
 * 现在有一个只包含数字的字符串，将该字符串转化成IP地址的形式，返回所有可能的情况。
 * 例如：给出的字符串为"25525522135",
 * 返回["255.255.22.135", "255.255.221.35"]. (顺序没有关系)
 * @method: DP
 * 由于 IP 地址的每一段必须是 [0,255][0, 255][0,255] 中的整数，
 * 因此我们从 segStart 开始，从小到大依次枚举当前这一段 IP 地址的结束位置 segEnd。
 * 如果满足要求，就递归地进行下一段搜索，调用递归函数 dfs(segId+1, segEnd+1)
 */


class Solution {
private:
    static constexpr int SEG_COUNT = 4;
    vector<string> ans;
    vector<int> segments;  // 存储每一个断对应的整数

public:
    vector<string> restoreIpAddresses(string s) {
        segments.resize(SEG_COUNT);
        dfs(s, 0, 0);
        return ans;
    }

    // 从下标 segStart 开始，搜索第segId 段
    void dfs(const string& s, int segId, int segStart) {
        // 如果找到了 4 段 IP 地址并且遍历完了字符串，那么就是一种答案
        if (segId == SEG_COUNT) {
            if (segStart == s.size()) {
                string ipAddr;
                for (int i = 0; i < SEG_COUNT; ++i) {
                    ipAddr += to_string(segments[i]);
                    if (i != SEG_COUNT - 1) {
                        ipAddr += ".";
                    }
                }
                ans.push_back(move(ipAddr));
            }
            return;
        }

        // 如果还没有找到 4 段 IP 地址就已经遍历完了字符串，那么提前回溯
        if (segStart == s.size()) {
            return;
        }

        // 由于不能有前导零，如果当前数字为 0，那么这一段 IP 地址只能为 0
        if (s[segStart] == '0') {
            segments[segId] = 0;
            dfs(s, segId + 1, segStart + 1);
            return;
        }
        
        // 一般情况，枚举每一种可能性并递归
        int addr = 0;
        // 遍历当前段，并判断是否满足ip地址要求
        for (int segEnd = segStart; segEnd < s.size(); ++segEnd) {
          addr = addr * 10 + (s[segEnd] - '0');
          if (addr > 0 && addr <= 255) {
            segments[segId] = addr;  // 记录当前ip段
            dfs(s, segId + 1, segEnd + 1);  // 递归的对剩余的字符串计算组合
          } else {  // 不合法的ip段
            break;
          }
        }
    }
};
constexpr int Solution::SEG_COUNT;

// 作者：力扣官方题解
// 链接：https://leetcode.cn/problems/restore-ip-addresses/solutions/366426/fu-yuan-ipdi-zhi-by-leetcode-solution/
