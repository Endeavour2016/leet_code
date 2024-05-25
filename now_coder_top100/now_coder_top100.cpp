/*
 * @Author: zlm 
 * @Date: 2024-04-27
 * @Last Modified by: zlm
 * @Last Modified time: 2024-04-27
 */

// 牛客网中top 100算法题

#include "../head_file.h"
using namespace std;

/**
 * @brief: BM52 数组中只出现一次的两个数字
 * 一个整型数组里除了两个数字只出现一次，其他的数字都出现了两次。请写程序找出这两个只出现一次的数字。
 * @method: 要求时间复杂度O(N), 空间复杂度O(1)
 */
vector<int> FindNumsAppearOnce(vector<int>& nums) {
    // write code here
    // write code here
    int xor_res = 0;
    for (int32_t idx = 0; idx < nums.size(); ++idx) {
        xor_res ^= nums[idx];
    }
    // 若a 和 b不相同，则a^b != 0
    // 因此只需要从结果中找到第一个不为0的位
    // 以该位为基准将数组分成两组，每一组内的元素单独做异或运算
    int k = 1;
    while ((k & xor_res) == 0) {
        k <<= 1;
    }
    int res_1 = 0, res_2 = 0;
    for (int32_t idx = 0; idx < nums.size(); ++idx) {
        if ((k & nums[idx]) == 0) {
            res_1 ^= nums[idx];
        } else {
            res_2 ^= nums[idx];
        }
    }
    
    if (res_1 <= res_2) {
        return {res_1, res_2};
    } else {
        return {res_2, res_1};
    }
}
