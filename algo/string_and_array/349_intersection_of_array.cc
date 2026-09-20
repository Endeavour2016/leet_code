/*
 * @Author: zlm 
 * @Date: 2021-07-25 10:02:23 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-07-25 10:27:45
 */


#include "../head_file.h"
#include "../data_structure.h"
using namespace std;

/**
 * @problem: 349. Intersection of Two Arrays (Easy)
 * @descr: Given two integer arrays nums1 and nums2, return an array of their intersection.
 * Each element in the result must be unique and you may return the result in any order.
 * 给两个数组，求出其交集，返回结果中相同元素只出现一次
 * @method: 先把第一个数组存入map: key为数组元素，value为bool类型，表示该元素是否为
 * 交集且已经被取出（保证输出结果去重）；数组2中的元素依次判断是否在字典中存在
 */

class Solution {
public:
  vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
    if (nums1.size() == 0 || nums2.size() == 0) {
      return {};
    }
    vector<int> res;
    map<int, bool> assist_map;
    for (auto num : nums1) {
      assist_map.insert(make_pair(num, false));
    }

    for (auto num : nums2) {
      auto iter = assist_map.find(num);
      // 字典中出现该元素且该元素尚未被取出
      if (iter != assist_map.end() && !iter->second) {
        res.push_back(iter->first);
        iter->second = true;
      }
    }

    return res;
  }
};


int main() {
  system ("pause");
  return 0;
}
