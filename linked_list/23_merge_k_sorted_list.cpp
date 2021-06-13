/**
 * @Author: zlm 
 * @Date: 2021-06-12 16:31:09 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-06-12 23:43:15
 */

#include "../head_file.h"
using namespace std;

/**
 * @problem: 23. Merge k Sorted Lists
 * @description: You are given an array of k linked-lists lists, 
 * each linked-list is sorted in ascending order.
 * Merge all the linked-lists into one sorted linked-list and return it.
 * @method: 
 * 方法一：借助优先级队列：1、把每个链表的头节点（最小元素）放入优先级队列，
 * 取出最小堆顶，即所有链表的最小节点 2、将堆顶所在链表的下一个节点（该链表的第二小的节点）加入
 * 优先队列，此时的堆顶即为所有链表中第二小的节点 3、重复步骤1和2，直到所有节点都出队
 */

struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// 仿函数
struct NodeCmp{
  bool operator()(ListNode* lhs, ListNode* rhs) {
    return lhs->val > rhs->val;
  }
};

class Solution {
public:
  ListNode* mergeKLists(vector<ListNode*>& lists) {
    if (lists.size() == 0) {
      return nullptr;
    }
    // 小顶堆
    std::priority_queue<ListNode*, vector<ListNode*>, NodeCmp> min_heap;
    for (auto list_node : lists) {
      if (list_node != nullptr) {
        min_heap.push(list_node);
      }
    }

    ListNode sorted_list_head;
    ListNode* node_ptr = &sorted_list_head;
    while (!min_heap.empty()) {
      ListNode* top_node = min_heap.top();  // 取出当前最小节点
      min_heap.pop();

      node_ptr->next = top_node;
      node_ptr = node_ptr->next;
      // top_node->next入堆，取第二小的节点
      if (top_node->next != nullptr) {
        min_heap.push(top_node->next);
      }
    }
    return sorted_list_head.next;
  }
};

int main() {
  return 0;
}