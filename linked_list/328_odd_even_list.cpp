/*
 * @Author: zlm 
 * @Date: 2021-07-10 23:18:07 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-07-11 00:22:11
 */

#include "../head_file.h"
#include "../data_structure.h"
using namespace std;

/**
 * @problem: 328. Odd Even Linked List
 * @descr: 给定一个单链表，把所有的奇数节点和偶数节点分别排在一起
 * 这里的奇数节点和偶数节点指的是节点编号的奇偶性，而不是节点的值的奇偶性
 * 要求空间复杂度应为 O(1), 时间复杂度应为 O(N)
 * 输入: 1->2->3->4->5->NULL
 * 输出: 1->3->5->2->4->NULL
 * @method: 
 */

class Solution {
public:
  ListNode* oddEvenList(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
      return head;
    }
    ListNode* odd_ptr = head;
    ListNode* even_head = head->next;
    ListNode* even_ptr = even_head;
    while (odd_ptr != nullptr && even_ptr != nullptr) {
      if (odd_ptr->next != nullptr && odd_ptr->next->next != nullptr) {
        // 1.先将odd节点串接起来
        odd_ptr->next = odd_ptr->next->next;
        odd_ptr = odd_ptr->next;

        // 2.将even节点串接起来
        if (even_ptr->next != nullptr && even_ptr->next->next != nullptr) {
          even_ptr->next = even_ptr->next->next;
          even_ptr = even_ptr->next;
        }
      } else {  // 执行到这里表明old_optr为最后一个奇数节点，此时even_ptr应该指向最后一个偶数节点
        break;
      }
    }

    // 3.连接奇数节点链和偶数节点链，即奇数链最后一个节点连接偶数链第一个节点
    odd_ptr->next = even_head;
    even_ptr->next = nullptr;
    return head;
  }
};


int main() {
  system ("pause");
  return 0;
}
