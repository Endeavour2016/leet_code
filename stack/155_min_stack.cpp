/*
 * @Author: zlm 
 * @Date: 2021-07-10 13:44:15 
 * @Last Modified by: zlm
 * @Last Modified time: 2021-07-10 14:22:55
 */

#include "../head_file.h"
#include "../data_structure.h"
using namespace std;

/**
 * @problem: 155. Min Stack
 * @descr: 设计一个栈，要求能够在常数时间内返回栈内的最小值
 * @method: 用STL中的stack存放栈元素，同时增加一个辅助的min_stack来存放当前元素的最小值:
 * (1) 新元素入栈时，取辅助栈栈顶元素与当前新元素的最小值，插入到min_stack
 * (2) 元素出栈时，把min_stack的栈顶也一并弹出
 * (3) 任意时刻，辅助栈的栈顶始终为当前栈中的最小元素
 * 时间复杂度: O(1)
 * 空间复杂度: O(n)
 */

class MinStack {
public:
    /** initialize your data structure here. */
    MinStack() {
        // 保证第一次入栈的元素可以直接push到辅助栈
        min_stack.push(INT_MAX);
    }
    
    void push(int val) {
        elements.push(val);
        min_stack.push(std::min(val, min_stack.top()));
    }
    
    void pop() {
        elements.pop();
        min_stack.pop();
    }
    
    int top() {
        return elements.top();
    }
    
    int getMin() {
        return min_stack.top();
    }

private:
  std::stack<int> elements;
  std::stack<int> min_stack;

};

// 用std::vector实现
class MinStack2 {
public:
    /** initialize your data structure here. */
    MinStack2() {
        // 保证第一次入栈的元素可以直接push到辅助栈
        min_stack.emplace_back(INT_MAX);
    }
    
    void push(int val) {
        elements.emplace_back(val);
        min_stack.emplace_back(std::min(val, min_stack.back()));
    }
    
    void pop() {
        elements.pop_back();
        min_stack.pop_back();
    }
    
    int top() {
        return elements.back();
    }
    
    int getMin() {
        return min_stack.back();
    }

private:
  std::vector<int> elements;
  std::vector<int> min_stack;

};

/**
 * Your MinStack object will be instantiated and called as such:
 * MinStack* obj = new MinStack();
 * obj->push(val);
 * obj->pop();
 * int param_3 = obj->top();
 * int param_4 = obj->getMin();
 */

int main() {
  return 0;
}