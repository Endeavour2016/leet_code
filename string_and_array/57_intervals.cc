/*
 * @Author: zlm 
 * @Date: 2024-03-23
 * @Last Modified by: zlm
 * @Last Modified time: 2024-03-23
 */

#include <iostream>
#include <vector>
#include "../data_structure.h"

using namespace std;

/**
 * @problem: 57. 插入区间
 * @descr: 给你一个 无重叠的 ，按照区间起始端点排序的区间列表 intervals，
 * 其中 intervals[i] = [starti, endi] 表示第 i 个区间的开始和结束，并且 intervals 按照 starti 升序排列。
 * 同样给定一个区间 newInterval = [start, end] 表示另一个区间的开始和结束。在 intervals 中插入区间 newInterval，
 * 使得 intervals 依然按照 starti 升序排列，且区间之间不重叠（如果有必要的话，可以合并区间）。
 * 返回插入之后的 intervals。
 * 注意 你不需要原地修改 intervals。你可以创建一个新数组然后返回它。
 * @method: 二分法确定起点和终点所在的区间位置，然后合并中间区间，并确定新区间前后相邻区间的位置（在原有区间集合中的下标）
 * 时间复杂度O(log(n)+n), 空间复杂度 O(n)
 **/

// 输入：intervals = [[1,3],[6,9]], newInterval = [2,5]
// 输出：[[1,5],[6,9]]

// 输入：intervals = [[1,2],[3,5],[6,7],[8,10],[12,16]], newInterval = [4,8]
// 输出：[[1,2],[3,10],[12,16]]
// 解释：这是因为新的区间 [4,8] 与 [3,5],[6,7],[8,10] 重叠。

// 确定一个数在区间集合中的位置，如果data存在于某个区间，则返回该区间的下标
// 否则，返回该数前后相邻区间的下标，比如data=3，intervals={[1,2], [6,8]}, 则返回下标 {0,1}
void FindPosition(int data, vector<vector<int>>& intervals, std::vector<int>* pos_vec) {
  int left = 0;
  int right = intervals.size() - 1;
  while (left <= right) {
    int mid = (left + right) / 2;
    if (data >= intervals[mid][0] && data <= intervals[mid][1]) {
      pos_vec->push_back(mid);
      break;
    }
    if (data < intervals[mid][0]) {
      right--;
      continue;
    }
    if (data > intervals[mid][1]) {
      left++;
      continue;
    }
  }

  if (left > right) {  // 表明data并没有落在某个区间内
    int left_interval_pos = right >=0 ? right : -1;
    int right_interval_pos = left < intervals.size() ? left : intervals.size();
    pos_vec->push_back(left_interval_pos);
    pos_vec->push_back(right_interval_pos);
  }

  // test log
  std::cout << "\ndata: " << data << "\n";
  for (auto d : *pos_vec) {
    std::cout << "pos: " << d << ",";
  }
  std::cout << "\n";
}


vector<vector<int>> insert(vector<vector<int>>& intervals, vector<int>& newInterval) {
  std::vector<std::vector<int> > res;

  // 1、找到新区间的起点和终点在区间集合中的位置
  std::vector<int> start_pos;
  FindPosition(newInterval[0], intervals, &start_pos);

  std::vector<int> end_pos;
  FindPosition(newInterval[1], intervals, &end_pos);

  // 2、计算新区间
  vector<int> new_interval;
  int new_interval_left_pos = 0;
  int new_interval_right_pos = 0;
  if (start_pos.size() == 1 && end_pos.size() == 1) {
    auto& start_interval = intervals[start_pos[0]];
    auto& end_interval = intervals[end_pos[0]];
    new_interval.push_back(start_interval[0]);
    new_interval.push_back(end_interval[1]);
    new_interval_left_pos = start_pos[0] - 1;
    new_interval_right_pos = end_pos[0] + 1;
  } else if (start_pos.size() == 2 && end_pos.size() == 1) {
    auto& end_interval = intervals[end_pos[0]];
    new_interval.push_back(newInterval[0]);
    new_interval.push_back(end_interval[1]);
    new_interval_left_pos = start_pos[0];
    new_interval_right_pos = end_pos[0] + 1;
  } else if (start_pos.size() == 1 && end_pos.size() == 2) {
    auto& start_interval = intervals[start_pos[0]];
    new_interval.push_back(start_interval[0]);
    new_interval.push_back(newInterval[1]);
    new_interval_left_pos = start_pos[0] - 1;
    new_interval_right_pos = end_pos[1];
  } else {
    new_interval.push_back(newInterval[0]);
    new_interval.push_back(newInterval[1]);
    new_interval_left_pos = start_pos[0];
    new_interval_right_pos = end_pos[1];
  }

  for (int idx = 0; idx <= new_interval_left_pos; ++idx) {
    res.push_back(intervals[idx]);
  }
  res.push_back(new_interval);
  for (int idx = new_interval_right_pos; idx < intervals.size(); ++idx) {
    res.push_back(intervals[idx]);
  }

  return res;
}


int main() {
  vector<vector<int>> intervals = {{1, 5}};
  vector<int> newInterval = {2, 7};

  vector<vector<int> > res = insert(intervals, newInterval);
  for (auto & vec : res) {
    std::cout << "\nvec: " << "\n";
    for (auto val : vec) {
      std::cout << val << ", ";
    }
    std::cout << "\n";
  }
  return 0;
}