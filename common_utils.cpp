// 公共的方法、函数等
#include "head_file.h"
using namespace std;

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

// 二叉树的中序遍历
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