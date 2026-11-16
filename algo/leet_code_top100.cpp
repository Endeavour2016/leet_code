/**
 * @file leet_code_top100.cpp
 * @brief 收录链表、数组、树、回溯和动态规划等算法练习。
 * @details 每个类或函数保留一份独立题目练习，注释说明解题思路和实现限制。
 * @note 本文件包含未完成实现和重复的 Solution 类名，不能直接整体编译。
 * 标有 TODO 的位置保留历史实现问题，使用前需要单独修正和验证。
 * @author zlm（原作者）
 * 创建日期：2018-06-24
 * 更新日期：2026-11-15
 * 修改日期：2026-11-15
 * AI辅助模型：GPT-6（未确认具体型号）
 */

#include <algorithm>
#include <climits>
#include <cmath>
#include <cstring>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

struct TreeNode {
  int val;
  TreeNode* left;
  TreeNode* right;
  TreeNode(int x) : val(x), left(NULL), right(NULL) {
  }
};

struct ListNode {
  int val;
  ListNode* next;
  ListNode(int x) : val(x), next(NULL) {
  }
};

// 1. 两数之和：返回从 1 开始计数的两个不同下标，未找到时返回空数组。
// 原记录日期：2018-06-24。
class Solution {
 public:
  vector<int> twoSum(vector<int>& numbers, int target) {
    if (numbers.empty()) {
      return vector<int>();
    }

    vector<int> indice;

    // 有序映射保存每个数最后出现的下标；std::map 不是哈希表。
    map<int, int> hash;
    int len = numbers.size();
    for (int i = 0; i < len; i++) {
      hash[numbers[i]] = i;
    }
    for (int i = 0; i < len; i++) {
      int sub = target - numbers[i];
      // 要求两个下标不同，数值可以相同；仅接受大于 i 的下标，避免复用同一元素。

      if (hash.find(sub) != hash.end() && hash[sub] > i) {
        indice.push_back(i + 1);
        indice.push_back(hash[sub] + 1);
        break;
      }
    }
    return indice;
  }
};

// 两数之和的单遍哈希表练习：先查补数，再记录当前值及其下标。
// TODO：第二个结果应为当前下标 i，当前却查询 mp[num[i]]，且函数末尾缺少返回值。
vector<int> twoSum2(vector<int>& num, int target) {
  if (num.empty()) {
    return vector<int>();
  }
  vector<int> res(2);
  unordered_map<int, int> mp;
  int len = num.size();
  for (int i = 0; i < len; i++) {
    int sub = target - num[i];
    if (mp.find(sub) != mp.end()) {  // 已经找到了两个元素
      res[0] = mp[sub];
      res[1] = mp[num[i]];
      break;
    } else {
      mp.insert({num[i], i});  // 仅记录尚未出现的值及其下标。
    }
  }
}

// 反转链表：递归写法的未完成草稿。
// head 指向首个数据结点，链表不带辅助头结点。
// 原记录日期：2018-06-24。
class Solution6 {
 public:
  ListNode* reverseList(ListNode* head) {
    if (head == nullptr) {
      return nullptr;
    }
    TreeNode* newHead = new TreeNode(0);
    // TODO：尚未实现反转逻辑；这里误用了 TreeNode，非空分支也缺少返回值。
  }
};

// 反转链表：迭代调整每个结点的 next，返回新的头指针。
// head 指向首个数据结点，链表不带辅助头结点；原链表会被修改。
// 原记录日期：2018-06-24。
class Solution7 {
 public:
  ListNode* reverseList(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {  // 只有一个结点或者为空
      return head;
    }

    ListNode* pre = head;
    ListNode* pcur = head->next;
    ListNode* pnext = nullptr;

    while (pcur != nullptr) {
      pnext = pcur->next;
      pcur->next = pre;  // 将当前结点接到已反转部分的前面。

      pre = pcur;
      pcur = pnext;
    }  // pre 指向原尾结点，即反转后的头结点。

    head->next = nullptr;

    return pre;
  }
};

// 环形链表的入口：快慢指针相遇后，再寻找入口；无环返回 nullptr。
// 原记录日期：2018-06-24。
class Solution8 {
 public:
  ListNode* detectCycle(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
      return nullptr;
    }

    ListNode* fast = head;
    ListNode* slow = head;

    while (fast != nullptr && fast->next != nullptr) {
      fast = fast->next->next;
      slow = slow->next;
      if (slow == fast) {
        break;
      }
    }

    // 快指针到达链表末尾，说明无环。
    if (fast == nullptr || fast->next == nullptr) {
      return nullptr;
    }

    // 相遇后，让 slow 从头结点出发，fast 从相遇点出发。
    // 两者每次各走一步，下次相遇的位置就是环入口。
    slow = head;
    while (slow != fast) {
      slow = slow->next;
      fast = fast->next;
    }
    return fast;
  }
};

// 数组中第 k 大的元素：原地降序排序，返回下标 k - 1 的值。
// 重复值分别计数；非法 k 或空数组返回 0，与合法结果 0 无法区分。
// 原记录日期：2018-06-24。
class Solution9 {
 public:
  int findKthLargest(vector<int>& nums, int k) {
    if (k < 1 || k > (int)nums.size() || nums.empty()) {
      return 0;
    }

    sort(nums.begin(), nums.end());
    reverse(nums.begin(), nums.end());  // 把序列从大到小排列
    int len = nums.size();

    return nums[k - 1];
  }
};

// 347. 前 K 个高频元素：最大堆。
// 统计每个值的出现次数，把 {次数, 值} 放入优先队列，依次取出频次最高的值。
// 前提：1 <= k <= 不同元素的个数。
// 原作者：zlm；原记录日期：2018-06-25。
class Solution10 {
 public:
  vector<int> topKFrequent(vector<int>& nums, int k) {
    vector<int> res;
    if (nums.empty()) {
      return res;
    }

    unordered_map<int, int> mp;
    for (auto v : nums) {
      mp[v]++;
    }

    if (k > mp.size()) {
      return res;
    }
    // 最大堆按 pair 的频次优先比较，频次相同再比较数值。
    priority_queue<pair<int, int>> que;

    // 将 {出现次数, 数值} 加入最大堆，堆顶的频次最大。
    for (auto it = mp.begin(); it != mp.end(); it++) {
      que.push({it->second, it->first});
    }

    // 堆顶保存当前剩余元素中出现次数最多的值。
    while (k--) {
      res.push_back(que.top().second);
      que.pop();
    }

    return res;
  }
};

// 347. 前 K 个高频元素：按出现次数分桶。
// bucket[f] 保存出现 f 次的值，从高频桶向低频桶取出 k 个值。
// 前提：1 <= k <= 不同元素的个数；当前实现未校验 k 的范围。
// 原作者：zlm；原记录日期：2018-06-25。
class Solution347 {
 public:
  vector<int> topKFrequent(vector<int>& nums, int k) {
    map<int, int> m;
    vector<vector<int>> bucket(nums.size() + 1);
    vector<int> res;
    for (auto a : nums) {
      ++m[a];
    }

    for (auto it : m) {
      bucket[it.second].push_back(it.first);
    }
    for (int i = nums.size(); i >= 0; --i) {
      for (int j = 0; j < bucket[i].size(); ++j) {
        res.push_back(bucket[i][j]);
        if (res.size() == k) {
          return res;
        }
      }
    }

    return res;
  }
};

// 169. 多数元素：寻找排序后下标为 n / 2 的元素。
// 前提：数组中存在出现次数严格超过 n / 2 的元素。
// 通过原地划分缩小查找范围，不需要将整个数组排序；会修改输入数组。
// 若不保证多数元素存在，还需统计候选值的频次，当前实现未做该验证。
// 原作者：zlm；原记录日期：2018-06-25。
class Solution12 {
 public:
  int majorityElement(vector<int>& nums) {
    if (nums.empty()) {
      return -1;
    }
    int N = nums.size();
    int low = 0, high = N - 1;

    while (low <= high) {
      int mid = partition(nums, low, high);
      if (mid == N / 2) {  // 在多数元素存在的前提下，中间位置的值就是答案。
        return nums[mid];
      } else if (mid < N / 2) {
        low = mid + 1;
      } else {
        high = mid - 1;
      }
    }
    return -1;
  }

