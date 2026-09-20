# 数组、字符串与哈希

## 核心思想

数组的优势是按下标访问；哈希表的优势是用额外空间换近似 `O(1)` 查询。题目出现“是否出现过、出现次数、两数关系、连续子数组和”时，优先考虑哈希；出现“原地修改”时，先明确有效区间。

生活化理解：数组像一排有编号的储物柜，哈希表像姓名到柜号的登记表。

## 三个高频模型

### 1. 计数哈希

```cpp
unordered_map<int, int> count;
for (int x : nums) ++count[x];
```

若字符集固定且很小，优先用数组计数，常数更小。仓库中的有效字母异位词使用 26 个计数槽：[242_valid_anagram.cc](../../algo/string_and_array/242_valid_anagram.cc#L17-L34)。

### 2. 前缀和 + 哈希

若 `prefix[i] - prefix[j] == k`，就把问题改写为“此前出现过多少个 `prefix[i] - k`”。

```cpp
unordered_map<int, int> frequency{{0, 1}};
int prefix = 0, answer = 0;
for (int x : nums) {
  prefix += x;
  answer += frequency[prefix - k];
  ++frequency[prefix];
}
```

关键是先放入 `{0, 1}`，它代表从下标 0 开始的子数组。仓库中已有朴素与哈希两种写法，但目前同名重定义，仍属待迁移代码：[lcr_solution.cc](../../algo/LCR/lcr_solution.cc#L69-L119)。

### 3. 区间合并

先按左端点排序；扫描时只需比较“当前区间左端点”和“结果最后一个区间右端点”。

```text
已合并: [1--------6]
当前:        [4---------9]
结果:   [1-------------9]
```

代表实现：[56_merge_intervals.cpp](../../algo/string_and_array/56_merge_intervals.cpp#L22-L44)。

## 复杂度与易错点

| 模型 | 时间 | 空间 | 常见错误 |
|---|---:|---:|---|
| 数组计数 | `O(n + K)` | `O(K)` | 忘记字符范围 |
| 哈希计数 | 平均 `O(n)` | `O(n)` | 使用 `operator[]` 意外插入键 |
| 前缀和 | `O(n)` | `O(n)` | 更新次数早于查询，导致重复计数 |
| 区间合并 | `O(n log n)` | `O(n)` | 没有先排序、端点相等处理错误 |

## 复习清单

- 能解释为什么前缀和把“连续区间”变成两个前缀的差。
- 能在数组计数与 `unordered_map` 之间做选择。
- 能手写区间合并，并覆盖空输入、单区间、完全包含和首尾相接。
