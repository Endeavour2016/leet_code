/*
 * @Author: zlm 
 * @Date: 2021-07-09 23:04:48 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-07-10 13:44:48
 */

#include "../head_file.h"
#include "../data_structure.h"
using namespace std;

/**
 * @problem: 92. Reverse Linked List II
 * @descr: 给一个长度为n的单链表以及指定的位置left, right, 要求反转left-right之间的节点
 * 其中 1 <= left <= right <= n
 * @method: 反转链表
 */
class Solution {
 public:
  ListNode* reverseBetween(ListNode* head, int left, int right) {
    // 1. 先找到left和right对应的节点指针
    if (head == nullptr || head->next == nullptr || left == right) {
      return head;
    }

    ListNode* p_left = nullptr;
    ListNode* p_right = nullptr;

    // 分别记录left前、right后的相邻节点
    ListNode* p_left_pre = nullptr;
    ListNode* p_right_next = nullptr;

    ListNode* p_cur = head;
    int cnt = 0;
    while (p_cur != nullptr) {
      ++cnt;
      if (cnt == left) {
        p_left = p_cur;
        break;
      }
      // 还没有找到left
      p_left_pre = p_cur;
      p_cur = p_cur->next;
    }

    p_cur = p_left->next;
    while (p_cur != nullptr) {
      ++cnt;
      if (cnt == right) {
        p_right = p_cur;
        break;
      }
      p_cur = p_cur->next;
    }
    p_right_next = p_right->next;

    // 区间节点反转
    p_right->next = nullptr;
    ReverseList(p_left);  // 反转后 p_right成为head, p_left成为tail

    p_left->next = p_right_next;
    if (p_left_pre == nullptr) {  // left指向head节点
      return p_right;
    } else {
      p_left_pre->next = p_right;
      return head;
    }
  }

 private:
  ListNode* ReverseList(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
      return head;
    }
    ListNode* p_pre = nullptr;
    ListNode* p_cur = head;
    ListNode* p_next = p_cur->next;
    while (p_cur != nullptr && p_cur->next != nullptr) {
      p_cur->next = p_pre;

      p_pre = p_cur;
      p_cur = p_next;
      p_next = p_next->next;
    }
    // 退出循环时理论上p_cur->next = nullptr
    p_cur->next = p_pre;
    return p_cur;
  }
};

int main() {
  system ("pause");
  return 0;
}