#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <pthread.h>
#include "Queue.h"

#define TRUE  1
#define FALSE 0

typedef struct {
    pthread_t *pthreads_;    // 线程池数组
    size_t size_;            // 线程池大小
    BOOL is_started_;        // 开启标志
    Queue queue_;            // 任务队列
    pthread_mutex_t mutex_;  // 互斥锁
    pthread_cond_t cond_;    // 条件变量
} pool_t;

void thread_pool_init(pool_t *pool, size_t size);  // 初始化线程池
void thread_pool_destroy(pool_t *pool);  // 销毁线程池
void thread_pool_start(pool_t *pool);    // 开启
void thread_pool_stop(pool_t *pool);     // 停止
void thread_pool_add_task(pool_t *pool, task_t task);    // 添加任务
BOOL thread_pool_get_task(pool_t *pool, task_t *task);   // 取出任务
BOOL thread_pool_is_started(const pool_t *pool);  // 线程池是否开启
size_t thread_pool_get_size(const pool_t *pool);  // 任务队列大小

#endif  /* _THREAD_POOL_H_ */