  int partition(vector<int>& num, int low, int high) {
    int pivot = num[low];
    while (low < high) {
      while (low < high && pivot <= num[high]) {
        high--;
      }
      num[low] = num[high];

      while (low < high && pivot > num[low]) {
        low++;
      }
      num[high] = num[low];
    }
    num[low] = pivot;
    return low;
  }
};

// 多数元素的另一种解法：Boyer-Moore 投票法，用不同的值两两抵消。
// cnt 为候选值的剩余票数；处理新元素前若 cnt 为 0，就把该元素设为候选值。
// 否则，相同值使 cnt 加一，不同值使 cnt 减一。
// 前提：多数元素存在；若无此保证，返回前必须再统计候选值的频次。
class Solution13 {
 public:
  int majorityElement(vector<int>& nums) {
    if (nums.empty()) {
      return -1;
    }

    int cnt = 1;
    int curElem = nums[0];  // 选择第一个元素用于和其他的元素进行抵消
    for (int i = 1; i < nums.size(); i++) {
      if (cnt != 0) {
        if (curElem == nums[i]) {
          cnt++;
        } else {
          cnt--;
        }
      } else {  // cnt == 0 时，需要重新选择当前元素并计数
        curElem = nums[i];
        cnt = 1;
      }
    }
    return curElem;  // 依赖多数元素存在的前提，当前没有额外验证频次。
  }
};

// 287. 寻找重复数（题目与当前实现不匹配）。
// 题目：n + 1 个整数的取值范围为 [1, n]，找出唯一重复出现的数。
// 要求不修改数组，额外空间为 O(1)。
// TODO：当前代码仅对所有元素求异或，不能解决本题。
// 异或法适用于一个数出现一次、其他数均出现两次的情形，参见 Solution136。
// 原作者：zlm；原记录日期：2018-06-25。
class Solution287 {
 public:
  int findDuplicate(vector<int>& nums) {
    if (nums.size() == 0) {
      return 0;
    }

    int res = 0;

    for (int i = 0; i < (int)nums.size(); i++) {
      res ^= nums[i];
    }

    return res;
  }
};

// 5. 最长回文子串：动态规划练习。
// dp[i][j] 表示闭区间 s[i..j] 是否为回文，依赖 dp[i + 1][j - 1]。
// 时间和额外空间复杂度均为 O(n^2)。
// TODO：尚未初始化单字符回文，且当前遍历顺序未保证依赖状态先计算。
// 原作者：zlm；原记录日期：2018-06-25。
class Solution15 {
 public:
  string longestPalindrome(string s) {
    if (s.empty()) {
      return string();
    }

    int len = s.length();
    vector<vector<bool>> dp(len, vector<bool>(len));

    int maxLength = 0;   // 当前找到的最长回文子串长度。
    int startIndex = 0;  // 最长回文子串的起始下标。

    for (int i = 0; i < len; i++) {
      for (int j = i + 1; j < len; j++) {
        if (j - i <= 1) {
          dp[i][j] = (s[i] == s[j]);
        } else {
          dp[i][j] = (s[i] == s[j]) && dp[i + 1][j - 1];
        }

        // 找到更长的回文时，更新起点和长度。
        if (dp[i][j] && maxLength < j - i + 1) {
          maxLength = j - i + 1;
          startIndex = i;
        }
      }
    }

    // 返回记录的最长回文子串。
    return s.substr(startIndex, maxLength);
  }
};

// 53. 最大子数组和：返回连续子数组的最大元素和。
// curSum 表示以当前元素结尾的最大子数组和；maxSum 保存已处理部分的最优值。
// 加入当前元素前，若之前的 curSum 为负，则从当前元素重新开始。
// 时间复杂度 O(n)，额外空间 O(1)；空数组返回 0。
// 原作者：zlm；原记录日期：2018-06-25。
class Solution53 {
 public:
  int maxSubArray(vector<int>& nums) {
    if (nums.empty()) {
      return 0;
    }

    int len = nums.size();
    int curSum = 0;  // 处理当前元素后，记录以它结尾的最大子数组和。
    int maxSum = INT_MIN;

    for (int i = 0; i < len; i++) {
      if (curSum < 0) {
        curSum = nums[i];
      } else {
        curSum += nums[i];  // 记录当前累加和
      }
      if (curSum > maxSum) {
        maxSum = curSum;
      }
    }
    return maxSum;
  }
};

// 70. 爬楼梯：每次走 1 或 2 级，求到达第 n 级的方案数。
// 最后一步来自第 n - 1 或 n - 2 级，因此 f(n) = f(n - 1) + f(n - 2)。
// 前提：n 为正整数，且结果能用 int 表示。
// 原作者：zlm；原记录日期：2018-06-25。
class Solution70 {
 public:
  int climbStairs(int n) {
    if (n <= 1) {
      return n;
    }

    // 设f(i)表示爬i阶楼梯的方案个数，i：1~n
    // f(i) = f(i-1)+f(i-2)  最后一步可以走一阶也可以走两阶

    vector<int> f(n + 1);
    f[1] = 1;
    f[2] = 2;

    for (int i = 3; i <= n; i++) {
      f[i] = f[i - 1] + f[i - 2];
    }

    return f[n];
  }
};
// 爬楼梯的滚动变量练习。
// TODO：初值与 Solution70 的定义不一致，例如 n == 2 时返回 1 而不是 2。
int climbeStair(int n) {
  if (n <= 1) {
    return n;
  }
  int f0 = 1, f1 = 0, f = 0;
  for (int i = 2; i <= n; i++) {
    f = f0 + f1;
    f0 = f1;
    f1 = f;
  }
  return f;
}

// 128. 最长连续序列：返回数值连续的最长序列长度。
// 元素不必在原数组中相邻，例如 {100, 4, 200, 1, 3, 2} 的结果为 4。
// 哈希表记录数值及其访问状态，从未访问的值向更小和更大的连续整数扩展。
// TODO：i +/- 1 及扩展时的递增、递减未处理 int 边界。
// 原作者：zlm；原记录日期：2018-06-25。
class Solution128 {
 public:
  int longestConsecutive(vector<int>& nums) {
    if (nums.empty()) {
      return 0;
    }

    unordered_map<int, bool> used;  // 用来表示数组中的元素是否被用过
    for (auto v : nums) {
      used[v] = false;
    }

    int maxLength = 0;

    for (auto i : nums) {
      if (used[i]) {
        continue;  // 如果元素已经使用过，则没必要再次使用
      }

      int len = 1;  // 记录当前查找的序列的长度
      used[i] = true;

      // 按数值递增查找 i + 1、i + 2 等，不是沿原数组下标查找。
      for (int j = i + 1; used.find(j) != used.end(); j++) {
        used[j] = true;
        len++;
      }

      // 按数值递减查找 i - 1、i - 2 等。
      for (int j = i - 1; used.find(j) != used.end(); j--) {
        used[j] = true;
        len++;
      }

      if (len > maxLength) {
        maxLength = len;
      }
    }

    return maxLength;
  }
};

// 234. 回文链表：反转后半段，与前半段逐个比较。
// 时间复杂度 O(n)，额外空间 O(1)。
// 注意：当前实现会断开链表并反转后半段，返回前不会恢复原链表。
// 原作者：zlm；原记录日期：2018-06-26。
class Solution234 {
 public:
  bool isPalindrome(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
      return true;
    }

    int len = 0;
    ListNode* tmp = head;
    while (tmp != nullptr) {
      len++;
      tmp = tmp->next;
    }

    // 找到链表中心
    tmp = head;
    for (int i = 1; i <= (len - 1) / 2; i++) {
      tmp = tmp->next;
    }
    // tmp 指向下标为 (len - 1) / 2 的结点（从 0 计数）。

    ListNode* head2 = tmp->next;  // head2 指向链表的后半段
    tmp->next = nullptr;          // 前半段链表末尾置空

    // head2指向的链表翻转
    ListNode* revHead = Reverse(head2);

    // 比较前半段与反转后的后半段，共比较 len / 2 对结点。
    // 奇数长度时，中间结点留在前半段，不参与比较。
    for (int i = 0; i < len / 2; i++) {
      if (head->val != revHead->val) {
        return false;
      }
      head = head->next;
      revHead = revHead->next;
    }

    return true;
  }

  // 翻转链表
  ListNode* Reverse(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
      return head;  // 空链表或单结点链表直接返回。
    }

    ListNode* pcur = head->next;
    ListNode* pre = head;

    ListNode* pnext = nullptr;

    while (pcur != nullptr) {
      pnext = pcur->next;
      pcur->next = pre;

      pre = pcur;
      pcur = pnext;
    }

    // 此时pre指向翻转后的链表首结点
    head->next = nullptr;
    return pre;
  }
};

