/*
 * @Author: zlm 
 * @Date: 2024-06-29
 * @Last Modified by: zlm
 * @Last Modified time: 2024-06-29
 * leet code 中剑指offer相关题目
 */

#include "../head_file.h"
#include "../data_structure.h"
using namespace std;



/**
 * @brief: LCR 143. 子结构判断（medium）
 * 判断tree2是不是tree1的某个字结构
 * 给定两棵二叉树 `tree1` 和 `tree2`，判断 `tree2` 是否根 以 `tree1` 的某个节点为根的子树具有 相同的结构和节点值
 * 注意，空树 不会是以 `tree1` 的某个节点为根的子树具有 相同的结构和节点值
 * @method: 递归。需要仔细考虑边界条件，需要注意的是：子树的节点不能垮层
 */

class Solution {
public:
    bool isSubStructure(TreeNode* A, TreeNode* B) {
      if (B == nullptr || A == nullptr) {
        return false;
      }

      if (A->val != B->val) {
        return isSubStructure(A->left, B) || isSubStructure(A->right, B); 
      } else {
        // 1.找到以当前root, 即A->val为根节点的子结构
        // 注：根节点相同的情况下，不能直接递归判断左子树是否包含B->left，因为有可能存在左子树的左子树包含B->left，
        // 这种情况下子结构根 B->val 就不连续了
        // 需要判断A->left包含B->left，且A->left->val == B->left->val
        bool res_1 = is_conatain_from_root(A->left, B->left) && is_conatain_from_root(A->right, B->right);

        // 2.如果上述没有找到子结构，需要继续在A的左/由子树中查找子结构
        // ex：A = [4 23 45 67 89], B = [4,8,9] 这种情况下就没法找到以root节点4为根节点的子结构
        // 只能在左子树中找到子结构，即倒数第二层的4，跟最后一层的8 9
        if (res_1) {
          return true;
        }

        return isSubStructure(A->left, B) || isSubStructure(A->right, B);
      }
    }

private:
    // 判断A包含B，且A和B的根节点相同
    bool is_conatain_from_root(TreeNode* A, TreeNode* B) {
      if (B == nullptr) {
        return true;
      }
      if (A == nullptr) {
        return false;
      }
      if (A->val != B->val) {
        return false;
      }

      return is_conatain_from_root(A->left, B->left) && is_conatain_from_root(A->right, B->right);
    }
};


/**
 * @brief: LCR 010. 和为 K 的子数组
 * 给定一个整数数组和一个整数 k ，请找到该数组中和为 k 的连续子数组的个数。
 * @method: hash，前缀和
 */
// 示例 1：
// 输入:nums = [1,1,1], k = 2
// 输出: 2
// 解释: 此题 [1,1] 与 [1,1] 为两种不同的情况

// 方法一: 对于数组中的每个元素 nums[idx]，计算出以 idx 为结尾的 和为k 的连续子数组个数
int subarraySum(vector<int>& nums, int k) {
  int count = 0;
  for (int idx = 0; idx < nums.size(); ++idx) {
    // 计算以 nums[idx] 为结尾的子数组
    int sum = 0;
    for (int end = idx; end >= 0; --end) {
      sum += nums[end];
      if (sum == k) {
        count++;
      }
    }
  }
  
  return count;
}

// 方法二：借助hash表优化时间复杂度
// 定义前缀和：pre[i] 为 [0..i] 里所有数的和，则 pre[i] 可以由 pre[i−1] 递推而来，即：
// pre[i] = pre[i−1] + nums[i]
// 那么「[j..i] 这个子数组和为 k 」这个条件我们可以转化为

// pre[i] − pre[j−1] == k
// 简单移项可得符合条件的下标 j 需要满足
// pre[j−1] == pre[i] − k
// 所以我们考虑以 i 结尾的和为 k 的连续子数组个数时，只要统计有多少个满足前缀和为 pre[i]−k 的 pre[j] 即可。

// 因此问题转化成了“求某个前缀和的”出现次数
// 建立哈希表 mp，以前缀和为key，出现次数为value，记录 pre[i] 出现的次数，从左往右边更新 mp 边计算答案
// 那么以 i 结尾的答案 mp[pre[i]−k] 即可在 O(1) 时间内得到。
int subarraySum(vector<int>& nums, int k) {
    unordered_map<int, int> mp;
    // 前缀和为0的先记为1
    mp[0] = 1;
    int count = 0, pre_sum = 0;
    for (auto& x : nums) {
        pre_sum += x;
        if (mp.find(pre_sum - k) != mp.end()) {
          count += mp[pre_sum - k];
        }
        mp[pre_sum]++;  // pre_sum不存在时，对应的value默认值为0
    }
    return count;
}
