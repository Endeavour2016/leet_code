/*
 * @Author: zlm 
 * @Date: 2021-02-27 10:08:02 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-02-27 23:39:59
 */

#include <iostream>
#include <climits>
#include <vector>
#include <stack>
#include <string>
#include "../data_structure.h"

using namespace std;

/**
 * @problem: 144. Binary Tree Preorder Traversal
 * @descr: Given the root of a binary tree, return the preorder traversal of its nodes' values.
 */
class Solution {
public:
  vector<int> preorderTraversal(TreeNode* root) {
    vector<int> nodes;
    PreOrder(root, &nodes);
    return nodes;
  }

private:
  void PreOrder(TreeNode* root, vector<int>* nodes) {
    if (root != nullptr) {
      nodes->push_back(root->val);
      PreOrder(root->left, nodes);
      PreOrder(root->right, nodes);
    }
  }
  
  // 非递归实现
  // 为什么用栈呢？访问完root节点后要先访问左子节点，访问完之后需要能找到右子节点的指针，然后才能访问右子节点
  // 因此需要提前存放右子节点指针，最后再取出其指针，此过程满足“先进后出”的特性
  void PreOrderNoRecursive(TreeNode* root, vector<int>* nodes) {
    if (root == nullptr) {
      return;
    }
    stack<TreeNode*> st;
    st.push(root);  // 根节点只有一个, 直接入栈

    while (!st.empty()) {
      // 访问当前栈顶元素,访问后弹出栈
      TreeNode* cur_root = st.top();
      nodes->push_back(cur_root->val);
      st.pop();

      // 先存右子节点
      if (cur_root->right != nullptr) {
        st.push(cur_root->right);
      }
      // 再存左子节点
      if (cur_root->left != nullptr) {
        st.push(cur_root->left);
      }
    }
  }
};

/**
 * @problem: 105. Construct Binary Tree from Preorder and Inorder Traversal
 * @descr:
 * Given two integer arrays preorder and inorder where preorder is the preorder 
 * traversal of a binary tree and inorder is the inorder traversal of the same tree,
 * construct and return the binary tree.
 * 根据前序和中序结果，构造二叉树
 * 题目保证输入数据均正确，且数中没有重复数字
 * 
 * 思路：利⽤递归思想，根据前序获得root，根据中序得到左⼦树和右⼦树
 * 计算左子树的preorder和inorder,递归构造左子树
 * 右子树同上
 */
TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
  if (preorder.empty() || inorder.empty()) {
    return nullptr;
  }
  int pre_order_len = preorder.size();
  int in_order_len = inorder.size();
  // 确定root节点
  int root_node_val = preorder.front();
  TreeNode* root = new TreeNode(root_node_val);

  // 根节点在中序遍历结果中的位置
  int root_node_index = 0;
  for (int i = 0; i < in_order_len; ++i) {
    if (inorder[i] == root_node_val) {
      root_node_index = i;
      break;
    }
  }

  // 计算左右子树的中序序列
  vector<int> left_tree_inorder(inorder.begin(), inorder.begin() + root_node_index);
  vector<int> right_tree_inorder(inorder.begin() + root_node_index + 1, inorder.end());
  
  // 计算左右子树的前序序列
  int left_tree_preorder_len = root_node_index;
  vector<int> left_tree_preorder(preorder.begin() + 1, preorder.begin() + 1 + left_tree_preorder_len);
  vector<int> right_tree_preorder(preorder.begin() + 1 + left_tree_preorder_len, preorder.end());

  // 递归构造左右子树
  root->left = buildTree(left_tree_preorder, left_tree_inorder);
  root->right = buildTree(right_tree_preorder, right_tree_inorder);
  return root;
}

/**
 * @problem: 701 Insert into a Binary Search Tree(Medium) 
 * @descr: 二叉树的插入(递归)
 * You are given the root node of a binary search tree (BST) and a value 
 * to insert into the tree. Return the root node of the BST after the insertion.
 * It is guaranteed that the new value does not exist in the original BST.
 */ 
TreeNode* insertIntoBST(TreeNode* root, int val) {
  TreeNode* node;
  if (nullptr == root) {
    node = new TreeNode(val);
    return node;
  }
  // 递归的左右子树中插入节点
  if (val < root->val) {
    root->left = insertIntoBST(root->left, val);
  } else {
    root->right = insertIntoBST(root->right, val);
  }
  return root;
}

/**
 * @problem: 二叉树的中序遍历
 * @descr:
 * @method: 迭代
 */
void InOrderByIterator(TreeNode *root) {
  if (root == nullptr) {
    return;
  }

  stack<TreeNode*> st;
  TreeNode* p = root;
  while (p != nullptr || !st.empty()) {
    if (p != nullptr) {
      st.push(p);
      p = p->left;
    } else {
      p = st.top();
      st.pop();
      cout << p->val << " ";
      p = p->right;
    }
  }
}

/**
 * @problem: 108 前序遍历构造二叉搜索树
 * @descr: Construct Binary Search Tree from Preorder Traversal
 * @method:
 */
class Solution {
public:
  TreeNode* bstFromPreorder(vector<int>& preorder) {

  }
};

// ====================================================================
int main() {
  cout <<"hello";
  system("pause");
}