// 647. 回文子串：分别以一个字符和两个相邻字符为中心向两侧扩展。
// 不同位置的相同子串分别计数；时间复杂度 O(n^2)。
// 中心扩展本身只需 O(1) 额外空间。
// 当前形参按值接收字符串，可能产生 O(n) 副本。
// TODO：成员 cnt 在每次调用前未重置，重复调用同一对象会累加历史结果。
// 原作者：zlm；原记录日期：2018-06-26。
class Solution647 {
 private:
  int cnt = 0;

 public:
  int countSubstrings(string s) {
    if (s.empty()) {
      return 0;
    }
    for (int i = 0; i < s.length(); i++) {
      HuiwenSubstring(s, i, i);  // 以一个字符为中心，统计奇数长度回文。
      // 以相邻字符之间为中心，统计偶数长度回文，如 abba。
      HuiwenSubstring(s, i, i + 1);
    }
    return cnt;
  }

  // 从给定中心向两侧扩展，每扩展成功一次就增加一个回文子串。
  void HuiwenSubstring(string& s, int left, int right) {
    while (left >= 0 && right < s.length() && s[left] == s[right]) {
      cnt++;
      left--;  // 只移动边界，原字符串不变。
      right++;
    }
  }
};

// 19. 删除链表的倒数第 n 个结点，返回删除后的头指针。
// 设链表长度为 len：p1 先从头结点前进 n - 1 步，再与 p2 同步前进。
// p1 到达尾结点时，p2 指向正数第 len - n + 1 个结点，即待删除结点。
// 前提：1 <= n <= len，结点由 new 分配且允许本函数 delete。
// n 超过链表长度时返回 nullptr，原链表不变；n <= 0 未校验。
// 原作者：zlm；原记录日期：2018-06-26。
class Solution19 {
 public:
  ListNode* removeNthFromEnd(ListNode* head, int n) {
    if (head == nullptr) {
      return nullptr;
    }

    ListNode* p1 = head;
    // p1 起始为第一个结点，向后移动 n-1 步
    for (int i = 1; i <= n - 1 && p1 != nullptr; i++) {
      p1 = p1->next;
    }
    if (p1 == nullptr) {  // 说明结点的个数小于 n
      return nullptr;
    }

    // p1 指向从头开始计数的第 n 个结点。
    ListNode* p2 = head;
    ListNode* pre = nullptr;       // 用于记录 p2 结点的前一个结点
    while (p1->next != nullptr) {  // 两个指针同步前进，共移动 len - n 步。
      p1 = p1->next;
      pre = p2;
      p2 = p2->next;
    }
    // p2 指向倒数第 n 个结点；pre 为其前驱，删除头结点时 pre 为空。
    // 如果待删除的结点为首结点head
    if (p2 == head) {
      head = p2->next;
      delete p2;
      p2 = nullptr;
    } else {
      pre->next = p2->next;
      delete p2;
      p2 = nullptr;
    }

    return head;
  }
};

// 136. 只出现一次的数字。
// 前提：恰有一个数出现一次，其他数均出现两次。
// 所有元素异或后，成对的数抵消为 0，剩下的值即为答案。
// 原作者：zlm；原记录日期：2018-06-26。
class Solution136 {
 public:
  int singleNumber(vector<int>& nums) {
    if (nums.empty()) {
      return 0;
    }

    int res = 0;  // 0^anyNum = anyNum

    for (int n : nums) {
      res = res ^ n;
    }

    return res;
  }
};

// 排列与组合。
// 46. 全排列：固定当前位置的元素，递归排列剩余位置。
// 递归返回后交换回来，恢复本层选择之前的数组。
// 前提：输入元素互不重复；当前实现不对重复排列去重。
// 原作者：zlm；原记录日期：2018-06-26。
class Solution46 {
 public:
  vector<vector<int>> permute(vector<int>& nums) {
    vector<vector<int>> res;  // 存放结果
    if (nums.empty()) {
      return res;
    }

    permutation(nums, 0, res);

    return res;
  }

  // nums 原地保存当前排列；res 收集结果；begin 为本次待确定的位置。
  void permutation(vector<int>& nums, int begin, vector<vector<int>>& res) {
    if (begin == nums.size() - 1) {  // 只剩最后一个位置，当前排列已确定。
      // 保存 nums 的副本；后续分支位于 else 中，本分支随即结束。
      res.push_back(nums);
    } else {
      // 依次选择 nums[begin..end) 中的元素放到 begin 位置。
      for (int i = begin; i < nums.size(); i++) {
        swap(nums[begin], nums[i]);
        // 递归的对 begin 后面的元素进行排列
        permutation(nums, begin + 1, res);

        // 排列后需要把原来的交换的元素再交换回来
        swap(nums[begin], nums[i]);
        // 下一次循环尝试另一个元素，已经确定的前缀保持不变。
      }
    }
  }
};

// 77. 组合：从 1 到 n 中选出 k 个不同的数，枚举所有组合。
// 按数值递增的顺序选择元素，避免重复生成同一组合。
// 前提：n > 0，0 <= k <= n；k == 0 时结果包含一个空组合。
// 结果共 C(n, k) 个；k > 0 时保存结果需要 O(k * C(n, k)) 空间。
// 递归最多选择 k 个数，调用栈深度为 O(k + 1)。
// 当前实现未按剩余元素个数剪枝，还会遍历不足以凑满 k 个数的分支。
class Solution77 {
 public:
  vector<vector<int>> combine(int n, int k) {
    vector<vector<int>> res;

    if (n <= 0 || k > n) {
      return res;
    }

    vector<int> cur;                 // 当前组合
    combination(res, cur, n, k, 1);  // 组合中的数字范围为：1~n

    return res;
  }

  // cur 为已选前缀；begin 为下一次可选数字的下界，不要求一定选中 begin。
  void combination(vector<vector<int>>& res, vector<int>& cur, int n, int k,
                   int begin) {
    if (cur.size() == k) {  // 回溯的终止条件：找到一种组合后函数返回
      res.push_back(cur);
      return;
    }

    // 将 i 追加到当前前缀，再选择后续元素。
    for (int i = begin; i <= n; i++) {
      cur.push_back(i);

      // 递归
      // 下一层只能选大于 i 的数，保证无重复且递增。
      combination(res, cur, n, k, i + 1);

      // 撤销本层添加的 i，恢复进入本次循环前的前缀；cur 不一定为空。
      cur.pop_back();
    }
  }
};

// 39. 组合总和：从 candidates 中选出总和为 target 的所有不同组合。
// 前提：candidates 由互不重复的正整数组成，target 为非负整数。
// 每个数可以重复选取；按下标不减的顺序扩展组合，避免排列顺序造成的重复。
// 当前组合的和等于 target 时保存结果，大于 target 时停止扩展。
class Solution39 {
 public:
  vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
    vector<vector<int>> res;
    if (candidates.size() <= 0) {
      return res;
    }

    vector<int> cur_com;  // 记录某一个组合

    findCombinations_DFS(res, cur_com, candidates, target, 0);

    return res;
  }

  // cur_com 为已选前缀，pos 为下一次允许选取的最小下标。
  void findCombinations_DFS(vector<vector<int>>& res, vector<int>& cur_com,
                            vector<int>& candidates, int target, int pos) {
    // 当前组合的元素和等于目标值时保存结果。
    if (std::accumulate(cur_com.begin(), cur_com.end(), 0) == target) {
      res.push_back(cur_com);
      return;
    }

    if (accumulate(cur_com.begin(), cur_com.end(), 0) > target) {
      return;
    }

    // 依次追加下标不小于 pos 的候选值，保持选择下标不减。
    for (int i = pos; i < candidates.size(); ++i) {
      cur_com.push_back(candidates[i]);

      // 在当前前缀基础上继续选择候选值。
      // 这里入参为 i 而不是 i+1：因为题目允许同一个数可以重复出现
      findCombinations_DFS(res, cur_com, candidates, target, i);

      // 移除本层刚添加的值，恢复原前缀；cur_com 不一定为空。
      cur_com.pop_back();
    }
  }
};

