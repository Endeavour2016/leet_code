
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
 * @brief: BM74 数字字符串转化成IP地址
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
