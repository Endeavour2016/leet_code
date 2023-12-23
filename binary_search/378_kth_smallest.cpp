/*
 * @Author: zlm 
 * @Date: 2021-03-14 13:59:34 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-03-14 15:23:20
 */

#include "../head_file.h"
using namespace std;

/**
 * @problem:378. Kth Smallest Element in a Sorted Matrix
 * @description:Given an n x n matrix where each of the rows and columns are sorted in ascending
 * order, return the kth smallest element in the matrix.(k>=1 && k <= N^2)
 * 题目大意：给定⼀个 n x n 矩阵，其中每⾏和每列元素均按升序排序，找到矩阵中第 k ⼩的元素。
 * 请注意，它是排序后的第 k ⼩元素，⽽不是第 k 个元素。
 * @method: 
 * 方法1：维护一个最大堆，前k个元素先放入堆中，然后遍历对于剩余的元素，
 * 如果当前元素大于堆顶，则跳过该元素（因为已经至少有k个元素小于该元素），否则用该元素替换堆顶
 */

class Solution {
public:
  int kthSmallest(vector<vector<int>>& matrix, int k) {
    priority_queue<int> max_queue;
    int index = 0;
    int len = matrix.size();
    for (int row = 0; row < len; ++row) {
      for (int col = 0; col < len; ++col) {
        ++index;
        if (index <= k) {
          max_queue.push(matrix[row][col]);
        } else {
          int top = max_queue.top();
          if (top <= matrix[row][col]) {  // 大于堆顶的元素跳过
            continue;
          }
          max_queue.pop();
          max_queue.push(matrix[row][col]);
        }
      }
    }
    // 此时堆顶即为第k小的元素
    return max_queue.top();
  }
};

// 方法2: 最优解法-二分法
// 确定查找的区间：矩阵左上角为最小值，右下角为最大值
// 查找的过程中，对区间不断划分：
// 如果小于=等于 mid_value的元素个数 count < k 时，在右半区间查找，此时更新low为mid+1
// 如果 count >= k，此时在左区间查找，更新high=low;

class Solution2 {
public:
  int kthSmallestBinary(vector<vector<int>>& matrix, int k) {
    int len = matrix.size();
    int low = matrix[0][0];
    int high = matrix[len-1][len-1];
    while (low < high) {
      int mid = low + (high-low) / 2;
      // 统计小于等于mid的元素个数
      int cnt = ElementsCounterLessThanMid(matrix, len, mid);
      if (cnt < k) {
        low = mid + 1;
      } else {
        high = mid;
      }
    }
    return low;
  }

 private:
  int ElementsCounterLessThanMid(const vector<vector<int>>& matrix,
                                 int len, int mid) {
    int cnt = 0;
    for (int row = 0; row < len; ++row) {
      for (int col = len-1; col >= 0; col--) {
        if (matrix[row][col] <= mid) {
          cnt += col+1;
          break;
        }
      }
    }
    return cnt;
  }
};

int main() {
  Solution solution;
  vector<vector<int>> matrix = {{1,5,9}, {10,11,13}, {12,13, 15}};
  int k = 8;
  int res = solution.kthSmallest(matrix, k);
  cout << "res: " << res << endl;
  
  system ("pause");
  return 0;
}