// 617. 合并二叉树：重叠位置的结点值相加，递归处理左右子树。
// 两个结点均存在时创建新结点；只有一个结点存在时直接复用其子树。
// 注意：返回树可能与输入树共享结点，不能把三棵树当作完全独立的对象释放。
// 原作者：zlm；原记录日期：2018-06-27。
class Solution617 {
 public:
  TreeNode* mergeTrees(TreeNode* t1, TreeNode* t2) {
    if (t1 == nullptr && t2 != nullptr) {
      return t2;
    } else if (t2 == nullptr && t1 != nullptr) {
      return t1;
    } else if (t1 == nullptr && t2 == nullptr) {
      return nullptr;
    }

    TreeNode* root = new TreeNode(0);
    root->val = t1->val + t2->val;  // 合并根结点

    // 递归对左子树和右子树进行合并
    root->left = mergeTrees(t1->left, t2->left);
    root->right = mergeTrees(t1->right, t2->right);

    return root;
  }
};

// 113. 路径总和 II：找出从根到叶子、结点值之和等于 sum 的所有路径。
// 深度优先遍历时保存当前路径，到达叶子后检查总和；返回上层前移除当前结点。
// 原作者：zlm；原记录日期：2018-06-27。
class Solution113 {
 public:
  vector<vector<int>> pathSum(TreeNode* root, int sum) {
    vector<vector<int>> res;
    if (root == nullptr) {
      return res;
    }

    vector<int> cur;
    FindPath(root, res, cur, sum);

    return res;
  }

  // 递归函数
  void FindPath(TreeNode* root, vector<vector<int>>& res, vector<int>& cur,
                int sum) {
    if (root == nullptr) {
      return;
    }

    cur.push_back(root->val);  // 将当前结点加入路径。

    // 只有根到叶子的完整路径才参与总和判断。
    if (root->left == nullptr && root->right == nullptr) {
      if (accumulate(cur.begin(), cur.end(), 0) == sum) {
        res.push_back(cur);
      }
    } else {
      if (root->left != nullptr) {
        FindPath(root->left, res, cur, sum);
      }
      if (root->right != nullptr) {
        FindPath(root->right, res, cur, sum);
      }
    }

    // 离开当前结点前移除它，使调用方的路径恢复原状。
    cur.pop_back();
  }
};

// 动态规划。
// 120. 三角形最小路径和。
// 从顶点走到最底层，每步从第 i 行的第 j 列走向下一行的第 j 或 j + 1 列。
// dp[i][j] 表示从顶点到 (i, j) 的最小路径和。
// 内部位置取两个上方来源的较小值再加当前值，两侧边界只有一个来源。
class Solution120 {
 public:
  int minimumTotal(vector<vector<int>>& triangle) {
    if (triangle.empty()) {
      return 0;
    }

    int row = triangle.size();
    vector<vector<int>> dp(row, vector<int>(row, 0));

    // 初始化顶点的路径和。
    dp[0][0] = triangle[0][0];  // 第一行只有一个数

    for (int i = 1; i < row; i++) {
      for (int j = 0; j < triangle[i].size(); j++) {
        if (j == 0) {  // 对于第一列, (i, j) 的前一个落点只能是(i-1, j)
          dp[i][j] = dp[i - 1][j] + triangle[i][j];
        } else if (j == triangle[i].size() - 1) {  // 当前行的最后一列
          dp[i][j] = dp[i - 1][j - 1] + triangle[i][j];
        } else {
          dp[i][j] = min(dp[i - 1][j - 1], dp[i - 1][j]) + triangle[i][j];
        }
      }
    }

    // 最底层所有 dp 值中的最小值，就是从顶点到底部的最小路径和。
    int minimum = dp[row - 1][0];
    for (int i = 1; i < row; i++) {
      if (dp[row - 1][i] < minimum) {
        minimum = dp[row - 1][i];
      }
    }
    return minimum;
  }
};

// 64. 最小路径和：从矩阵左上角走到右下角，每次只能向右或向下。
// dp[i][j] 为到达 (i, j) 的最小路径和，由上方或左方的较小路径和转移。
// 前提：grid 为矩形网格。
class Solution64 {
 public:
  int minPathSum(vector<vector<int>>& grid) {
    if (grid.empty() || grid[0].empty()) {
      return 0;
    }
    int row = grid.size();
    int col = grid[0].size();

    vector<vector<int>> dp(row, vector<int>(col, 0));

    for (int i = 0; i < row; i++) {
      for (int j = 0; j < col; j++) {
        if (i == 0 && j == 0) {
          dp[i][j] = grid[i][j];
        } else if (j == 0) {  // 第一列
          dp[i][j] = dp[i - 1][j] + grid[i][j];
        } else if (i == 0) {  // 计算第一行
          dp[i][j] = dp[i][j - 1] + grid[i][j];
        } else {
          dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + grid[i][j];
        }
      }
    }
    return dp[row - 1][col - 1];
  }
};

// 63. 不同路径 II：统计绕过障碍、从左上角走到右下角的路径数。
// 每次只能向右或向下；网格中 0 表示可通行，1 表示障碍。
// dp[i][j] 为到达 (i, j) 的路径数：遇到障碍为 0，否则累加上方和左方。
// 前提：obstacleGrid 为矩形网格，所有中间路径计数均能用 int 表示。
class Solution63 {
 public:
  int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
    // 边界条件
    if (obstacleGrid.empty() || obstacleGrid[0].empty()) {
      return 0;
    }

    int row = obstacleGrid.size();
    int col = obstacleGrid[0].size();

    vector<vector<int>> dp(row, vector<int>(col, 0));

    // 确定初值
    dp[0][0] = obstacleGrid[0][0] == 0 ? 1 : 0;  // 有障碍时，路径个数为0

    // 第一列上的点只能向下走,注意障碍物
    // 如果上一个点有路径，且当前点无障碍，则dp[i][0] = 1
    for (int i = 1; i < row; i++) {
      dp[i][0] = (dp[i - 1][0] == 1 && obstacleGrid[i][0] == 0) ? 1 : 0;
    }

    // 第一行上的点只能向右走,注意障碍物
    // 如果上一个点有路径，且当前点无障碍，则dp[0][j] = 1
    for (int j = 1; j < col; j++) {
      dp[0][j] = (dp[0][j - 1] == 1 && obstacleGrid[0][j] == 0) ? 1 : 0;
    }

    // 其它点的递推公式
    for (int i = 1; i < row; i++) {
      for (int j = 1; j < col; j++) {
        if (obstacleGrid[i][j] == 1) {
          dp[i][j] = 0;
        } else {
          dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
        }
      }
    }

    return dp[row - 1][col - 1];
  }
};

// 69. x 的平方根：对非负整数 x，返回平方根向下取整的值。
// 二分查找满足 mid * mid <= x 的最大整数。
// TODO：当前使用 int 计算 low + high 和 mid * mid，较大输入会溢出。
// 原作者：zlm；原记录日期：2018-07-11。
class Solution69 {
 public:
  int mySqrt(int x) {
    if (x <= 1) {
      return x;
    }

    int low = 1, high = x;  // 对于大于1的正数，显然sqrt(x) < x
    int mid;
    while (low <= high) {
      mid = (low + high) / 2;
      if (mid * mid == x) {
        return mid;
      } else if (mid * mid < x) {
        low = mid + 1;
      } else {
        high = mid - 1;
      }
    }
    return high;
  }
};

// 牛顿迭代法练习：求 f(s) = s^2 - x 的零点，迭代式为 s_next = (s + x / s) / 2。
// TODO：s0/s1 使用 int，当前精度判断等价于判断两值是否相等。
// 整数截断可能导致循环，例如 x == 3 时在 1 和 2 之间往返；加法也可能溢出。
int sqrt2(int x) {
  if (x <= 1) {
    return x;
  }
  int s0 = 0, s1 = 1;  // 迭代初始值

  while (abs(s1 - s0) > 1.0e-6) {
    // 当前两次整数结果不同，继续迭代；这不是浮点精度判断。
    s0 = s1;
    s1 = (s0 + x / s0) / 2;
  }
  return s0;
}

