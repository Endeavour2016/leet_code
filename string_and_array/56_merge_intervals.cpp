/*
 * @Author: zlm 
 * @Date: 2021-07-10 10:14:51 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-07-10 11:22:50
 */

#include "../head_file.h"
#include "../data_structure.h"
using namespace std;

/**
 * @problem: 56. Merge Intervals
 * @descr: 以数组 intervals 表示若干个区间的集合，其中单个区间为 intervals[i] = [starti, endi]
 * 请你合并所有重叠的区间，并返回一个不重叠的区间数组，该数组需恰好覆盖输入中的所有区间
 * @method: 
 * 方法1: 先按照区间起点从小到大进行排序，然后从前向后扫描，依次合并有重叠的区间
 * (题目并没有要求最终的数组中区间有序，因此可以排序)
 * 时间复杂度:
 * 空间复杂度:
 */
vector<vector<int>> merge(vector<vector<int>>& intervals) {
  if (intervals.size() == 0) {
    return {};
  }
  vector<vector<int> > merged_res;
  std::sort(intervals.begin(), intervals.end());
  for (const auto& interval : intervals) {
    if (merged_res.empty()) {  // 扫描的第一个区间不需要合并，直接保存
      merged_res.push_back(interval);
      continue;
    }
    int cur_left = interval[0];
    auto& last_interval = merged_res.back();
    int last_interval_right = last_interval.back();
    if (cur_left > last_interval_right) {  // 当前区间左端点大于上一个区间的右端点，不重叠
      merged_res.push_back(interval);
    } else {  // 有重叠，需要合并到上一个区间：只需要更新上一个区间的右端点即可
      int max_val = std::max(last_interval_right, interval.back());
      last_interval.back() = max_val;
    }
  }

  return merged_res;
}


int main() {
  system ("pause");
  return 0;
}
