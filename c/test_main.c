#include <stdio.h>
#include "Queue.h"
#include "ThreadPool.h"

void task_func(void *arg) {
    int t = (int)arg;
    printf("Execute Task: %d\n", t * t);
}

int main()
{
    pool_t pool;
    thread_pool_init(&pool, 5);
    thread_pool_start(&pool);

    srand(10086);
    while(1) {
        task_t task;
        task.thread_callback = task_func;
        task.arg = (void *)(rand() % 100);
        thread_pool_add_task(&pool, task);
        printf("ThreadPool size: %d, TaskQueue size: %d\n", (int)pool.size_, (int)pool.queue_.size_);
    }
    thread_pool_stop(&pool);
    thread_pool_destroy(&pool);
    return 0;
}