// 贪心算法。
// 135. 分发糖果：每人至少一颗，相邻孩子中评分更高者获得更多糖果。
// 先从左向右满足与左邻居的关系，再从右向左满足与右邻居的关系。
// 第二遍若左侧评分更高，且糖果数不多于右侧，就调整为右侧糖果数加一。
// 时间复杂度 O(n)，额外空间 O(n)。
// 原作者：zlm；原记录日期：2018-07-13。
class Solution135 {
 public:
  int candy(vector<int>& ratings) {
    if (ratings.empty()) {
      return 0;
    }

    int N = ratings.size();
    vector<int> candy(N, 1);  // 每个人至少一块糖果

    // 从左向右扫描，找右侧等级高的孩子
    for (int i = 0; i < N - 1; i++) {
      if (ratings[i] < ratings[i + 1]) {
        candy[i + 1] = candy[i] + 1;
      }
    }

    // 从右向左扫描,找左侧等级高的孩子
    for (int i = N - 1; i >= 1; i--) {
      if (ratings[i - 1] > ratings[i] && candy[i - 1] <= candy[i]) {
        candy[i - 1] = candy[i] + 1;
      }
    }

    // 返回candy之和
    int sum = 0;
    for (int i : candy) {
      sum += i;
    }

    return sum;
  }
};

// 55. 跳跃游戏：nums[i] 表示在位置 i 最多可以向右跳的距离。
// 判断能否从下标 0 到达最后一个位置；数组元素必须为非负整数。
// 扫描到位置 j 时，step 表示已知可达范围在 j 右侧还剩多少距离。
// 先扣除前进一步的距离，再用 nums[j] 扩展可达范围；扣除后为负即不可达。
// 原作者：zlm；原记录日期：2018-07-13。
class Solution55 {
 public:
  bool canJump(vector<int>& nums) {
    // 向量为空，返回false
    if (nums.empty()) {
      return false;
    }

    int len = nums.size();

    int step = nums[0];  // 从位置 0 出发时的最大可达距离。

    for (int j = 1; j < len; j++) {
      step--;  // 扫描位置右移一格，剩余可达距离减一。

      if (step < 0) {  // 先前的可达范围未覆盖位置 j。
        return false;
      }

      // 位置 j 可达，可以利用 nums[j] 扩展可达范围。
      if (step < nums[j]) {
        step = nums[j];  // 更新从 j 起还可以覆盖的距离。
      }
    }

    return true;
  }
};

// 45. 跳跃游戏 II：求从起点到末尾的最少跳跃次数。
// 在一次能到达的范围 (cur, cur + A[cur]] 中，选择 i + A[i] 最大的落点。
// 前提：A 包含 n 个非负整数，且末尾可达；当前实现不能可靠地处理不可达输入。
// 例如 n > 1 且 A[0] == 0 时会循环，不能依赖末尾的 return -1 判断失败。
// 原作者：zlm；原记录日期：2018-07-13。
class Solution45 {
 public:
  int jump(int A[], int n) {
    if (n <= 1) {
      return 0;
    }

    int step = 0;      // 最少的跳跃次数
    int cur = 0;       // 本次起跳的下标。
    int furthest = 0;  // 用来存放从当前位置一次跳跃可到达的最远位置

    // 从 cur 位置起跳
    while (cur < n) {
      step++;  // 当前区间内跳跃一次
      furthest = cur + A[cur];
      if (furthest >= n - 1) {
        return step;
      }
      // 确定区间(cur, furthest]内的落点
      int tmp = -1;
      int index = 0;
      for (int i = cur + 1; i <= furthest; i++) {
        if (tmp < A[i] + i) {
          tmp = A[i] + i;
          index = i;  // 记录下一跳能延伸得最远的候选落点。
        }
      }  // index 为选中的下一次落点。
      cur = index;
    }
    return -1;  // 当前循环并未可靠地检测不可达情况，参见函数前说明。
  }
};

// 83. 删除排序链表中的重复元素（题目与当前实现不匹配）。
// 本题要求每个值保留一个，例如 1->2->2->3 应变为 1->2->3。
// TODO：当前实现跳过整段重复值，实际得到 1->3，更接近第 82 题的要求。
// newHead 是辅助头结点，pre 指向已保留部分的尾结点，pcur 扫描后续结点。
// 当前仅断开重复结点的连接，未释放被跳过的结点和辅助头结点。
// 原作者：zlm；原记录日期：2018-07-14。
class Solution83 {
 public:
  ListNode* deleteDuplicates(ListNode* head) {
    if (head == nullptr) {
      return nullptr;
    }

    ListNode* newHead = new ListNode(0);
    newHead->next = head;

    ListNode* pcur = head;
    ListNode* pre = newHead;

    while (pcur != nullptr && pcur->next != nullptr) {
      if (pcur->val == pcur->next->val) {  // 发现重复段，跳过所有值相同的结点。
        int tmp = pcur->val;

        while (pcur != nullptr && pcur->val == tmp) {
          pcur = pcur->next;
        }

        // 让已保留部分接到重复段之后；这里只断开连接，不释放结点。
        pre->next = pcur;
      } else {  // pcur->val != pcur->next->val
        pcur = pcur->next;
        pre = pre->next;  // 当前值不重复，保留该结点并继续扫描。
      }
    }

    return newHead->next;
  }
};

// 3. 无重复字符的最长子串：滑动窗口配合集合 set。
// set 保存半开区间 s[left..right) 中的字符，窗口内始终没有重复字符。
// 若 s[right] 已存在，每次移除最左侧一个字符，直到可以加入 s[right]。
// 这里按 char 处理字节，不支持把多字节编码的字符作为一个整体。
// 原作者：zlm；原记录日期：2018-07-15。
class Solution3 {
 public:
  int lengthOfLongestSubstring(string s) {
    int len = s.length();
    set<char> st;
    int maxLen = 0;
    int left = 0, right = 0;

    while (right < len && left < len) {
      // 若s[right]在集合 st 中没有出现过，则将其加入集合
      if (st.find(s[right]) == st.end()) {
        st.insert(s[right]);
        right++;
        // right - left 为当前无重复窗口的长度。
        maxLen = max(maxLen, right - left);
      } else {  // 右侧字符已存在，本轮只移除窗口最左侧的一个字符。
        st.erase(s[left]);
        left++;
      }
    }

    return maxLen;
  }
};

// 61. 旋转链表：将末尾 k 个结点整体移到前面，保持各段内部顺序。
// 例如 1->2->3->4->5 向右旋转 2 位后为 4->5->1->2->3。
// 先求长度 len 并连接首尾，再在第 len - k % len 个结点后断开。
// 时间复杂度 O(len)，额外空间 O(1)。
// TODO：k == 0 应返回原头指针，当前实现却返回 nullptr。
// 原作者：zlm；原记录日期：2018-07-15。
class Solution61 {
 public:
  ListNode* rotateRight(ListNode* head, int k) {
    if (head == nullptr || k <= 0) {
      return nullptr;
    }

    // 1. 遍历求链表长度
    int len = 0;
    ListNode* pcur = head;
    ListNode* pre = head;
    while (pcur != nullptr) {
      len++;
      pre = pcur;
      pcur = pcur->next;
    }
    pre->next = head;  // 首尾连接

    // 原头结点起第 len - k % len 个结点将成为新的尾结点。
    pcur = head;
    for (int i = 1; i < len - k % len; i++) {
      pcur = pcur->next;
    }  // pcur 的后继是新头结点；k 为 len 的倍数时，新头仍是原头。

    ListNode* newHead = pcur->next;
    pcur->next = nullptr;

    return newHead;
  }
};

// 最小的 K 个数：用最大堆保留已经扫描过的元素中最小的 k 个。
// 每加入一个值，若堆中元素超过 k 个，就删除最大的值。
// 默认 priority_queue 的堆顶为最大值，堆内并非整体有序，也不保证队尾最小。
// 最后依次弹出堆顶，返回的 k 个值按从大到小排列。
// 原作者：zlm；原记录日期：2018-07-16。
class Solution112 {
 public:
  vector<int> GetLeastNumbers_Solution(vector<int> input, int k) {
    vector<int> res;
    if (k <= 0 || k > input.size()) {
      return res;
    }
    priority_queue<int> que;

    for (int i = 0; i < input.size(); i++) {
      que.push(input[i]);
      if (que.size() ==
          k + 1) {  // 超出 k 个时删除堆顶，留下当前最小的 k 个值。
        que.pop();  // 出队，即删除队首
      }
    }

    while (!que.empty()) {
      res.push_back(que.top());  // 依次出队
      que.pop();
    }
    return res;
  }
};

