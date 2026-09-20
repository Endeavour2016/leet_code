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
 * 1.先访问root节点：记录在path中
 * 2.递归的访问左子树和右子树
 * 3.递归调用后：从path中删除递归调用过程中保存的节点（回溯），以便后续扫描其他节点
 * 
 * 终止条件：遇到叶子节点时，说明本次扫描已经确定了一条路径，记录当前路径，然后return退出DFS函数
 * 之后返回上一层时会弹出栈顶元素，然后继续搜索下一层的其它节点
 */

class Solution {
public:
  vector<string> binaryTreePaths(TreeNode* root) {
    vector<string> result;
    if (nullptr == root) {
      return result;
    }
    vector<int> path;  // 记录搜索过程中的某一个路径
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
    if (root->left == nullptr && root->right == nullptr) {  // 到达叶子节点
      result->push_back(GenerateStringPath(*path));
      return;
    }

    if (root->left != nullptr) {
      DfsTraversal(root->left, path, result);
      path->pop_back();    // 回溯
    }
    if (root->right != nullptr) {
      DfsTraversal(root->right, path, result);
      path->pop_back();    // 回溯
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