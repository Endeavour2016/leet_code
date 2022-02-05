/*
 * @Author: zlm 
 * @Date: 2022-01-31 16:42:15 
 * @Last Modified by: zlm
 * @Last Modified time: 2022-01-31 17:37:13
 */

/**
 * @problem: 347. Top K Frequent Elements
 * @descr: Given an integer array nums and an integer k, return the k most frequent elements.
 * You may return the answer in any order.
 * @method:
 */

#include "../head_file.h"
using namespace std;

// 方法一：桶排序：根据元素出现的频次将其分配到不同的桶中，用桶的序号表示元素出现的次数。按照序号大小
// 遍历桶，取出总共k个元素即可
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        unordered_map<int, int> data_cnt;
        // 桶序号最大为元素的个数
        vector<vector<int>> bucket_vec;
        bucket_vec.resize(nums.size() + 1);
        vector<int> top_k;
        for (auto& num : nums) {
            auto it = data_cnt.find(num);
            if (it == data_cnt.end()) {
              data_cnt.insert(make_pair(num, 1));
            } else {
              it->second++;
            }
        }

        for (const auto& iter : data_cnt) {
            bucket_vec[iter.second].push_back(iter.first);
        }

        for (int i = nums.size(); i >=0; --i) {
            int cur_bucket_size = bucket_vec[i].size();
            for (int j = 0; j < cur_bucket_size; ++j) {
                top_k.push_back(bucket_vec[i][j]);
                if (top_k.size() == k) {
                    return top_k;
                }
            }
        }
        return top_k;
    }
};

// 方法二：优先级队列
// 统计元素出现的次数，以次数作为比较标准，将元素存入到优先级队列中，构造小顶堆

vector<int> topKFrequent(vector<int>& nums, int k) {
  unordered_map<int, int> data_cnt;
  for (auto num : nums) {
    ++data_cnt[num];
  }

  // if (data_cnt.size() <= k) {
  //   return nums;
  // }

  // 自定义比较函数
  auto cmp = [](const pair<int, int>& lhs, const pair<int, int>& rhs) {
    return lhs.second > rhs.second;
  };

  vector<int> top_k;
  priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> p_queue(cmp);
  // 先存入k个元素
  auto iter = data_cnt.begin();
  for (int idx = 0; iter != data_cnt.end() && idx <= k - 1; ++iter, ++idx) {
    std::pair<int, int> value = {iter->first, iter->second};
    p_queue.push(value);
  }

  // 将剩余元素跟堆顶比较，频次大于堆顶的元素替换当前堆顶
  while (iter != data_cnt.end()) {
    const auto& top_data = p_queue.top();
    if (top_data.second < iter->second) {
      p_queue.pop();
      p_queue.push({iter->first, iter->second});
    }
    ++iter;
  }

  // 取出topK
  while (!p_queue.empty()) {
    const auto& top_data = p_queue.top();
    top_k.push_back(top_data.first);
    p_queue.pop();
  }
  return top_k;
}



int main() {
  vector<int> nums = {1,1,1,2,2,3};
  int k = 2;
  Solution sol;
  vector<int> res;
  res = sol.topKFrequent(nums, k);
  for (auto i : res) {
    cout << i << ", ";
  }
  cout << endl;
  system("pause");
  return 0;
}
