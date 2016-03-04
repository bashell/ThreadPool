#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <iostream>
#include <functional>
#include <queue>
#include <vector>
#include <pthread.h>
#include "CondVar.h"
#include "MutexLock.h"

#define DEFAULT_QUEUE_SIZE 10
#define DEFAULT_POOL_SIZE 5


class ThreadPool {
 public:
  using Task = std::function<void()>;
  ThreadPool();
  ThreadPool(size_t, size_t);
  ~ThreadPool();

  void pool_init();      // 初始化线程池
  void pool_destroy();   // 销毁线程池
  void addTask(const Task&);  // 添加任务
  Task getTask(); 
  void run();

 private:
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool &operator=(const ThreadPool&) = delete;
 
 private:
  std::vector<pthread_t> threads_;  // 线程池
  std::queue<Task> queue_;          // 任务队列
  mutable MutexLock mutex_;
  Condition full_;    
  Condition empty_;   
  size_t queueSize_;  // 任务队列大小
  size_t poolSize_;   // 线程池大小
  bool isStarted_;    // 线程池是否开启标志
};


#endif  /* _THREAD_POOL_H_ */
