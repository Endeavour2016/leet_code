/*
 * @Author: zlm 
 * @Date: 2022-02-01 18:53:31 
 * @Last Modified by: zlm
 * @Last Modified time: 2022-02-01 18:55:33
 */

#include "../head_file.h"
#include "../data_structure.h"
using namespace std;

/**
 * @problem: 21. Merge Two Sorted Lists(Easy)
 * @method:
 * 设置一个头结点 Head 用来指向合并后的链表的第一个元素
 * 遍历比较各个节点的大小，较小的节点接在ptr的后面
 */
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        if (l1 == nullptr)
            return l2;
        if (l2 == nullptr)
            return l1;
        
        ListNode *head = new ListNode(0);
        ListNode *ptr = head;
        while (l1 != nullptr && l2 != nullptr) {
            if (l1->val < l2->val) {
                ptr->next = l1;
                ptr = ptr->next;
                l1 = l1->next;
            } else {
                ptr->next = l2;
                ptr = ptr->next;
                l2 = l2->next;
            }
        }   // end while 把剩余的链表接上

        if (l1 != nullptr)
            ptr->next = l1;
        if (l2 != nullptr)
            ptr->next = l2;
        
       return head->next;
    }
};

int main() {
  system("pause");
  return 0;
}
