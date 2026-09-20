/**
 * @Author: zlm 
 * @Date: 2024-01-20
 * @Last Modified by: zlm
 * @Last Modified time: 2024-01-20
 */

#include "../head_file.h"
using namespace std;

/**
 * @problem: 239. Sliding Window Maximum(hard)
 * @descr: You are given an array of integers nums, there is a sliding window of size k which
 * is moving from the very left of the array to the very right.
 * You can only see the k numbers in the window.
 * Each time the sliding window moves right by one position.
 * Return the max sliding window.
 * @method: 双端队列
 * @时间复杂度：O(n)
 * @空间复杂度：O(k)
 */

// 利用双端队列，通过一系列入队和出队操作计算滑动窗口最大值
// 1. 数组元素从i=0开始入队，若新入队的元素大于前一个元素（即当前队尾），则把队尾元素剔除，因为此时队尾元素不可能为窗口的最大值；
// 2. 若新入队元素小于当前队尾，则保留队尾，同时新元素入队（因为随着窗口的移动，该元素有可能成为某个窗口最大值）
// 3. 随着新元素入队，要及时判断队头元素是否在当前窗口内：通过下标差是否 >=k 来判断，若不在窗口内，则将队头出列；否则暂时保留！
// 4. 通过上述操作，队头始终为当前窗口的最大值
// 这里的队列存放的是元素的下标index
vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    // border cases 边界情况
    if (nums.empty() || k > nums.size() || k <= 0)
        return vector<int>();

    deque<int> dq; // 队列中存放的下标
    vector<int> res;

    int n = nums.size();
    // 先将前 k 个元素入队：这是第一个窗口
    // 举例：先入队2 3 4， 队列中的元素为：4(2,3小于4，被剔除)
    int i = 0;
    for (; i < k; i++) {
        // 新入队元素大于队尾，剔除队尾
        while (!dq.empty() && nums[i] >= nums[dq.back()]) {
            dq.pop_back();
        }

        dq.push_back(i);
    } // 第一个窗口元素经过入队操作后，在队列中只剩下最大元素 4

    res.push_back(nums[dq.front()]);

    // 对于下标i=size~n-1的元素，每个元素都是对应窗口的最后一个元素
    // 除第一个窗口，剩下窗口个数为 n-k
    for (int i = k; i < n; i++) {
        // 首先判断当前队头元素是否处在当前窗口内：当前窗口对应的数据下标 [i-(k-1) ~ i]
        if (i - dq.front() >= k) {
            dq.pop_front();
        }

        while (!dq.empty() && nums[i] >= nums[dq.back()]) {
            dq.pop_back();
        }

        dq.push_back(i);

        // 队头为当前窗口最大值
        res.push_back(nums[dq.front()]);
    }  // end for

    return res;
}

