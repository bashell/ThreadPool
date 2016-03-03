#include "ThreadPool.h"


void thread_pool_init(pool_t *pool, size_t size) {
    int res;
    if((pool->pthreads_ = (pthread_t *)malloc(sizeof(pthread_t) * size)) == NULL)
        ERR_EXIT("malloc falied");
    pool->size_ = size;
    pool->is_started_ = FALSE;
    queue_init(&pool->queue_);
    if((res = pthread_mutex_init(&pool->mutex_, NULL)) != 0)
        ERR_EXIT("mutex initialized failed");
    if((res = pthread_cond_init(&pool->cond_, NULL)) != 0)
        ERR_EXIT("cond initialized failed");
}

void thread_pool_destroy(pool_t *pool) {
    thread_pool_stop(pool);
    free(pool->pthreads_);
    queue_destroy(&pool->queue_);
    pthread_mutex_destroy(&pool->mutex_);
    pthread_cond_destroy(&pool->cond_);
}

void *thread_pool_threadfunc(void *arg) {
    pool_t *pool = (pool_t *)arg;
    while(1) {
        task_t task;
        BOOL ret = thread_pool_get_task(pool, &task);
        if(ret == TRUE)  // 成功获取任务
            task.thread_callback(task.arg);  // 执行任务
        else
            break;  // 获取任务失败, 线程池已经停止
    }
}

void thread_pool_start(pool_t *pool) {
    if(pool->is_started_ == FALSE) {
        pool->is_started_ = TRUE;
        int i, res;
        for(i = 0; i < pool->size_; ++i) {
            if((res = pthread_create(&pool->pthreads_[i], NULL, thread_pool_threadfunc, pool)) != 0)
                ERR_EXIT("Thread creation failed");
        }
    }
}

void thread_pool_stop(pool_t *pool) {
    if(pool->is_started_ == TRUE) {
        pool->is_started_ = FALSE;
        pthread_cond_broadcast(&pool->cond_);  // 唤醒所有等待该条件的线程
        int i, res;
        for(i = 0; i < pool->size_; ++i) {
            if((res = pthread_join(pool->pthreads_[i], NULL)) != 0)  // 等待线程终止
                ERR_EXIT("Thread join failed");
        }
        queue_clear(&pool->queue_);  // 所有线程终止后, 清除任务队列
    }
}

/* 一次添加一个任务 */
void thread_pool_add_task(pool_t *pool, task_t task) {
    pthread_mutex_lock(&pool->mutex_);
    queue_push(&pool->queue_, task);
    pthread_cond_signal(&pool->cond_);
    pthread_mutex_unlock(&pool->mutex_);
}

/* 一次处理一个任务 */
BOOL thread_pool_get_task(pool_t *pool, task_t *task) {
    pthread_mutex_lock(&pool->mutex_);
    /*
     * 两种唤醒方式:
     * 1. 任务到来, 被signal唤醒
     * 2. 线程池即将关闭, 被broadcast唤醒
     */
    while(queue_is_empty(&pool->queue_) == TRUE && pool->is_started_ == TRUE)
        pthread_cond_wait(&pool->cond_, &pool->mutex_);
    if(pool->is_started_ == FALSE) {  // 线程池关闭情况
        pthread_mutex_unlock(&pool->mutex_);
        return FALSE;
    } else {  // 任务到来情况
        *task = queue_top(&pool->queue_);
        queue_pop(&pool->queue_);
        pthread_mutex_unlock(&pool->mutex_);
        return TRUE;
    }
}

BOOL thread_pool_is_started(const pool_t *pool) {
    return pool->is_started_ == TRUE;
}

size_t thread_pool_get_size(const pool_t *pool) {
    return pool->size_;
}
