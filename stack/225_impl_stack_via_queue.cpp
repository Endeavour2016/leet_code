/**
 * @Author: zlm 
 * @Date: 2024-01-29
 * @Last Modified by: zlm
 * @Last Modified time: 2024-01-28
 */

#include "../head_file.h"
using namespace std;

/**
 * @problem: 225. Implement Stack using Queues
 * @descr: 请你仅使用两个队列实现一个后入先出（LIFO）的栈，并支持普通栈的全部四种操作（push、top、pop 和 empty）
 * 你只能使用队列的基本操作 —— 也就是 push to back、peek/pop from front、size 和 is empty 这些操作。
 * 你所使用的语言也许不支持队列。 你可以使用 list（列表）或者 deque（双端队列）来模拟一个队列 , 只要是标准的队列操作即可。
 * @method: 根据栈的LIFO特点分析数据入栈后逻辑顺序和实际在队列中存储顺序，比如入栈数据为[1, 2, 3]，为保证出栈时
 * 后入的先出，则数据在队列中实际存储顺序应为[3, 2, 1], 这样出队顺序为 [3，2, 1]，也就达到了出栈的效果；
 * 有新数据4入栈时，为保证4可以先出栈，需要把已存的数据[3, 2, 1] 先依次插入另一个队列，当前队列插入4，
 * 然后把另一个队列的数据依次出队到当前队列，使得当前队列的数据为[4, 3, 2, 1]
 */

class MyStack {
public:
    MyStack() {
    }

    void push(int x) {
        if (m_que_.empty()) {
            m_que_.push(x);
            return;
        }

        // 队列不为空的情况下，比如已存在数据[3, 2, 1]
        // 需要把数据先依次出队保存到辅助队列；主队列插入新元素x；
        // 辅助队列中的元素出队插入到主队列
        while (!m_que_.empty()) {
            int data = m_que_.front();
            assist_que_.push(data);
            m_que_.pop();
        }
        m_que_.push(x);
        while (!assist_que_.empty()) {
            m_que_.push(assist_que_.front());
            assist_que_.pop();
        }
    }

    int pop() {
        if (m_que_.empty()) {
            return -1;
        }
        int data = m_que_.front();
        m_que_.pop();
        return data;
    }

    int top() {
        if (m_que_.empty()) {
            return -1;
        }
        return m_que_.front();
    }

    bool empty() {
        return m_que_.empty();
    }

private:
    std::queue<int> m_que_;
    std::queue<int> assist_que_;
};

/**
 * Your MyStack object will be instantiated and called as such:
 * MyStack* obj = new MyStack();
 * obj->push(x);
 * int param_2 = obj->pop();
 * int param_3 = obj->top();
 * bool param_4 = obj->empty();
 */