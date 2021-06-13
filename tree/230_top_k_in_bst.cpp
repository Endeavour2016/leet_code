/*
 * @Author: zlm 
 * @Date: 2021-06-13 11:12:00 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-06-13 15:10:23
 */

#include "../head_file.h"
using namespace std;

/**
 * @problem: 230. Kth Smallest Element in a BST
 * @description: 二叉搜索树中第k小的元素
 * @method:
 * 方法一：先中序遍历，得到有序数组array，则第k小的元素显示为array[k-1]
 * 时空复杂度均为O(N)
 */

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

class Solution {
public:
  int kthSmallest(TreeNode* root, int k) {
    vector<int> traversal_res;
    InOrder(root, &traversal_res);
    if (k <= traversal_res.size()) {
      return traversal_res[k-1];
    }

    return 0;
  }

private:
  void InOrder(TreeNode* root, vector<int>* res) {
    if (root == nullptr) {
      return;
    }
    InOrder(root->left, res);
    res->push_back(root->val);
    InOrder(root->right, res);
  }
};

// 方法二：中序遍历剪枝：遍历到第k小的节点即停止，不需要记录所有的节点
// 时间复杂度: O(log(n) + k), 空间复杂度: O(1)
class Solution2 {
public:
  int kthSmallest(TreeNode* root, int k) {
    int result = 0;
    InOrder(root, &k, &result);
    return result;
  }

private:
  void InOrder(TreeNode* root, int* k, int* result) {
    if (root == nullptr || *k == 0) {
      return;
    }

    InOrder(root->left, k, result);
    if (*k == 0) {  // 说明左子树遍历已经找打了Kth小的节点，则当前跟节点无需再遍历
      return;
    }

    --(*k);
    if (*k == 0) {
      *result = root->val;
      return;
    }

    InOrder(root->right, k, result);
  }

  // 迭代的方式进行中序遍历
  void InOrderByIterator(TreeNode* root, int* k, int* result) {
    if (root == nullptr || *k == 0) {
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
        --(*k);
        if (*k == 0) {  // 已经找到第Kth小的元素
          *result = p->val;
          break;
        }
        p = p->right;
      }
    }
  }
};


int main() {
  return 0;
}