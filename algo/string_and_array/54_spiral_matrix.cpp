/*
 * @Author: zlm 
 * @Date:  2024-06-02 14:50
 * @Last Modified by: zlm
 * @Last Modified time: 2024-06-02 14:50
 */
#include <vector>
using namespace std;

/**
 * @brief: 54. 螺旋矩阵
 * 给你一个 m 行 n 列的矩阵 matrix ，请按照 顺时针螺旋顺序 ，返回矩阵中的所有元素。
 * @method:
 * 方法二：按层模拟
 * 可以将矩阵看成若干层，首先输出最外层的元素，其次输出次外层的元素，直到输出最内层的元素。
 * 对于每一层，先遍历上面一行，其次遍历最右边一列，再次，遍历下面一行，最后遍历左边一列
 */
vector<int> spiralOrder(vector<vector<int>>& matrix) {
    if (matrix.size() == 0 || matrix[0].size() == 0) {
        return {};
    }

    int rows = matrix.size(), columns = matrix[0].size();
    vector<int> order;
    int left = 0, right = columns - 1, top = 0, bottom = rows - 1;
    while (left <= right && top <= bottom) {  // 逐层遍历
        // 对于当前层
        // 1.遍历上面一行元素
        for (int c = left; c <= right; c++) {
            order.push_back(matrix[top][c]);
        }
        // 2.遍历右侧一列元素
        for (int row = top + 1; row <= bottom; row++) {
            order.push_back(matrix[row][right]);
        }
        
        if (left < right && top < bottom) {
            // 3.遍历下面一行元素
            for (int c = right - 1; c > left; c--) {
                order.push_back(matrix[bottom][c]);
            }
            // 4.遍历左侧一列元素
            for (int row = bottom; row > top; row--) {
                order.push_back(matrix[row][left]);
            }
        }
        left++;
        right--;
        top++;
        bottom--;
    }
    return order;
}
