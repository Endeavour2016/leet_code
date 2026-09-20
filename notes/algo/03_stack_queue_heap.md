# 栈、队列与堆

## 栈：处理最近尚未完成的事情

栈是“后进先出”。括号匹配时，栈顶就是最近一个尚未闭合的左括号；表达式求值时，栈保存暂时不能结算的状态。括号实现入口：[20_valid_parentheses.cpp](../../algo/stack/20_valid_parentheses.cpp#L22-L45)。

```cpp
stack<char> pending;
for (char ch : text) {
  if (is_left(ch)) pending.push(ch);
  else {
    if (pending.empty() || !matches(pending.top(), ch)) return false;
    pending.pop();
  }
}
return pending.empty();
```

## 单调队列：窗口最大值

队列中存下标，并让对应值从队头到队尾单调递减。新元素入队前，删除队尾所有不可能再成为最大值的元素；队头过期时弹出。

```text
窗口值:  [1, 3, -1, -3, 5]
候选队列: 3, -1, -3
新来 5:  -3、-1、3 都永久失去竞争资格 -> 只留 5
```

代表实现：[slide_window_max.cpp](../../algo/queue/slide_window_max.cpp#L29-L69)。每个下标最多入队、出队各一次，因此时间为 `O(n)`。

## 堆：只关心动态极值

Top K 不需要全排序。维护大小为 `k` 的小顶堆：堆顶是当前 K 个最大值中最小的那个，也是最应该被替换的候选。

```cpp
priority_queue<int, vector<int>, greater<int>> top_k;
for (int x : nums) {
  top_k.push(x);
  if (top_k.size() > k) top_k.pop();
}
```

仓库示例：[find_kth_largest.cpp](../../algo/top_k/find_kth_largest.cpp#L22-L37)。复杂度为 `O(n log k)`，空间为 `O(k)`。

## 选择口诀

| 需求 | 结构 |
|---|---|
| 最近一个未完成状态 | 栈 |
| 按进入顺序处理 | 队列 |
| 窗口极值且元素会过期 | 单调双端队列 |
| 动态最大/最小、Top K | 堆 |

易错点：堆比较器方向与直觉相反；单调队列必须存下标才能判断过期；栈题最后仍需检查是否为空。
