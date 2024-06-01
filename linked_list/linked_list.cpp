/*
 * @Author: zlm 
 * @Date: 2021-07-09 23:04:48 
 * @Last Modified by: zlm
 * @Last Modified time: 2024-5-26 14:00
 */

#include "../head_file.h"
#include "../data_structure.h"
using namespace std;

/**
 * @brief: 141. Linked List Cycle
 * 经典题目：判断链表是否有环
 * @method: 快慢指针、双指针
 */

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
// method1: 快慢指针法
// 快指针一次走 2 格，慢指针一次走 1 格
// 如果存在环，那么前一个指针一定会经过若干圈之后追上慢的指针
// 数学分析：假设头节点到圆环入口节点的距离为d，圆环本身的周长为c
// 当慢指针走到入口节点 p1 时，此时快指针可能已经在圆环中走了若干圈，假设此时快指针的位置为 p2
// 两者的距离为p2-p1, 若快指针能追上慢指针，则两者的相对路程应该是deta_s = C-(p2-p1)，所消耗的时间
// 为time_cost = deta_s / deta_speed, 只要保证能整除即可，显然当 deta_speed = 1时，快指针一定可以能追上慢指针

// 若存在圆环，则快指针一定追上慢指针，此时两者相对路程为N
bool hasCycle(ListNode *head) {
    if (head == nullptr || head->next == nullptr) {
        return false;
    }
    
    ListNode* fast = head;
    ListNode* slow = head;
    
    while (fast != nullptr && fast->next != nullptr) {
        fast = fast->next->next;
        slow = slow->next;
        if (slow == fast) {
            return true;
        }
    }
    
    // 如果while循环内没有执行return语句，说明链表没有环
    return false;
}

// method2: 遍历节点，统计节点出现的次数
bool hasCycle2(ListNode *head) {
    if (head == nullptr || head->next == nullptr) {
        return false;
    }
    
    unordered_map<ListNode*, int> mp;
    while (head != nullptr) {
        mp[head]++;
        if(mp[head] == 2) {
            return true;
        }
        head = head->next;
    }
    // 如果while循环内没有执行return语句，说明链表没有环
    return false;
}

/**
 * @brief: 92. Reverse Linked List II
 * 给一个长度为n的单链表以及指定的位置left, right, 要求反转left-right之间的节点
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

/**
 * @brief: 61. 旋转链表
 * 给你一个链表的头节点 head ，旋转链表，将链表每个节点向右移动 k 个位置。
 * @method: 设链表长度为len, 链表分为两部分：前 n-k 和节点和后 k 个节点 (若 k > len，取 k = k % len) 
 * 1. 遍历一次，求出len, 由于后k个节点翻转到前面，因此最后一个结点一定与第一个结点首尾相连； 
 * 2. 遍历前n-k个节点，断开第n-k个节点与下一个结点的连接
 * 复杂度：时间O(n), 空间O(1)
*/
ListNode* rotateRight(ListNode* head, int k) {
  if (head == nullptr || head->next == nullptr || k <= 0) {
    return head;
  }

  // 1. 遍历求链表长度
  int len = 0;
  ListNode *pcur = head;
  ListNode *pre = head;
  while (pcur != nullptr) {
    len++;
    pre = pcur;
    pcur = pcur->next;
  }
  // 此时pre指向尾部节点，尾部节点应该指向head
  pre->next = head;

  // 2. 遍历前 len - k%len个结点,  并断开连接
  pcur = head; 
  for (int i = 1; i < len - k%len; i++) {
    pcur = pcur->next;
  }  // pcur指向第len-k%len个结点，其下一个结点将作为旋转后的head

  ListNode *newHead = pcur->next;
  pcur->next = nullptr;  // 断开连接

  return newHead;
}


int main() {
  system ("pause");
  return 0;
}