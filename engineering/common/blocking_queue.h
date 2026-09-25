/**
 * @file blocking_queue.h
 * @brief 提供有容量限制、支持关闭后继续取出任务的阻塞队列。
 * @details 用一把互斥锁保护队列和关闭标志，用两个条件变量分别等待任务和空位。
 * @note 支持多个生产者和消费者；销毁前须结束所有访问线程，元素移动必须正常完成。
 * @see notes/engineering/02_blocking_queue.md
 */

#ifndef LEET_CODE_ENGINEERING_COMMON_BLOCKING_QUEUE_H_
#define LEET_CODE_ENGINEERING_COMMON_BLOCKING_QUEUE_H_

#include <condition_variable>
#include <cstddef>
#include <deque>
#include <mutex>
#include <utility>

// 本例按无异常模式编译，元素类型也必须遵守该约定。
// close 唤醒生产者和消费者，已接受的任务仍可取出。
template <class T>
class BlockingQueue {
 public:
  // 容量为 0 时创建无效且关闭的队列，所有入队/出队操作立即失败。
  explicit BlockingQueue(std::size_t capacity)
      : capacity_(capacity), closed_(capacity == 0) {
  }

  // 参数错误可在构造后显式检查，不会产生无限等待。
  bool valid() const {
    return capacity_ != 0;
  }

  // 队列满时等待；队列关闭后返回 false，不接受这个任务。
  bool push(T value) {
    std::unique_lock<std::mutex> lock(mutex_);
    not_full_.wait(lock, [this] {
      return closed_ || items_.size() < capacity_;
    });
    if (closed_) {
      return false;
    }
    items_.push_back(std::move(value));
    lock.unlock();
    not_empty_.notify_one();
    return true;
  }

  // 不等待空位；队列满或已关闭时立即返回 false。
  bool try_push(T value) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (closed_ || items_.size() == capacity_) {
      return false;
    }
    items_.push_back(std::move(value));
    lock.unlock();
    not_empty_.notify_one();
    return true;
  }

  // 关闭后继续取任务，直到队列为空才返回 false。
  bool pop(T& value) {
    std::unique_lock<std::mutex> lock(mutex_);
    not_empty_.wait(lock, [this] {
      return closed_ || !items_.empty();
    });
    if (items_.empty()) {
      return false;
    }
    value = std::move(items_.front());
    items_.pop_front();
    lock.unlock();
    not_full_.notify_one();
    return true;
  }

  // 可重复关闭；两类等待线程都需要被唤醒并检查关闭标志。
  void close() {
    {
      std::lock_guard<std::mutex> lock(mutex_);
      closed_ = true;
    }
    not_empty_.notify_all();
    not_full_.notify_all();
  }

 private:
  const std::size_t capacity_;
  bool closed_;  // 与 items_ 一起由 mutex_ 保护。
  std::deque<T> items_;
  std::mutex mutex_;
  std::condition_variable not_empty_;
  std::condition_variable not_full_;
};

#endif  // LEET_CODE_ENGINEERING_COMMON_BLOCKING_QUEUE_H_
