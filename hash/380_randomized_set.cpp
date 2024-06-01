/**
 * @Author: zlm 
 * @Date: 2024-01-14
 * @Last Modified by: zlm
 * @Last Modified time: 2024-01-14
 */

#include "../head_file.h"
#include <cstdlib>
using namespace std;

/**
 * @problem: 380. O(1) 时间插入、删除和获取随机元素
 * @descr: 实现 RandomizedSet 类：
 * RandomizedSet() 初始化 RandomizedSet 对象
 * bool insert(int val) 当元素 val 不存在时，向集合中插入该项，并返回 true；否则，返回 false
 * bool remove(int val) 当元素 val 存在时，从集合中移除该项，并返回 true；否则，返回 false
 * int getRandom() 随机返回现有集合中的一项（测试用例保证调用此方法时集合中至少存在一个元素）。每个元素应该有相同的概率被返回。
 * 你必须实现类的所有函数，并满足每个函数的 平均时间复杂度为 O(1) 。
 * @method: 数组+hash map
 */

class RandomizedSet {
public:
    RandomizedSet() {}
    
    bool insert(int val) {
        if (num_2_index_.count(val) > 0) {
            return false;
        }
        nums_.push_back(val);
        num_2_index_[val] = nums_.size() - 1;
        return true;
    }
    
    bool remove(int val) {
        if (num_2_index_.count(val) == 0) {
            return false;
        }
        // 把数组最后一个元素拷贝到val所在的位置，通过覆盖的方式删除val
        // 同时需要把数组最后一个元素删除
        int val_index = num_2_index_[val];
        nums_[val_index] = nums_.back();
        // update index of the last element
        num_2_index_[nums_.back()] = val_index;

        num_2_index_.erase(val);
        nums_.pop_back();
        return true;
    }
    
    int getRandom() {
        std::srand((unsigned)std::time(nullptr));
        return nums_[rand() % nums_.size()];
    }

private:
    std::vector<int> nums_;
    std::unordered_map<int, int> num_2_index_;
};

/**
 * Your RandomizedSet object will be instantiated and called as such:
 * RandomizedSet* obj = new RandomizedSet();
 * bool param_1 = obj->insert(val);
 * bool param_2 = obj->remove(val);
 * int param_3 = obj->getRandom();
 */

int main() {
  return 0;
}


