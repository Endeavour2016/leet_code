/*
 * @Author: zlm 
 * @Date: 2021-07-11 09:18:17 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-07-11 11:48:52
 */
#include "../head_file.h"
#include "../data_structure.h"
using namespace std;

/**
 * @problem: 48. Rotate Image
 * @descr: 给定一个 n × n 的二维矩阵 matrix 表示一个图像, 你将图像顺时针旋转 90 度
 * 要求原地修改，且不能创建额外的矩阵来存储数据
 * @method: 
 * 方法一：(1)分析每个元素旋转后的位置，可以得到规律：
 * 矩阵中第 i 行的第 j 个元素，在旋转后，它出现在倒数第i列的第j个位置，即矩阵中[i,j]位置元素，
 * 旋转后其新位置为[j, n-1-i];
 * (2) 对于每个数字，按照间隔90度顺时针查找可以确定4个位置，将这四个位置按顺时针替换位置，
 * 由于一次性可以原地替换四个数字，因此只需要处理1/4矩阵中的数据即可
 * (3) 代码上可以逐行处理,需要注意数组下标的范围
 * (4) 空间复杂度O(1)
 */

class Solution {
public:
  void rotate(vector<vector<int>>& matrix) {
    int n = matrix.size();
    for (int i = 0; i < n / 2; ++i) {
      for (int j = 0; j < (n + 1) / 2; ++j) {
        int temp = matrix[i][j];
        matrix[i][j] = matrix[n - j - 1][i];  // 左下角元素放到左上角
        matrix[n - j - 1][i] = matrix[n - i - 1][n - j - 1];  // 右下角元素放到左下角
        matrix[n - i - 1][n - j - 1] = matrix[j][n - i - 1];  // 右上角元素放到右下角
        matrix[j][n - i - 1] = temp;  // 左上角元素放到由上角
      }
    }
  }
};


int main() {
  system ("pause");
  return 0;
}