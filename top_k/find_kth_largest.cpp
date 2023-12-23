/*
 * @Author: zlm 
 * @Date: 2022-02-06 10:42:50 
 * @Last Modified by: zlm
 * @Last Modified time: 2022-02-06 10:47:12
 */

#include "../head_file.h"
#include "../data_structure.h"
using namespace std;


/**
 * @problem: 215. 数组中的第K个最大元素
 * @descr: 给定整数数组 nums 和整数 k，请返回数组中第 k 个最大的元素
 * 请注意，你需要找的是数组排序后的第 k 个最大的元素，而不是第 k 个不同的元素
 * @method: 小顶堆, priority_queue
 * 使用前 K 个元素构造一个小顶堆，然后依次把后面的元素与堆顶对比，若大于堆顶，则替换之，并重新调整堆
 * 最终堆顶元素为第k大的元素
 * 这里使用 优先队列(设置比较类型 greater<int>, 使得队首top最小)
 */
class Solution {
public:
  int findKthLargest(vector<int>& nums, int k) {
    if (k <= 0 || k > nums.size() || nums.empty()) {
      return 0;
    }
    priority_queue<int, vector<int>, greater<int>> que;
    for (int i = 0; i < nums.size(); i++) {
      que.push(nums[i]);
      if (que.size() == k+1) {  // 队首始终为第k+1大的元素，故需要出队
          que.pop();
      }
    }
    return que.top();
  }
};


int main() {
  system("pause");
  return 0;
}
