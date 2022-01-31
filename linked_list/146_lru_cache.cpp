/*
 * @Author: zlm 
 * @Date: 2021-10-24 20:16:19 
 * @Last Modified by:   zlm 
 * @Last Modified time: 2021-10-24 20:16:19 
 */

#include "../head_file.h"
#include "../data_structure.h"
using namespace std;

/**
 * @problem: 146. LRU Cache
 * @descr:实现 LRUCache 类：以正整数作为容量 capacity 初始化 LRU 缓存，实现get和put操作
 * 要求在 O(1) 时间复杂度内完成这两种操作
 * @method: 双向链表+哈希表
 */
class LRUCache {
public:
    using list_iterator = list<pair<int, int> >::iterator;
    LRUCache(int capacity) {
        capacity_ = capacity;
    }
    
    int get(int key) {
        auto mp_iter = key_map_.find(key);
        if (mp_iter == key_map_.end()) {
            return -1;
        }
        list_iterator list_iter = mp_iter->second;
        // 当期访问过的记录，移动到列表头部
        data_list_.splice(data_list_.begin(), data_list_, list_iter);
        return mp_iter->second->second;

    }
  
    void put(int key, int value) {
        auto iter = key_map_.find(key);
        if (iter != key_map_.end()) {
          list_iterator list_iter = iter->second;
          // update value
          list_iter->second = value;
          // move current value to list top
          data_list_.splice(data_list_.begin(), data_list_, list_iter);
        } else {
          // 如果cache已满，先删除部分元素以腾出空间
          if (key_map_.size() == capacity_) {
              pair<int, int>& tail_elem = data_list_.back();
              key_map_.erase(tail_elem.first);
              data_list_.pop_back();  // 删除最久未使用的记录
          }
          pair<int, int> new_record(key, value);
          data_list_.push_front(new_record);
          key_map_.insert(make_pair(key, data_list_.begin()));
        }
    }

private:
    std::list<pair<int, int>> data_list_;
    // 记录key-value映射关系，实现快速访问
    unordered_map<int, list_iterator> key_map_;
    int capacity_ = 0;
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */

int main() {
    LRUCache* obj = new LRUCache(10);
    obj->put(1, 111);
    int val = obj->get(2);
    cout << "val: " << val << endl;

    system("pause");
    return 0;
}
