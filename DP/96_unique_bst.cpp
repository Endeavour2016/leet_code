/*
 * @Author: zlm 
 * @Date: 2021-02-27 15:49:12 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-02-27 21:12:21
 */

#include <iostream>
#include <vector>
#include "../data_structure.h"

using namespace std;

/**
 * 96. Unique Binary Search Trees (Medium)
 * @description:
 * 给一个整数n，求1~n作为节点构成的二叉搜索树的个数
 * @label: 动态规划
 * 根据题目提供的example发现规律：当根节点为i时，二叉树的个数为“左子树个数*右子树个数”
 * 其中左子树由[1~i-1]组成，右子树由[i+1,n]组成
 * 这里用f(i)表示由[1~i]能够组成的BST的个数，其中根节点的可能取值为1~i
 * 令i取值为0~3，则容易推导f(i)的计算规律:
 * f(0) = 0, f(1) = 1
 * f(2) = [f(0)*f(1), 1作为根的情况] + [ f(1)*(0), 2作为根的情况]
 * f(3) = f(0)*f(2) + f(1)*f(1) + f(2)*f(0)
 * f(i)的递推式为：f(i) = 累加 f(k-1)*(i-k), k=1~i
 * 至此，该问题划归为一维动态规划
 */

// 时间复杂度O(n), 空间复杂度 O(n)
class Solution {
public:
  int numTrees(int n) {
    vector<int> f(n+1, 0);
    f[0] = 1;
    f[1] = 1;
    for (int i = 2; i <= n; ++i) {
      int temp = 0;
      for (int k = 1; k <= i; k++) {
        temp += f[k-1]*f[i-k];
      }
      f[i] = temp;
    }
    return f[n];
  }
};

int main() {
  Solution solution;
  int n = 3;
  int num = solution.numTrees(n);
  cout << "BST num is: " << num << endl;
  system("pause");
  return 0;
}