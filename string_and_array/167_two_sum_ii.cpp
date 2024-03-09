/**
 * @Author: zlm 
 * @Date: 2024-01-14
 * @Last Modified by: zlm
 * @Last Modified time: 2024-01-14
 */

#include "../head_file.h"
using namespace std;

/**
 * @problem: 167. Two Sum II - Input Array Is Sorted
 * @descr: Given a 1-indexed array of integers numbers that is already sorted
 * in non-decreasing order, find two numbers such that they add up to a specific target number.
 * Let these two numbers be numbers[index1] and numbers[index2] where 1 <= index1 < index2 <= numbers.length.
 * Return the indices of the two numbers, index1 and index2,
 * added by one as an integer array [index1, index2] of length 2.
 * @method: 双指针法：从数组两头开始夹逼
 */

vector<int> twoSum(vector<int>& numbers, int target) {
    std::vector<int> index;
    if (numbers.empty()) {
        return index;
    }
    int left = 0;
    int right = numbers.size() - 1;
    while (left < right) {
        int sum = numbers[left] + numbers[right];
        if (sum == target) {
            index.push_back(left + 1);  // 题目要求下标从1开始
            index.push_back(right + 1);
            break;
        } else if (sum < target) {  // 此时需要找一个更大的数，由于right已经最大，只能从左侧寻找
            left++;
        } else {  // sum > target, 需要找一个更小的数，由于left对应的数已经最小，只能从最右侧寻找
            right--;
        }
    }
    return index;
}

int main() {
  return 0;
}


