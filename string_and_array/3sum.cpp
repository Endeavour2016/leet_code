/**
 * @Author: zlm 
 * @Date: 2024-01-20
 * @Last Modified by: zlm
 * @Last Modified time: 2024-01-20
 */

#include "../head_file.h"
using namespace std;

/**
 * @problem: 15. 3Sum
 * @descr: Given an integer array nums, return all the triplets [nums[i], nums[j], nums[k]] 
 * such that i != j, i != k, and j != k, and nums[i] + nums[j] + nums[k] == 0.
 * Notice that the solution set must not contain duplicate triplets.
 * 给一个数组，从中找到三个数，三数之和为0。要求返回所有的三元组，且之间不重复。
 * 比如[0,1,-1]和[0, -1, 1]算作重复
 * @flag: 双指针左右夹逼，two sum
 */

// method1: 先确定第一个数a，剩下两个数按照two sum解法进行（左右夹逼）；
// 由于三元组之间的数字不能重复，因此先对数组排序，保证相同的数字相邻，方便遍历过程中去重
// 时间复杂度：O(nlogn + O(n^2))
vector<vector<int>> threeSum(vector<int>& nums) {
    vector<vector<int>> res;
    if (nums.size() <= 2) {
        return res;
    }

    int n = nums.size();
    // 1、先排序
    std::sort(nums.begin(), nums.end());

    // 2、for循环先确定三元组中的第一个数
    for (int i = 0; i < n - 2; i++) {
        int target = 0 - nums[i];
        
        // 2sum问题，由于数组有序，可以采用for循环左右逼近
        int left = i + 1, right = n - 1;
        while (left < right) {
            int sum = nums[left] + nums[right];
            if (sum < target) {
                left++;
            } else if (sum > target) {
                right--;
            } else {
                res.push_back(vector<int>{nums[i], nums[left], nums[right]});
                // 跳过重复元素
                while (left < right && nums[left] == nums[left + 1]) {
                    ++left;
                }
                ++left;

                while (left < right && nums[right] == nums[right - 1]) {
                    --right;
                }
                --right;
            }
        }
        // 本次while循环结束后，需要跳过重复元素
        while (i < n - 2 && nums[i] == nums[i + 1]) {
            i++;
        }
    }  // end for

    return res;
}


// method2: 三重循环+双指针法减少遍历次数
// 时间复杂度：排序的O(nlogn) + 遍历O(N^2) 
// ex: [1, 1, 1, -2, -2, -3, 4, xxx]
vector<vector<int>> threeSum(vector<int>& nums) {
    std::vector<std::vector<int> > res;
    int count = nums.size();
    if (count == 0) {
        return res;
    }

    // 1.先排序：保证后续遍历得到的三元组内部内部有序，用于三元组之间去重
    std::sort(nums.begin(), nums.end());
    // 2.1first loop
    for (int first = 0; first < count; ++first) {
        // 先检查相邻元素是否重复，重复的元素没必要处理
        if (first != 0 && nums[first] == nums[first - 1]) {
            continue;
        }

        // 2.2 second loop
        int third = count-1;
        for (int second = first + 1; second < count; ++second) {
            if (second != first +1 && nums[second] == nums[second - 1]) {
                continue;
            }
            // 走到这一步，可以先固定前两个数：a+b，假设第三个数记为c
            // 第三个数如果从second+1开始遍历，则遍历次数为 n-1-second，这意味着2和3重循环总共遍历n^2次
            // 由于b--->c升序排列，因此可以考虑c从尾部遍历
            // 2.3 third loop
            while (third > second && nums[first] + nums[second] + nums[third] > 0) {
                third--;
            }
            if (third <= second) {
                // 说明没有找到满足条件的c，使得 a+b+c <= 0
                // 下一次遍历到b'时，由于b'>=b，c'>=b'因此 a+b'+c'必然 >0，因此没必要再继续遍历b'及其之后的值
                break;
            } 
    
            if (nums[first] + nums[second] + nums[third] == 0) {
                // 此时说明找到了 a+b+c = 0
                // second loop下一次遍历到b'时，由于b' >= b, 因此对应的c'必然<=c，此时c'不需要从数组尾部遍历
                // 而是直接从上一次c元素对应的位置开始向前扫描, 即当前third的值不变，second loop的下一次扫描直接用
                std::vector<int> temp = {nums[first], nums[second], nums[third]};
                res.emplace_back(std::move(temp));

                // 元素c可能有重复元素，将c的下标遍历到重复元素最左侧的位置
                while (third > second && nums[third] == nums[third - 1]) {
                    third--;
                }
            }
        }  // second for
    }  // first for

    return res;
}

int main() {
  return 0;
}


