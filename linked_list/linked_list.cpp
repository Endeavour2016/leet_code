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
 * @brief: 23. 合并 K 个升序链表
 * @method: 借助优先级队列：
 * 1、把每个链表的头节点（最小元素）放入优先级队列，取出最小堆顶，即所有链表的最小节点 
 * 2、将堆顶所在链表的下一个节点（该链表的第二小的节点）加入优先队列，此时的堆顶
 * 即为所有链表中第二小的节点
 * 3、重复步骤1和2，直到所有节点都出队
 */
// 仿函数
struct NodeCmp{
    bool operator()(ListNode* lhs, ListNode* rhs) {
      return lhs->val > rhs->val;
    }
};

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

  ListNode* sorted_list_head = new ListNode(0);
  ListNode* node_ptr = sorted_list_head;
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

  return sorted_list_head->next;
}

// 方法二：分治法：递归的进行归并
// 两个有序链表合并 fast-template
ListNode* Merge2(ListNode* pHead1, ListNode* pHead2) {
    //一个已经为空了，直接返回另一个
    if (pHead1 == NULL)
        return pHead2;
    if (pHead2 == NULL)
        return pHead1;
    //加一个表头
    ListNode* head = new ListNode(0);
    ListNode* cur = head;
    //两个链表都要不为空
    while (pHead1 && pHead2) {
        //取较小值的结点
        if (pHead1->val <= pHead2->val) {
            cur->next = pHead1;
            //只移动取值的指针
            pHead1 = pHead1->next;
        } else {
            cur->next = pHead2;
            //只移动取值的指针
            pHead2 = pHead2->next;
        }
        //指针后移
        cur = cur->next;
    }
    //哪个链表还有剩，直接连在后面
    if (pHead1)
        cur->next = pHead1;
    else
        cur->next = pHead2;
    //返回值去掉表头
    return head->next;
}
// 划分合并区间
ListNode* divideMerge(vector<ListNode*>& lists, int left, int right) {
    if (left > right)
        return NULL;
    //中间一个的情况
    else if (left == right)
        return lists[left];
    //从中间分成两段，再将合并好的两段合并
    int mid = (left + right) / 2;
    return Merge2(divideMerge(lists, left, mid), divideMerge(lists, mid + 1,
                  right));
}
ListNode* mergeKLists(vector<ListNode*>& lists) {
    // k个链表归并排序
    return divideMerge(lists, 0, lists.size() - 1);
}


/**
 * @brief: 86. Partition List 分隔链表
 * 给你一个链表的头节点 head 和一个特定值 x ，请你对链表进行分隔，
 * 使得所有 小于 x 的节点都出现在 大于或等于 x 的节点之前。
 * @method: 拆分两个链表，small链表存放所有小于x的节点，large 链表按顺序存储所有大于等于 x 的节点
 * 两个链表拼接后形成完整链表
 */
class Solution {
public:
    ListNode* partition(ListNode* head, int x) {
        ListNode* small = new ListNode(0);
        ListNode* smallHead = small;
        ListNode* large = new ListNode(0);
        ListNode* largeHead = large;
        while (head != nullptr) {
            if (head->val < x) {
                small->next = head;
                small = small->next;
            } else {
                large->next = head;
                large = large->next;
            }
            head = head->next;
        }
        
        large->next = nullptr;
        small->next = largeHead->next;
        return smallHead->next;
    }
};


/**
 * @brief: 148. Sort List
 * 对链表进行排序，本身可以不考虑时间和空间复杂度，但题目的进阶要求是：时间复杂度O(nlogn)，空间复杂度O(n)
 * @method: 不考虑复杂度，可以先把链表元素拷贝到数组中，直接对数组排序，然后再把元素插入到新的链表
 */

// 方法二: 自顶向下的归并排序，考虑到递归调用的栈空间，空间复杂度 O(log⁡n)，时间复杂度 O(nlogn)
ListNode *sortList(ListNode *head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }
      
    // 归并排序需要找到中间位置的结点
    // 对于链表，可以使用快慢指针
    // fast 每次移动2个位置，slow 每次移动1个位置，速度相差2倍
    // 当 fast 移动至末尾结点 n，slow 只能移动到一半位置 n/2
    ListNode *fast = head;
    ListNode *slow = head;
    while (fast->next != nullptr && fast->next->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }  // end, 此时slow指向中间位置n/2
    
    // 链表分成两部分，递归排序，然后合并
    fast = slow->next; 
    slow->next = nullptr;
    
    // 递归
    ListNode *h1 = sortList(head);
    ListNode *h2 = sortList(fast);
    
    // 合并
    return merge(h1, h2);
}

// 合并两个有序链表的成员函数
// 这个函数也是之前leetcode上某个题目
ListNode *merge(ListNode *h1, ListNode *h2) {
    if (h1 == nullptr) {
        return h2;
    }
    if (h2 == nullptr) {
        return h1;
    }
    
    // 创建新的头结点，用于指向合并后的链表
    ListNode *newHead = new ListNode(0); 
    ListNode *temp = newHead;
    while ((h1 != nullptr) && (h2 != nullptr)) {
        if (h1->val < h2->val) {  // temp指向较小的结点
            temp->next = h1;
            h1 = h1->next;        // h1移动到下一个结点
            temp = temp->next;
        } else {
            temp->next = h2;
            h2 = h2->next;
            temp = temp->next;
        }
    }
    // 判断哪个链表还有剩下的元素
    if (h1 != nullptr) {  // 说明h2已经比较完毕
        temp->next = h1;
    }
    if (h2 != nullptr) {
        temp->next = h2;
    }
    
    return newHead->next;
  }


/**
 * @brief: 141. Linked List Cycle
 * 经典题目：判断链表是否有环
 * @method: 快慢指针、双指针
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
 * 给你一个链表的头节点 head，旋转链表，将链表每个节点向右移动 k 个位置。
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