/*
 * @Author: zlm 
 * @Date: 2021-02-27 21:11:17 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-03-06 10:13:00
 */

#include "../head_file.h"
#include "../data_structure.h"
using namespace std;

/**
 * @problem: 257. Binary Tree Paths
 * @description: Given a binary tree, return all root-to-leaf paths.
 * Note: A leaf is a node with no children.
 * @method: 二叉树的遍历问题之一，可以用DFS实现
 * 从root节点开始，递归的访问每个节点的子节点, 遍历过程中用栈记录已扫描的节点，
 * 遇到叶子节点时，说明本次扫描已经确定了一条路径，记录当前路径。
 * 之后返回上一层时会弹出栈顶元素，然后继续搜索下一层的其它节点
 */

class Solution {
public:
  vector<string> binaryTreePaths(TreeNode* root) {
    vector<string> result;
    if (nullptr == root) {
      return result;
    }
    vector<int> path;  // 记录节点值
    DfsTraversal(root, &path, &result);
    return result;
  }

private:
  void DfsTraversal(TreeNode* root, vector<int>* path, vector<string>* result) {
    if (root == nullptr) {
      return;
    }

    // 记录已扫描节点
    path->push_back(root->val);
    if (root->left == nullptr && root->right == nullptr) {  // 叶子节点
      result->push_back(GenerateStringPath(*path));
    } else {
      if (root->left != nullptr) {
        DfsTraversal(root->left, path, result);  // 递归的搜索左子树
        path->pop_back();    // 递归调用结束后在这里弹出栈顶节点，以便继续后面的搜索
      }
      if (root->right != nullptr) {
        DfsTraversal(root->right, path, result);
        path->pop_back();
      }
    }
  }

  string GenerateStringPath(const vector<int>& path) {
    stringstream ss;
    int i;
    for (i = 0; i < path.size() - 1; i++) {
      ss << path[i] << "->";
    }
    ss << path[i];
    return ss.str();
  }
};

int main() {
  system ("pause");
  return 0;
}