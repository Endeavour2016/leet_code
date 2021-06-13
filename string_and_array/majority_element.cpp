/*
 * @Author: zlm 
 * @Date: 2021-02-27 10:09:18 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-02-27 10:13:31
 */

#include <iostream>
#include <climits>
#include <vector>
#include <string>

using namespace std;

// 229. Majority Element II
// 题目: 给一个整数数组，输出其中出现次数超过1/3的元素，要求时间复杂度O(n)，空间复杂度O(1)
// 要找大于三分之一的数字，那么这样的数最多有2个
// 参考讲解：https://zhuanlan.zhihu.com/p/107433404

// 思路: 
// 1、用num1、num2表示复合条件的两个候选元素，cnt1、cnt2分别表示对应元素的计数值
// 2、遍历其他元素，如果当期元素等于其中某一个候选元素，则对应计数值+1，如果候选元素计数变为0，则将
// 该候选元素更新为当期元素。
// 3、如果当期元素与两个候选
// 4、代码实现时，可以先在数组元素中找两个不相等的元素赋值给num1、num2。
// 
// 原理：假定两个候选元素符合条件，则剩下的元素个数一定小于1/3，因此按照上述方法
// 将候选元素跟剩下元素成对剔除时，候选元素最终的计数一定会大于0。
// 由于在遍历过程中，候选元素会动态更新，最终剩下的候选元素不能判定一定满足条件，
// 因为有可能只有一个元素的数量超过1/3

class Solution {
public:
    vector<int> majorityElement(vector<int>& nums) {
        vector<int> res;
        int num1 = INT_MAX, num2 = INT_MAX;
        int cnt1 = 0, cnt2 = 0;
        for (int d : nums) {
            if (d == num1) {
                cnt1++;
            } else if (d == num2) {
                cnt2++;
            } else if (cnt1 == 0) {
                cnt1 = 1;
                num1 = d;  // update candidate num
            } else if (cnt2 == 0) {
                cnt2 = 1;
                num2 = d;
            } else {  // 与候选元素均不相同,计数减一
                cnt1--;
                cnt2--;
            }
        }

        // 第一次遍历，虽然确定了后续元素，但是不能保证均符合条件
        cnt1 = 0;
        cnt2 = 0;
        for (int d : nums) {
            if (d == num1) {
                cnt1++;
            } else if (d == num2) {
                cnt2++;
            }
        }

        if (cnt1 > nums.size() / 3) {
            res.push_back(num1);
        }
        if (cnt2 > nums.size() / 3) {
            res.push_back(num2);
        }
        return res;
    }
};


int main() {
  Solution solution;
  vector<int> nums = {3,2,3};
  auto result = solution.majorityElement(nums);
  for (auto d : result) {
     cout << d << endl;
  }
  system("pause");
  return 0;
}