
/**
 * @Author: zlm 
 * @Date: 2024-01-21
 * @Last Modified by: zlm
 * @Last Modified time: 2024-01-21
 */

#include "../head_file.h"


/**
 * @problem: 83. Remove Duplicates from Sorted List
 * @descr: 删除有序链表中的重复元素，比如 1-->1-->2-->3-->3-->4  =====> 1->2->3->4
 * @method: 快慢指针：pre/pcur，两个指针处于相邻位置，同时向后扫描，遇到重复节点时删除pcur指向的节点
 * 然后继续快指针pcur扫描下一个节点
 * @时间复杂度：O(n)
 * @空间复杂度：O(1)
 */

struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        if (nullptr == head) {
            return nullptr;
        }

        ListNode* pre = head;
        ListNode* pcur = pre->next;
        while (pcur != nullptr) {
            if (pcur->val == pre->val) {
                // delete cur node
                pre->next = pcur->next;

                ListNode* temp = pcur;
                pcur = pcur->next;
                delete temp;
            } else {
                pre = pcur;
                pcur = pcur->next;
            }
        }
        return head;
    }
};

/**
 * @problem: 82. Remove Duplicates from Sorted List II
 * @descr: Given the head of a sorted linked list, delete all nodes that have duplicate numbers,
 * leaving only distinct numbers from the original list. Return the linked list sorted as well.
 * 删除有序链表中的所有重复元素，注意这里跟83题不一样的是重复元素会全部删除，比如 1-->1-->2-->3-->3-->4 ===> 2-->4
 * 而在83题中重复元素会保留一个：1-->2-->3--4
 * @method: 快慢指针：pre/pcur/pnext，遇到重复节点时删除所有的重复节点，然后移动pre和pcur/pnext
 * @时间复杂度：O(n)
 * @空间复杂度：O(1)
 */
ListNode* deleteDuplicates82(ListNode* head) {
    if (nullptr == head || head->next == nullptr)  {
        return head;
    }
    ListNode* new_head = nullptr;
    ListNode* pre = nullptr;
    ListNode* pcur = head;
    ListNode* pnext = pcur->next;
    while (pcur != nullptr && pnext != nullptr) {
        if (pnext->val == pcur->val) {
            // 删除后面重复的元素，直到遇到下一个不重复元素
            while (pnext != nullptr && pnext->val == pcur->val) {
                ListNode* temp = pnext;
                pnext = pnext->next;
                delete temp;
            }
            // 删除pcur
            delete pcur;
            pcur = pnext;
            if (pnext != nullptr) {
                pnext = pnext->next;
            }
            if (pre != nullptr) {
                pre->next = pcur;
            }
            if (pnext == nullptr && pre == nullptr) {
                // 说明此时已经处理了所有节点且pcur之前的节点均被删除了
                // 此时pcur即为新的头节点
                new_head = pcur;
                break;
            }
        } else {
            // pre pcur pnext三个指针向后扫描
            if (pre == nullptr) {
                // 表明此时找到了第一个不重复的节点pcur，即最终的头节点
                new_head = pcur;
            }
            pre = pcur;
            pcur = pnext;
            pnext = pnext->next;
        }
    }
    return new_head;
}


// 更容易理解的写法
// 通过一个指针变量pcur遍历链表, 遍历过程中：
//  1.如果遇到重复元素，即pcur->val == pcur->next->val：
//     a.先把当前所有重复元素删除，直到遇到下一个不同的元素，然后重复步骤1
//  2.若遇到不重复元素，即pcur->val != pcur->next->val
//     a.记录新的头节点，也就是第一次遇到的不重复元素（这里可以根据新的头节点是否赋过值来判断是否为第一次）
//  3.边界case：如果pcur->next==nullptr，意味着已经处理完所有节点了，break退出循环即可
//  注：处理前先判断head为nullptr或者只有单节点的case
ListNode* deleteDuplicatesUpdate(ListNode* head) {
    if (nullptr == head) {
        return nullptr;
    }
    ListNode* new_head = nullptr;
    ListNode* pre = nullptr;  // 表示pcur的前驱街道
    ListNode* pcur = head;
    while (pcur != nullptr) {
        if (pcur->next == nullptr) {
            if (new_head == nullptr) {  // 说明此时还没有确定头节点，则意味着pcur为新的头节点
               new_head = pcur;
            }
            break;  // next==null的判断也可以直接放在while后的条件表达式中
        }

        if (pcur->val == pcur->next->val) {
            // 删除重复的元素，直到遇到下一个不重复元素
            while (pcur->val == pcur->next->val) {
                ListNode* nxt = pcur->next;
                delete pcur;
                pcur = nxt;

                if (pcur->next == nullptr) {
                    break;
                }
            }
            //  此时pcur指向当前重复节点的最后一个，也要删除
            ListNode* nxt = pcur->next;
            delete pcur;
            pcur = nxt;
            
            if (pre != nullptr) {
                pre->next = pcur;
            }
        } else {
            if (new_head == nullptr) {
                // 表明此时找到了第一个不重复的节点，也就是最终的头节点
                new_head = pcur;
            }
            pre = pcur;
            pcur = pcur->next;
        }
    }
    return new_head;
}


int main() {
    return 0;
}