// 调整数组顺序，使奇数在偶数前，并保留两组各自的相对次序。
// 偶数从左向右扫描并放到双端队列尾部；奇数从右向左扫描并放到队首。
// 时间复杂度 O(n)，额外空间 O(n)。
// TODO：当前用 value % 2 == 1 判断奇数，会遗漏负奇数。
// 原作者：zlm；原记录日期：2018-07-18。
class Solution111 {
 public:
  void reOrderArray(vector<int>& array) {
    if (array.empty()) {
      return;
    }

    deque<int> dq;

    int len = array.size();

    for (int i = 0; i < len; i++) {
      if (array[i] % 2 == 0) {  // 偶数放在dq的后面
        dq.push_back(array[i]);
      }
      // 奇数倒序扫描并插入队首，两次反向操作保留原先的相对顺序。
      if (array[len - i - 1] % 2 == 1) {
        dq.push_front(array[len - 1 - i]);
      }
    }
    array.assign(dq.begin(), dq.end());  // 用队列中的稳定分组结果替换原数组。
  }
};

// 把非负整数数组拼接成数值最小的字符串。
// 例如 {3, 32, 321} 的结果为 "321323"。
// 将整数转为字符串，以 a + b < b + a 作为排序规则，再依次拼接。
// 原作者：zlm；原记录日期：2018-07-19。
class Solution114 {
 public:
  string PrintMinNumber(vector<int> numbers) {
    if (numbers.empty()) {
      return "";
    }

    // 1 数字转换成string
    int len = numbers.size();
    vector<string> strNum(len);
    for (int i = 0; i < len; i++) {
      strNum[i] = to_string(numbers[i]);
    }
    // 2 按自定义规则排序
    sort(strNum.begin(), strNum.end(), myCompare);

    // 3 拼接字符串
    string res;
    for (int i = 0; i < len; i++) {
      res += strNum[i];
    }

    return res;
  }

  // 比较两种拼接顺序，使排序后的整体拼接结果最小。
  // 对于两个字符串 A，B
  // 若 AB < BA, 则应该把A放在前面，B放在后面，即认为 A“小于”B
  // 若 AB > BA, 则应该把B放在前面, 认为 B "小于" A。
 private:
  // static 成员不依赖对象，可直接作为 sort 的比较函数。
  static bool myCompare(const string& a, const string& b) {
    return a + b < b + a;
  }
};

// 丑数：质因子只包含 2、3、5 的正整数，约定 1 为第一个丑数。
// 三个下标分别指向乘以 2、3、5 后可能生成下一个丑数的位置。
// 每次取三个候选值的最小值；所有生成该最小值的下标都要前进，以免重复。
// 原作者：zlm；原记录日期：2018-07-19。
class Solution116 {
 public:
  int GetUglyNumber_Solutiion(int index) {
    if (index < 1) {
      return 0;
    }

    vector<int> pre;   // 用来存放已有的丑数(其中按顺序存放)
    pre.push_back(1);  // 1是第一个丑数
    int i2 = 0, i3 = 0, i5 = 0;
    // i2/i3/i5 分别记录乘以 2/3/5 时下一个待使用的丑数下标。
    int tmp;

    while (pre.size() < index) {
      tmp = min(pre[i2] * 2, min(pre[i3] * 3, pre[i5] * 5));
      pre.push_back(tmp);

      // 所有产生 tmp 的候选下标都前进一步，不能写成互斥的 else if。
      // 例如 6 同时来自 3 * 2 和 2 * 3，两个下标都必须更新。
      if (pre[i2] * 2 == tmp) {
        i2++;
      }
      if (pre[i3] * 3 == tmp) {
        i3++;
      }
      if (pre[i5] * 5 == tmp) {
        i5++;
      }
    }

    // pre.size()==index
    return pre[index - 1];
  }
};

// 构建乘积数组：B[i] 等于 A 中除 A[i] 以外所有元素的乘积。
// 不用除法：先令 B[i] 保存下标小于 i 的元素乘积，再乘以下标大于 i 的元素乘积。
// 空区间的乘积为 1；时间复杂度 O(n)，除结果数组外额外空间 O(1)。
// 前提：所有中间乘积能用 int 表示。
// 原作者：zlm；原记录日期：2018-07-19。
class Solution_117 {
 public:
  vector<int> multiply(const vector<int>& A) {
    int len = A.size();
    if (len < 1) {
      return vector<int>();
    }

    vector<int> B(len);
    B[0] = 1;
    int tmp = A[0];
    for (int i = 1; i < len; i++) {
      B[i] = tmp;
      tmp *= A[i];
    }

    // tmp 保存当前位置右侧所有元素的乘积，与 B[i] 中的左侧乘积相乘。
    tmp = 1;
    for (int i = len - 1; i >= 0; i--) {
      B[i] *= tmp;
      tmp *= A[i];
    }

    return B;
  }
};

// 数组中的逆序对：统计 i < j 且 data[i] > data[j] 的下标对。
// 归并两个有序区间时，若左侧当前值大于右侧值，左侧剩余值均构成逆序对。
// 每次合并只统计跨越两个区间的数对，区间内部的数对由递归处理。
// TODO：右半段递归起点应为 m + 1，当前的 m 会导致长度为 2 时递归不终止。
// TODO：cnt 未在每次调用前重置，且累加时未防止 int 溢出。
// 原作者：zlm；原记录日期：2018-07-19。
class Solution_51 {
 private:
  vector<int> tmp;  // 复用合并缓冲区，避免每次 merge 都创建临时数组。
  int cnt = 0;      // 累计逆序对数；当前未在每次公开调用前重置。

 public:
  int InversePairs(vector<int> data) {
    if (data.size() <= 1) {
      return 0;
    }

    tmp.resize(data.size());
    int low = 0, high = data.size() - 1;
    // cnt的累计在 merge 函数中进行
    mergeSort(data, low, high);

    return (cnt % 1000000007);
  }

  // 归并排序相关的函数
  void mergeSort(vector<int>& data, int low, int high) {
    if (low < high) {
      int m = (low + high) / 2;  // 把序列分成两段，递归调用mergeSort
      mergeSort(data, low, m);
      mergeSort(data, m, high);

      // 合并要求 data[low..m] 和 data[m+1..high] 各自有序，递归边界尚待修正。
      merge(data, low, m, high);
    }
  }
  // 在合并子序列的过程中，统计逆序对的个数
  void merge(vector<int>& a, int low, int mid, int high) {
    int i = low, j = mid + 1, k = low;
    while (i <= mid && j <= high) {
      if (a[i] <= a[j]) {
        tmp[k++] = a[i++];
      } else {
        tmp[k++] = a[j++];  // 当前右侧值小于所有尚未合并的左侧值。
        this->cnt += mid - i + 1;
      }
    }
    while (i <= mid) {
      tmp[k++] = a[i++];
    }
    while (j <= high) {
      tmp[k++] = a[j++];
    }

    // 把合并后的结果复制到原数组
    for (k = low; k <= high; k++) {
      a[k] = tmp[k];
    }
  }
};

// 包含 min 函数的栈：data 保存全部元素，Min 保存历次最小值。
// 新值小于或等于当前最小值时，同时压入 Min；等值也入栈以保留重复次数。
// 弹出的值等于 Min.top() 时，同时弹出 Min；查询最小值读取 Min.top()。
// TODO：pop 后未同步 minimum，之后 push 可能破坏 Min 的记录。
// 空栈的 top/min 返回 -1，无法与实际值 -1 区分。
// 原作者：zlm；原记录日期：2018-07-19。
class Solution_min_Stack {
 private:
  stack<int> data, Min;
  // Min.top() 为当前最小值；minimum 是缓存值，pop 后尚未同步。
  int minimum;

 public:
  void push(int value) {
    if (data.empty() && Min.empty()) {
      data.push(value);
      Min.push(value);
      minimum = value;
    } else {
      if (value <= minimum) {
        data.push(value);
        Min.push(value);
        minimum = value;
      } else {
        data.push(value);
      }
    }
  }

  void pop() {
    if (!data.empty()) {
      if (data.top() == Min.top()) {
        data.pop();
        Min.pop();
      } else {
        data.pop();
      }
    }
  }

  int top() {
    if (data.empty()) {
      return -1;
    }
    return data.top();
  }

  int min() {
    if (Min.empty()) {
      return -1;
    }
    return Min.top();
  }
};

