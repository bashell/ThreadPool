#include <errno.h>
#include <string.h>
#include "ThreadPool.h"

/*
 * threads_: 由std::vector<pthread_t>初始化
 * queue_:   由queue<Task>初始化
 * mutex_:   由MutexLock::MutexLock()初始化
 */
ThreadPool::ThreadPool()
    : full_(mutex_),  // 调用Condition::Condition(MutexLock&)
      empty_(mutex_),
      queueSize_(DEFAULT_QUEUE_SIZE), 
      poolSize_(DEFAULT_POOL_SIZE),
      isStarted_(false) { 

}


ThreadPool::ThreadPool(size_t queueSize, size_t poolSize)
    : full_(mutex_),
      empty_(mutex_),
      queueSize_(queueSize), 
      poolSize_(poolSize),
      isStarted_(false) { 
      
}


ThreadPool::~ThreadPool() {
  if(isStarted_)  // 析构前先停止运行线程池
    pool_destroy();
}


void *start_thread(void *arg) {
  ThreadPool *tp = static_cast<ThreadPool *>(arg);
  tp -> run();
  return NULL;
}


void ThreadPool::pool_init() {
  if(isStarted_ == true) 
    return ;
  isStarted_ = true;
  int res;
  for(size_t i = 0; i != poolSize_; ++i) {
    pthread_t tid;
    res = pthread_create(&tid, NULL, start_thread, (void*)this);
    if(res != 0) 
      std::cerr << "pthread_create failed" << std::endl;
    threads_.push_back(tid);
  }
  std::cout << poolSize_ << " threads created by the ThreadPool." << std::endl;
}


void ThreadPool::pool_destroy() {
  if(isStarted_ == false)
    return ;
  mutex_.lock();
  isStarted_ = false;
  full_.signal_all();  // 激活所有等待的线程
  mutex_.unlock();
   
  std::cout << "Broadcasting STOP signal to all threads" << std::endl;
  int ret;
  for(size_t i = 0; i != poolSize_; ++i) {
    ret = pthread_join(threads_[i], NULL);
  }
  std::cout << "All threads returned." << std::endl;
  while(!queue_.empty())
    queue_.pop();
  std::cout << "TaskQueue has been cleared." << std::endl;
}


void ThreadPool::addTask(const Task &task) {
  mutex_.lock();
  while(queue_.size() >= queueSize_)
    empty_.wait();
  queue_.push(task);   // 添加任务到队列
  full_.signal_one();  // 通知消费者取任务
  mutex_.unlock();
}


ThreadPool::Task ThreadPool::getTask() {
  mutex_.lock();
  while(queue_.empty() && isStarted_)
    full_.wait();
  Task task;  // task默认为一个空函数
  if(!queue_.empty()) {
    task = queue_.front();
    queue_.pop();
    empty_.signal_one();  // 通知生产者添加任务
  }
  mutex_.unlock();
  return task;
}


void ThreadPool::run() {
  while(isStarted_) {
    Task task = getTask();
    if(task)
      task();
  }
}
