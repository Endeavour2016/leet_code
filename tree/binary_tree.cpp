/*
 * @Author: zlm 
 * @Date: 2021-02-27 10:08:02 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-02-27 23:39:59
 */

#include "../head_file.h"
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
 * @descr: 二叉树的插入
 * @method: 递归
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
 * 先遍历左子树，再遍历根节点，最后遍历右子树
 * 1、从树的root开始，先找到左子树最底层的叶子节点(左子节点)
 * 2、访问左子节点->访问当前根节点->访问右子树(这个过程跟步骤1一样)
 * 3、为了保证访问完左子节点之后，能够找到其根节点，步骤1在遍历左子树的过程中，需要按照
 * 遍历的顺序将左子树的根节点入栈，这样出栈时即可保证按中序来访问根节点
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
 * @problem: 1008 Construct Binary Search Tree from Preorder Traversal 
 * @descr: 根据前序遍历的结果构造二叉搜索树, 按照题目要求，这里的二叉树严格满足
 * 左子树节点均小于跟节点，右子树节点均大于根节点
 * @method: 递归、二分查找
 * 根据【前序遍历】的定义：前序遍历的第 1 个结点一定是二叉树的根结点
 * 除第一个节点，剩下的节点可以分为两个区间：
 * 第 1 个子区间里所有的元素都严格小于根节点，可以递归构建成根节点的左子树
 * 第 2 个子区间里所有的元素都严格大于根节点，可以递归构建成根节点的右子树
 * 找到这两个子区间的分界线，可以使用二分查找
 */

// 更方便实现递归调用
TreeNode* GenBstRecursive(const vector<int>& preorder, int left, int right) {
  if (left > right) {
    return nullptr;
  }
  int root_val = preorder[left];
  TreeNode* root_node = new TreeNode(root_val);

  // 找到两个区间的边界
  int low = left + 1;
  int high = right;
  int mid = 0;
  while (low < high) {
    mid = low + (high - low) / 2;
    if (preorder[mid] < root_val) {
      low = mid + 1;
    } else {
      high = mid;  // 此处不用mid-1是为了保证low能恰好追上high，不会出现low > high的case
    }
  }
  // 理论上此时 low == high，但还不确定是左子树区间的右边界还是右子树区间的左边界
  mid = low;
  if (mid == right && preorder[mid] < root_val) {  // 此时表明当前的roo节点没有右子树
    root_node->left = GenBstRecursive(preorder, left+1, mid);
    root_node->right = GenBstRecursive(preorder, mid+1, right);
  } else {
    root_node->left = GenBstRecursive(preorder, left+1, mid-1);
    root_node->right = GenBstRecursive(preorder, mid, right);
  }
  return root_node;
}

TreeNode* BstFromPreorder(vector<int>& preorder) {
  int len = preorder.size();
  if (len == 0) {
    return nullptr;
  }
  return GenBstRecursive(preorder, 0, len-1);
}

/**
 * @problem: 102. Binary Tree Level Order Traversal
 * @descr: 
 * @method: BFS
 */
class Solution {
public:
  vector<vector<int> > levelOrder(TreeNode *root) {
    vector<vector<int>> res;
    if (root == nullptr) {
      return res;
    }

    queue<TreeNode*> que; // 用来存放结点
    que.push(root);

    // while循环控制逐层向下遍历
    while (!que.empty()) {

      vector<int> curLevel;  // 用来存放当前层的节点
      int len = que.size();  // 当前层的结点个数

      // for循环遍历当前层
      for (int i = 0; i < len; i++) {
        TreeNode *tmp = que.front(); // 先取出队首元素
        curLevel.push_back(tmp->val);

        // 先把下一层的节点放到que后面
        if (tmp->left != nullptr) {
          que.push(tmp->left);
        }
        if (tmp->right != nullptr) {
          que.push(tmp->right);
        }

        que.pop(); // 弹出队首
      }   // 在每一个for循环后，当前层的结点全部存入到curLevel中, 此时que存放的是下一层的结点
    
      res.push_back(curLevel);
    }
    return res;
  }
};

// ====================================================================
int main() {
  cout <<"hello";
  system("pause");
}