// 表示数值的字符串：依次解析可选符号、数字、小数部分和指数部分。
// 参数需为以空字符结尾的字符串；指数部分允许符号，但必须包含数字。
// TODO：入口条件中的 strlen(string) 会拒绝所有非空字符串。
// TODO：小数部分和指数部分尚未完整检查是否实际读到了数字。
// 原作者：zlm；原记录日期：2018-07-19。
class Solution_numeric_str {
 public:
  bool isNumeric(char* string) {
    if (string == nullptr || strlen(string)) {
      return false;
    }

    int index = 0;
    int len = strlen(string);

    // 1. 判断符号
    if (string[index] == '+' || string[index] == '-') {
      index++;
    }

    // 空串或只含符号的字符串均不构成数值。
    if (index == len) {
      return false;
    }

    // 2. 扫描数字部分
    index = scanDigits(string, index);

    if (index < strlen(string)) {
      // 3. 如果有小数
      if (string[index] == '.') {
        index++;

        index = scanDigits(string, index);

        // 4. 如果有指数表示的形式
        if (index < strlen(string)) {
          if (string[index] == 'e' || string[index] == 'E') {
            index++;
            return isExponential(string, index);
          }
          return false;
        }
        return true;
      } else if (string[index] == 'e' || string[index] == 'E') {
        // 如果没有小数，且有指数形式
        index++;
        return isExponential(string, index);
      }
      return false;
    }
    return true;
  }

  bool isExponential(char* str, int index) {
    if (index < strlen(str)) {
      // 如果是符号，移动至下一个
      if (str[index] == '+' || str[index] == '-') {
        index++;
      }

      index = scanDigits(str, index);

      if (index == strlen(str)) {
        return true;
      }

      return false;
    }

    return false;
  }
  int scanDigits(char* str, int index) {
    while (index < strlen(str) && str[index] >= '0' && str[index] <= '9') {
      index++;
    }
    return index;
  }
};

// 239. 滑动窗口最大值：用双端队列保存候选元素的下标。
// 下标从队首到队尾递增，对应值严格递减；队首对应当前窗口最大值。
// 入队前移除队尾所有不大于新值的元素，它们更早过期且不会比新值更优。
// 窗口右移时移除过期的队首；每个下标至多入队、出队一次，时间复杂度 O(n)。
// 原作者：zlm；原记录日期：2018-07-20。
class Solution239 {
 public:
  vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    // 窗口长度必须在 [1, nums.size()] 范围内。
    if (nums.empty() || k > nums.size() || k <= 0) {
      return vector<int>();
    }

    deque<int> dq;  // 保存候选元素的下标，值严格递减。

    vector<int> res;

    int n = nums.size();
    // 先处理前 k 个元素，建立第一个窗口的候选队列。
    // 例如值为 {2, 3, 4}、k == 3 时，最终只保留下标 2（对应值 4）。

    int i = 0;
    for (; i < k; i++) {
      // 移除值小于或等于新值的队尾下标。
      while (!dq.empty() && nums[i] >= nums[dq.back()]) {
        dq.pop_back();
      }

      dq.push_back(i);
    }  // 首窗口的候选队列已就绪，不一定只含一个下标。

    res.push_back(nums[dq.front()]);

    // i 为新窗口的最右侧下标，接着处理剩余 n - k 个窗口。
    for (int i = k; i < n; i++) {
      // 首先判断当前队头是否处在当前窗口内
      if (i - dq.front() >= k) {
        dq.pop_front();
      }
      while (!dq.empty() && nums[i] >= nums[dq.back()]) {
        dq.pop_back();
      }
      dq.push_back(i);

      // 队首下标对应的值是当前窗口最大值。
      res.push_back(nums[dq.front()]);
    }

    return res;
  }
};

// 数据流中的中位数：最大堆保存较小的一半，最小堆保存较大的一半。
// 两堆均非空时满足 maxHeap.top() <= minHeap.top()，因此两半的大小关系有序。
// 总数为奇数时，minHeap 多一个元素，中位数为其堆顶；偶数时取两堆顶平均值。
// 插入后总数为奇数：先把 num 放入 maxHeap，再把其堆顶移入 minHeap。
// 插入后总数为偶数：先把 num 放入 minHeap，再把其堆顶移入 maxHeap。
// GetMedian 要求至少插入一个值；偶数时两堆顶的 int 加法尚未防止溢出。
// 原作者：zlm；原记录日期：2018-07-20。
class Solution_63 {
 private:
  // 保存较小的一半，top() 返回其中最大值。
  priority_queue<int, vector<int>, less<int>> maxHeap;
  // 保存较大的一半，top() 返回其中最小值。
  priority_queue<int, vector<int>, greater<int>> minHeap;
  int cnt = 0;  // 已插入的元素个数。

 public:
  void Insert(int num) {
    cnt++;  // 统计当前读取的数据流个数
    int tmp;

    if (cnt % 2 == 0) {  // 插入后总数为偶数，将较大一半中的最小值移到较小一半。
      minHeap.push(num);
      maxHeap.push(minHeap.top());

      minHeap.pop();
    } else if (cnt % 2 == 1) {
      maxHeap.push(num);
      minHeap.push(maxHeap.top());

      maxHeap.pop();
    }
  }

  double GetMedian() {
    if (cnt % 2 == 0) {
      return (minHeap.top() + maxHeap.top()) / 2.0;
    } else {
      return minHeap.top();
    }
  }
};

// 机器人的运动范围：从 (0, 0) 出发，每次向上下左右移动一格。
// 只允许进入行、列坐标的十进制数位和不超过 threshold 的格子。
// 深度优先搜索统计可达格子，访问标记保留到搜索结束，避免重复计数。
// TODO：在检查 rows/cols 前已分配访问数组，负尺寸未被安全处理。
// 原作者：zlm；原记录日期：2018-07-20。
class Solution_64 {
 public:
  int movingCount(int threshold, int rows, int cols) {
    // 标记网格是否被访问过
    vector<vector<bool>> isVisited(rows, vector<bool>(cols, false));

    int cnt = 0;

    if (threshold < 0 || rows <= 0 || cols <= 0) {
      return cnt;
    }

    cnt = backTracking(threshold, 0, 0, rows, cols, isVisited);

    return cnt;
  }

  // 从 (r, c) 开始深度优先搜索，返回本次新访问的格子数。
  int backTracking(int threshold, int r, int c, int rows, int cols,
                   vector<vector<bool>>& isVisited) {
    int cnt = 0;
    // 越界或数位和超限时，不进入该格子。
    if (!check(threshold, r, c, rows, cols)) {  // 坐标(r, c)无法进入返回 0
      return 0;
    }

    // 对尚未访问的合法格子计数，再递归搜索四个方向。
    // 返回时保留访问标记，同一格子只计数一次。
    if (!isVisited[r][c]) {
      isVisited[r][c] = true;
      cnt = 1 + backTracking(threshold, r, c - 1, rows, cols, isVisited) +
            backTracking(threshold, r, c + 1, rows, cols, isVisited) +
            backTracking(threshold, r - 1, c, rows, cols, isVisited) +
            backTracking(threshold, r + 1, c, rows, cols, isVisited);
    }
    return cnt;
  }

  // 检查 (r, c) 是否在网格内且满足数位和限制，不检查访问状态。
  bool check(int threshold, int r, int c, int rows, int cols) {
    bool flag = (r >= 0 && r < rows) && (c >= 0 && c < cols);
    if (flag && (sumOfDigit(r) + sumOfDigit(c) <= threshold)) {
      return true;
    } else {
      return false;
    }
  }

  // 计算数字各个位之和
  int sumOfDigit(int d) {
    int sum = 0;
    while (d != 0) {
      sum += d % 10;
      d = d / 10;
    }
    return sum;
  }
};

