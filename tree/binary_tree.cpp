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
 * @brief: 124. 二叉树中的最大路径和（hard）
 * 二叉树中的 路径 被定义为一条节点序列，序列中每对相邻节点之间都存在一条边。
 * 同一个节点在一条路径序列中 至多出现一次。该路径 至少包含一个 节点，且不一定经过根节点。
 * @method：递归/DFS
 * MaxPath(node): 表示以node节点为起点的最大路径和，则 
 * MaxPath(root) = root->val + std::max(left_max_path, right_max_path)
 * 其中left_max_pth可以递归的调用 MaxPath 获得
 */
class Solution {
public:
    int maxPathSum(TreeNode* root) {
        int max_path = INT_MIN;
        MaxPath(root, &max_path);
        return max_path;
    }
    int MaxPath(TreeNode* node, int* max_path) {
        if (node == nullptr) {
            return 0;
        }
        
        // 递归计算左右子节点的最大路径
        // 只有在最大路径大于 0 时，才会选取对应子节点
        int left_max_path = std::max(MaxPath(node->left, max_path), 0);
        int right_max_path = std::max(MaxPath(node->right, max_path), 0);

        // 更新全局的最大路径和
        // left->root->right 作为路径与已经计算过历史最大值做比较
        *max_path = std::max(*max_path, node->val + left_max_path + right_max_path);

        // 当前node的最大路径取决于当前节点的值与左右子节点的最大路径值
        return node->val + max(right_max_path, left_max_path);
    }
};


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
};


// 前序遍历-非递归版本实现思路
// 1.先把root入栈：root
// 2.访问栈顶，即vist(root)，然后弹出栈
// 3.root->right入栈：[right]
// 4.root->left入栈：[right, left]
// 重复步骤 2～4：保证每次访问根节点后，之后两次的访问顺序都是先left后right
void PreOrderNoRecursive(TreeNode* root, vector<int>* nodes) {
    if (root == nullptr) {
        return;
    }
    stack<TreeNode*> st;
    st.push(root); // 根节点先入栈

    while (!st.empty()) {
        // 先访问root：即当前栈顶
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

/*
前序遍历过程分析
1.root
2.访问左子树 left0 {
    root(left0)
    left1 {
        (root)left1
        left2 {
            root(left2)
            nullptr
            right3
        }
        right2
    }
    right1
}

3.访问右子树 right0
*/

// 前序遍历：实现思路二
// 跟下面中序的写法类似，注意真正visit(node)的地方：
// 1、先访问根节点，然后一直遍历左子节点，每遍历一个节点时直接访问该节点，但此时节点还不能丢，需要存入栈中，直到左子节点为空
// 2、开始出栈，根据弹出的节点找到其右子树，开始遍历右子树：即重复步骤1、2
// p = stack.top(); st.pop(); p=p->right

void PreOrderNoRecursive(TreeNode* root, vector<int> &res) {
    stack<TreeNode*> st;
    TreeNode* p = root;

    while (p != nullptr || !st.empty()) {
        while (p != nullptr) {
            res.push_back(p->val);  // 1.先访问根节点【visit】
            st.push(p);
            p = p->left;            // 2.下一次循环时访问的是左子节点
        }
        // 3.开始出栈，取出右子节点，这样进入到下一次循环时开始访问右子树
        if (!st.empty()) {
            p = st.top();
            st.pop();
            p = p->right;
        }
    }
}


/**
 * @problem: 94. Binary Tree Inorder Traversal
 * @descr: Given the root of a binary tree, return the inorder traversal of its nodes' values.
 */

/* 中序遍历过程分析
     root
   /      \
left0    right0

1.left0 {
     left1  {
          left2  {
              null
              left2(root)
              null
          }
          left1(root)
          right2
     }
     left0(root)
     right1
}

2.root

3.right0

分析思路：
1、整体顺序是“left0->root->right0“，访问left的过程则需要一直向下找到最里层的左子节点，
这个查找过程对应入栈顺序，直到某个左子节点的左孩子为null：left0, left1, left2
2.从最里面的左子树开始访问（对应出栈顺序）：pop(left2)->left2的右子节点为null，不需要访问->pop(left1)
->此时left1的右子节点right2不为空，因此需要遍历，遍历过程跟1一样，也是从right2开始一直找到其最内层的左子树

由于当left0访问结束后，需要访问root节点，因此root节点的入栈顺序是在left0之前。

对应的中序遍历代码实现逻辑总结如下
1、从当前root节点开始遍历，先把当前节点入栈
2、如果左子节点不为空，则一直往下遍历左子节点，保存在栈中，直到最后一层左子节点（该节点的左子树为空）
3、弹出当前栈中的节点，并打印该节点（即访问该节点）
4、如果该节点的右子树不为空，开始访问右子树，即重复上述1、2、3，将该节点的
左子树逐层存入栈中，然后再出栈访问。

当栈为空时，所有元素均访问完成
*/

void inorderNoRecursive(TreeNode* root, vector<int>& res) {
    stack<TreeNode*> st;
    TreeNode* p = root;
    while (p != nullptr || !st.empty()) {
        // 一直向下遍历左子树，并将每个左子树的root节点存入栈中
        while (p != nullptr) {
            // res.push_back(p->val);  // 前序遍历时，在这个位置开始访问节点
            st.push(p);
            p = p->left;
        } // 直到左子树节点为null

        if (!st.empty()) {
            p = st.top();
            st.pop();
            res.push_back(p->val); // 访问节点，先访问的必然是左子节点【visit】

            // p指向右子树：在下次循环时会执行代码{173-176}，
            // 即把右子树中的左子树逐层的入栈，然后通过出栈的方式访问
            p = p->right;
        }
    }
}

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
 * @descr: 二叉树的层序遍历
 * @method: BFS
 */
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

      que.pop();  // 弹出队首
    }   // 在每一个for循环后，当前层的结点全部存入到curLevel中, 此时que存放的是下一层的结点
  
    res.push_back(curLevel);
  }

  return res;
}


/**
 * @problem: 111. Minimum Depth of Binary Tree
 * @descr: Given a binary tree, find its minimum depth.
 * The minimum depth is the number of nodes along the shortest path from 
 * the root node down to the nearest leaf node.
 * @method: DFS: 递归方式求二叉树的最小深度
 */
int minDepth(TreeNode* root) {
  if (root == nullptr) {
    return 0;
  }
  if (root->left == nullptr) {  // 左子树为空, 则叶子节点只可能出现在右子树
    return minDepth(root->right) + 1;
  }
  if (root->right == nullptr) {
    return minDepth(root->left) + 1;
  }
  return std::min(minDepth(root->left), minDepth(root->right)) + 1;
}


/**
 * @problem: 104. Maximum Depth of Binary Tree
 * @descr: 二叉树的最大高度
 * @method: 递归
 */
int maxDepth(TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }
    return 1 + max(maxDepth(root->left), maxDepth(root->right));
}


// ====================================================================
int main() {
  cout <<"hello";
  system("pause");
}
