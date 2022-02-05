/*
 * @Author: zlm 
 * @Date: 2021-07-25 00:45:49 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-07-25 01:15:54
 */

#include "../head_file.h"
#include "../data_structure.h"
using namespace std;

/**
 * @problem: 面试题 17.14. 最小的K个数
 * @descr: 设计一个算法，找出数组中最小的k个数，以任意顺序返回这k个数均可
 * https://leetcode-cn.com/problems/smallest-k-lcci/
 * @method: 典型解法：大顶堆
 */

// 方法一：排序，然后直接取前k个元素
// 时间复杂度：O(nlogn)，其中 n 是数组 arr 的长度。算法的时间复杂度即排序的时间复杂度
// 空间复杂度：O(logn)，排序所需额外的空间复杂度为 O(logn)。

vector<int> smallestK(vector<int>& arr, int k) {
  vector<int> res(k, 0);
  std::sort(arr.begin(), arr.end());
  for (int i = 0; i < k; ++i) {
    res[i] = arr[i];
  }
  return res;
}

// 方法二：维护一个包含k个元素的大顶堆，对于剩下的n-1个元素，如果比堆顶小，则弹出堆顶，并插入该
// 元素后重新调整堆
class Solution {
public:
  vector<int> smallestK(vector<int>& arr, int k) {
    if (k <= 0) {
      return {};
    }
    int len = arr.size();
    vector<int> res;
    res.reserve(k);

    std::priority_queue<int> max_queue;
    // 先存入k个元素，建立max heap
    for (int i = 0; i < k; ++i) {
      max_queue.push(arr[i]);
    }
    for (int i = k; i < len; ++i) {
      if (max_queue.top() > arr[i]) {
        max_queue.pop();
        max_queue.push(arr[i]);
      }
    }

    while (!max_queue.empty()) {
      res.push_back(max_queue.top());
      max_queue.pop();
    }

    return res;
  }
};

int main() {
  system ("pause");
  return 0;
}