// 矩阵中的路径：寻找依次拼出 str 的相邻格子，每条路径不能重复使用格子。
// 枚举起点，匹配当前字符后递归尝试上下左右的格子；失败时撤销本次选择。
// matrix 按行存储 rows * cols 个字符；str 是以空字符结尾的字符串。
// 前提：指针有效、rows/cols 为正，且缓冲区长度满足约定；当前未检查这些条件。
// 原作者：zlm；原记录日期：2018-07-20。
class Solution_65 {
 public:
  // str是给定的路径
  bool hasPath(char* matrix, int rows, int cols, char* str) {
    // 标记矩阵中的节点是否被访问过
    vector<vector<bool>> isVisited(rows, vector<bool>(cols, false));

    // 依次尝试每个格子作为路径起点。
    int index = 0;
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        if (hasPathBackTracking(matrix, i, j, rows, cols, str, index,
                                isVisited)) {
          return true;
        }
      }
    }

    return false;
  }

  // 尝试从 (r, c) 匹配 str[index]，index 为下一个待匹配字符的下标。
  // 失败时恢复 index 和本次访问标记，成功时保留匹配完成的状态。
  bool hasPathBackTracking(char* matrix, int r, int c, int rows, int cols,
                           char* str, int& index,
                           vector<vector<bool>>& isVisited) {
    // 所有字符均已匹配，递归成功；此时无需再访问当前坐标。
    if (index == strlen(str)) {
      return true;
    }

    bool pathFinded = false;  // 是否找到路径的标志位

    if (check(matrix, r, c, rows, cols, str, index, isVisited)) {
      index++;
      isVisited[r][c] = true;

      // 递归的从[r,c]的相邻结点寻找路径
      // 下一位置：(r,c-1) (r, c+1) (r-1, c) (r+1, c)
      pathFinded = hasPathBackTracking(matrix, r, c - 1, rows, cols, str, index,
                                       isVisited) ||
                   hasPathBackTracking(matrix, r, c + 1, rows, cols, str, index,
                                       isVisited) ||
                   hasPathBackTracking(matrix, r - 1, c, rows, cols, str, index,
                                       isVisited) ||
                   hasPathBackTracking(matrix, r + 1, c, rows, cols, str, index,
                                       isVisited);

      // 四个方向都无法完成剩余匹配时，撤销当前字符和访问标记。
      // index 已在进入当前结点后递增，此时指向下一个待匹配字符。
      if (!pathFinded) {
        index--;
        isVisited[r][c] = false;
      }
    }
    return pathFinded;
  }

  // 当前格子必须在边界内、匹配 str[index]，且未出现在本次路径中。
  bool check(char* matrix, int r, int c, int rows, int cols, char* str,
             int& index, vector<vector<bool>>& isVisited) {
    bool flag = (r >= 0 && r < rows) && (c >= 0 && c < cols) &&
                (str[index] == matrix[r * cols + c]) && !isVisited[r][c];

    return flag;
  }
};

// 树的子结构：判断 B 的结点值和父子关系能否在 A 的某处完整匹配。
// 依次尝试从 A 的根结点、左子树或右子树开始匹配 B。
// 根值相同只是一项必要条件，还需递归匹配 B 中存在的左右子结点。
// 约定空树 B 不是任何树的子结构；匹配时 A 可以有 B 中未要求的额外结点。
// 原作者：zlm；原记录日期：2018-07-20。
class Solution_subtree {
 public:
  bool HasSubtree(TreeNode* pRoot1, TreeNode* pRoot2) {
    if (pRoot1 == nullptr || pRoot2 == nullptr) {
      return false;
    }
    return isSubtreeWithSameRoot(pRoot1, pRoot2) ||
           HasSubtree(pRoot1->left, pRoot2) ||
           HasSubtree(pRoot1->right, pRoot2);
  }
  // 从当前两个结点开始，逐个匹配 p2 中存在的结点及其父子关系。
  bool isSubtreeWithSameRoot(TreeNode* p1, TreeNode* p2) {
    if (p2 == nullptr) {  // p2树中的结点判断完毕
      return true;
    }
    if (p1 == nullptr) {
      return false;
    }

    // p1和p2均不为空
    if (p1->val != p2->val) {
      return false;
    } else {
      return isSubtreeWithSameRoot(p1->left, p2->left) &&
             isSubtreeWithSameRoot(p1->right, p2->right);
    }
  }
};

// 将二叉搜索树原地转换为按值递增的双向链表。
// 中序遍历时，把当前结点与已处理链表的尾结点连接，再处理右子树。
// left 改作前驱，right 改作后继；转换后不再保留原树结构。
// TODO：head 和 pLastNode 在入口未重置，同一对象不能直接复用于另一棵树。
// 原作者：zlm；原记录日期：2018-07-21。
class Solution_bst_to_LinkedList {
 private:
  TreeNode* pLastNode = nullptr;  // 已经排序好的链表中的最后一个结点指针
  TreeNode* head = nullptr;

 public:
  TreeNode* Convert(TreeNode* pRootOfTree) {
    if (pRootOfTree == nullptr) {
      return nullptr;
    }
    InorderConvert(pRootOfTree);
    return head;
  }

  // 采用中序遍历的思路进行转换
  void InorderConvert(TreeNode* root) {
    if (root == nullptr) {
      return;  // 递归终止条件
    }

    // 1. 先对左子树递归处理，形成排序链表
    InorderConvert(root->left);

    // 2. 将左子树链表最后一个结点 pLastNode 与当前 root 连接
    root->left = pLastNode;

    if (pLastNode == nullptr) {
      // 当 pLastNode 为空时，已经遍历至最左边的结点，即链表的第一个结点
      pLastNode = root;
    } else {                    // pLastNode!=null，指向已排序链表的最后一个结点
      pLastNode->right = root;  // 先把原尾结点的后继接到 root。
      pLastNode = root;
    }
    if (head == nullptr) {  // 首次赋值时记录最左结点，作为链表头。
      head = root;
    }

    // 此时 pLastNode 指向 root；继续处理中序顺序中的右子树。
    InorderConvert(root->right);
  }
};

// 删除单链表中的指定结点，node 必须属于 pListHead 指向的链表。
// 非尾结点：复制后继的值并删除后继，时间 O(1)，node 指向的对象仍然存在。
// 唯一结点：删除后将头指针置空；其他尾结点需遍历寻找前驱，时间 O(n)。
// 被删除的结点必须由 new 分配且允许本函数释放，调用方持有的相关指针可能失效。
// 原作者：zlm；原记录日期：2018-07-29。
void DeleteNode(ListNode*& pListHead, ListNode* node) {
  if (pListHead == nullptr || node == nullptr) {
    return;
  }

  if (node->next != nullptr) {
    ListNode* pNext = node->next;  // node在前，pNext在后
    node->val = pNext->val;

    node->next = pNext->next;
    // 删除pNext
    delete pNext;
    pNext = nullptr;
  } else if (node == pListHead) {  // 无后继且为头结点，说明链表仅有这一个结点。
    delete node;
    pListHead = node = nullptr;
  } else {  // 待删除节点为尾节点
    ListNode* pTemp = pListHead;
    while (pTemp->next != node) {
      pTemp = pTemp->next;
    }

    pTemp->next = nullptr;
    delete node;
    node = nullptr;
  }
}

// 翻转句子中的单词顺序：先反转整个字符串，再逐个反转单词。
// 只把普通空格 ' ' 作为分隔符，去掉首尾空格并将单词间空格合并为一个。
// 当前逐字符调用 erase，最坏时间复杂度 O(n^2)，额外空间 O(1)。
// TODO：全为空格的输入会在删除后变成空串，此时调用 s.back() 不满足前提。
// 原作者：zlm；原记录日期：2018-10-04。
void reverseWords(string& s) {
  if (s.size() < 1) {
    return;
  }

  // 整体反转
  reverse(s.begin(), s.end());

  int start = 0, j = 0;  // [start, j) 保存已经扫描到的当前单词。
  int len = s.size();

  while (j < len) {
    // 遇到空格时，结束当前单词或删除多余的分隔空格。
    if (s[j] == ' ') {
      if (j != start) {  // [start, j) 是刚扫描完的非空单词。
        reverse(s.begin() + start, s.begin() + j);
        start = ++j;  // 跳过一个分隔空格，后续循环继续清理多余空格。
      } else {        // j == start,表明此处为空格，应删除之
        // 删除当前空格后，后续字符左移；保持 j 不变以继续检查移入的字符。
        s.erase(j, 1);
        --len;
      }
    } else {
      // 当前字符不是空格，继续扫描单词。
      ++j;
    }
  }
  // 若反转后的句子末尾有空格，循环结束时 start == j == len。
  // 若末尾没有空格，则最后一个单词尚未反转。
  reverse(s.begin() + start, s.begin() + j);

  // 去除句子最右边的空格
  if (s.back() == ' ') {
    s.pop_back();
  }
}

// 演示后置自增：a 得到自增前的值 0，i 随后变为 1。
int main() {
  int i = 0;
  int a = i++;
  cout << a << " " << i << endl;
  return 0;
}
