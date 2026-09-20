/*
 * @Author: zlm 
 * @Date: 2021-03-06 10:10:30 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-03-14 13:58:18
 */

#include "../head_file.h"
using namespace std;

/**
 * @problem: 74. Search a 2D Matrix
 * @description: Write an efficient algorithm that searches for a value in an m x n matrix.
 * This matrix has the following properties: 
 *   Integers in each row are sorted from left to right.
 *   The first integer of each row is greater than the last integer of the previous row.
 * Input: matrix = [[1,3,5,7],[10,11,16,20],[23,30,34,60]], target = 3
 * Output: true
 * @method: 该矩阵每一行从左到右递增，下一行均大于上一行（每一列自上到下递增）
 * 二分法：从最右列自上而下查找，确定元素所在的行，然后在行内调用二分法
 */
class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        if (matrix.size() == 0) {
            return false;
        }
        int row_num = matrix.size();
        int col_num = matrix[0].size();
        for (int r = 0; r < row_num; r++) {
            if (matrix[r][col_num-1] == target) {
                cout << "row:" << r << ", col:" << col_num - 1 << endl;
                return true;
            }
            if (target > matrix[r][col_num - 1]) {
                continue;
            }
            // 当前列元素小于target_info
            return BinarySearch(matrix[r], target);
        }
        return false;
    }

private:
    bool BinarySearch(const vector<int>& vec, int target) {
      if (vec.size() == 0) {
        return false;
      }
      int left = 0, right = vec.size() - 1;
      while (left <= right) {
        int mid = left + (right-left) / 2;
        if (vec[mid] == target) {
          cout << "mid:" << mid << endl;
          return true;
        } else if (vec[mid] < target) {
          left = mid + 1;
        } else {
          right = mid - 1;
        }
      }
      return false;
    }
};


int main() {
  Solution solution;
  vector<vector<int>> matrix = {{1,3,5,7}, {10,11,16,20}, {23,30,34,60}};
  int target = 3;
  bool res = solution.searchMatrix(matrix, target);
  if (res) {
    cout << "find target:" << target << " success!" << endl;
  } else {
    cout << "find target:" << target << " fail!" << endl;
  }
  
  system ("pause");
  return 0;
}