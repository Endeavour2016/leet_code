/*
 * @Author: zlm 
 * @Date: 2021-02-27 10:06:30 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-02-27 10:51:02
 */

// 刷题过程中常用数据结构定义

// Definition for a binary tree node.
struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) :
      val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode *left, TreeNode *right) :
      val(x), left(left), right(right) {}
};

struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};