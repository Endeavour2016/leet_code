/*
 * @Author: zlm 
 * @Date: 2021-07-10 22:06:31 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-07-10 23:00:06
 */
#include "../head_file.h"
#include "../data_structure.h"
using namespace std;

/**
 * @problem: 448. Find All Numbers Disappeared in an Array
 * @descr: 给一个长度为n的数组，其中每个元素的取值为[1~n]的整数, 求出求[1~n]中没有出现在数组中的整数
 * @method: 可以利用数组模拟n个桶(编号为1~n), 对于数组中出现的整数k,将对应编号的桶的值设置为1(
 * 或者其它值); 最后遍历所有的桶，没有被设置过值的桶的编号即为数组中没有出现过的数
 */
vector<int> findDisappearedNumbers(vector<int>& nums) {
  vector<int> res;
  vector<int> bucket(nums.size(), 0);  // 桶的初始值为0
  for (const int& num : nums) {
    int pos = num - 1;  // 下标从0开始
    if (bucket[pos] == 0) {
      bucket[pos] = 1;
    }
  }

  // 检查桶的状态
  for (int i = 0; i < nums.size(); ++i) {
    if (bucket[i] == 0) {  // 表示当前桶对应的整数 i+1 没有出现过
      res.push_back(i + 1);
    }
  }
  return res;
}

int main() {
  vector<int> nums = {4,3,2,7,8,2,3,1};
  vector<int> res = findDisappearedNumbers(nums);
  for (auto& d : res) {
    cout << d << "\n";
  }

  system ("pause");
  return 0;
}
