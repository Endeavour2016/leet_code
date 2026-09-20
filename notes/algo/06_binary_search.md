# 二分查找

## 核心思想

二分查找的本质不是“数组有序”，而是存在一个能把搜索空间分成两半的单调判定。写代码前先决定区间定义，本章统一使用闭区间 `[left, right]`。

## 闭区间模板

```cpp
int left = 0;
int right = static_cast<int>(nums.size()) - 1;
while (left <= right) {
  int middle = left + (right - left) / 2;
  if (nums[middle] == target) return middle;
  if (nums[middle] < target) left = middle + 1;
  else right = middle - 1;
}
return -1;
```

已验证实现：[704_binary_search.cpp](../../algo/binary_search/704_binary_search.cpp#L15-L41)。

## 为什么是 `left <= right`

闭区间在 `left == right` 时仍有一个候选元素，必须继续判断。每次更新使用 `middle + 1` 或 `middle - 1`，因为 `middle` 已经检查过。

```text
下标:  0  1  2  3  4  5
数值: -1  0  3  5  9 12
       L       M        R
```

## 边界二分

找“第一个满足条件的位置”时，不要在命中后立即返回，而是记录答案并继续向左收缩：

```cpp
int answer = n;
while (left <= right) {
  int middle = left + (right - left) / 2;
  if (check(middle)) {
    answer = middle;
    right = middle - 1;
  } else {
    left = middle + 1;
  }
}
```

旋转数组的关键是每轮至少有一半有序，再判断目标是否落在这一半。矩阵、平方根和“第 K 小”也可转化为单调判定；平方根示例：[math_solution.cc](../../algo/math/math_solution.cc#L20-L40)。

## 易错点

- 空数组时 `size() - 1` 的无符号下溢：先转为有符号整数。
- `middle = (left + right) / 2` 可能溢出。
- 混用闭区间与半开区间更新规则。
- 答案二分时不会证明 `check(x)` 的单调性。

默写检查：区间含义、循环条件、更新是否排除 `middle`、未找到时返回什么。
