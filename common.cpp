// 公共的方法、函数等
#include "head_file.h"
#include "data_structure.h"
using namespace std;

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