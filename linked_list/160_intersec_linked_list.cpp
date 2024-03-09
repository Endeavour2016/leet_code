/**
 * @Author: zlm 
 * @Date: 2024-01-20
 * @Last Modified by: zlm
 * @Last Modified time: 2024-01-20
 */

#include "../head_file.h"
using namespace std;

/**
 * @problem: 160. Intersection of Two Linked Lists
 * @descr: Given the heads of two singly linked-lists headA and headB,
 * return the node at which the two lists intersect. If the two linked lists have no intersection at all,
 * return null.
 * @method: 直接用hash表存放节点的指针，遇到重复的说明有交点，返回对应指针即可
 * @时间复杂度：O(n+m), 空间O(n)或者O(m)
 */


 // Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
 };

// 方法二：
// 1.计算交点之前两个链表的节点数量之差（实际上就是两个链表长度之差），比如A链表长度为n，B链表长度为m，且n <= m
// 2.将B链表头指针headB向后移动m-n个节点
// 3.同时移动headB和headA，直到两个指针相同，否则说明链表不相交
int GetLength(ListNode *head) {
    int len = 0;
    while (head != nullptr) {
        len++;
        head = head->next;
    }
    return len;
}

ListNode *getIntersectionNode2(ListNode *headA, ListNode *headB) {
    if (nullptr == headA || nullptr == headB) {
        return nullptr;
    }
    int list_a_len = GetLength(headA);
    int list_b_len = GetLength(headB);
    if (list_a_len <= list_b_len) {  // B链表更长
        for (int idx = 0; idx < list_b_len - list_a_len && headB != nullptr; ++idx) {
            headB = headB->next;
        }
    } else {  // A链表更长
        for (int idx = 0; idx < list_a_len - list_b_len && headA != nullptr; ++idx) {
            headA = headA->next;
        }
    }

    // AB链表同时扫描
    while (headA != nullptr && headB != nullptr) {
        if (headA == headB) {
            return headA;
        }
        headA = headA->next;
        headB = headB->next;
    }

    return nullptr;
}


// 方法一：直接借助hash set
ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
    if (nullptr == headA || nullptr == headB) {
        return nullptr;
    }
    std::unordered_set<ListNode*> node_set;
    while (headA != nullptr) {
        node_set.insert(headA);
        headA = headA->next;
    }
    while (headB != nullptr) {
        if (node_set.count(headB) > 0) {
            return headB;
        } else {
            headB = headB->next;
        }
    }

    return nullptr;
}