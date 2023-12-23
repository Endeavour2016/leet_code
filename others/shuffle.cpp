#include "head_file.h"

using namespace std;

// shuffle an array: 2019-3-17
// 洗牌算法，可以使用 STL 里的 random_shuffle
class Solution {
private:
    vector<int> origin;
public:
    Solution(vector<int> nums) : origin(nums) {}
    
    /** Resets the array to its original configuration and return it. */
    vector<int> reset() {
        return origin;
    }
    
    /** Returns a random shuffling of the array. */
    vector<int> shuffle() {
        vector<int> res = origin;
        random_shuffle(res.begin(), res.end());
        return res;
    }
};

/**
 * Your Solution object will be instantiated and called as such:
 * Solution obj = new Solution(nums);
 * vector<int> param_1 = obj.reset();
 * vector<int> param_2 = obj.shuffle();
 */