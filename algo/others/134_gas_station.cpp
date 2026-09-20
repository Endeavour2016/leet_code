/*
 * @Author: zlm 
 * @Date: 2024-04-27
 * @Last Modified by: zlm
 * @Last Modified time: 2024-04-27
 */

#include <iostream>
#include <vector>
#include "../data_structure.h"

using namespace std;

/**
 * @problem: 1143. Longest Common Subsequence
 * @descr: 在一条环路上有 n 个加油站，其中第 i 个加油站有汽油 gas[i] 升。
 * 你有一辆油箱容量无限的的汽车，从第 i 个加油站开往第 i+1 个加油站需要消耗汽油 cost[i] 升。
 * 你从其中的一个加油站出发，开始时油箱为空。
 * 给定两个整数数组 gas 和 cost ，如果你可以按顺序绕环路行驶一周，则返回出发时加油站的编号，否则返回 -1 。
 * 如果存在解，则 保证 它是 唯一 的。
 * @method: 
 */

// 解法一：暴力法(会超时)
// 遍历所有的加油站，对于每个加油站，判断从此出发能否再回到该位置
// https://leetcode.cn/problems/gas-station/solutions/25644/xiang-xi-tong-su-de-si-lu-fen-xi-duo-jie-fa-by--30/?envType=study-plan-v2&envId=top-interview-150

int CanReturn(const vector<int>& gas, const vector<int>& cost,
              int start_pos, int station_num);

int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
  int num = gas.size();

  // 考虑以不同的加油站作为起点
  for (int idx = 0; idx < num; ++idx) {
    int res = CanReturn(gas, cost, idx, num);
    if (res != -1) {
      return res;
    }
  }

  // 走到这里，说明从任何点出发都没法回到原点
  return -1;
}


// 判断从 start_pos 出发，能否回到原点
int CanReturn(const vector<int>& gas, const vector<int>& cost,
              int start_pos, int station_num) {
  // 当前起点处剩余的汽油量
  int cur_remain = gas[start_pos];

  // 当前剩余的油能否到达下一个点，若能到达则继续向后扫描
  int j = start_pos;
  while (cur_remain - cost[j] >= 0) {
    // 下一个加油站的补给 + 本次行程的消耗
    int next_pos = (j + 1) % station_num;
    cur_remain = cur_remain - cost[j] + gas[next_pos];
    j = next_pos;
    if (j == start_pos) {
      return start_pos;
    }
  }
  return -1;
}