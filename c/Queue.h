#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stddef.h>
#include <stdlib.h>

#define ERR_EXIT(m) \
    do { \
        perror(m); \
        exit(EXIT_FAILURE); \
    } while(0)

typedef int BOOL;

/* 任务属性 */
typedef struct {
    void (*thread_callback)(void *);  // 任务执行函数
    void *arg;  // 任务执行时的参数
} task_t;

/* 任务队列结点 */
typedef struct {
    task_t data_;
    struct qNode *next_;
} qNode, *pNode;

/* 任务队列 */
typedef struct {
    pNode head_;
    pNode tail_;
    size_t size_;
} Queue;


void queue_init(Queue *Q);
void queue_destroy(Queue *Q);
void queue_push(Queue *Q, task_t task);
void queue_pop(Queue *Q);
task_t queue_top(const Queue *Q);
void queue_clear(Queue *Q);
BOOL queue_is_empty(const Queue *Q);
size_t queue_size(const Queue *Q);


#endif  /* _QUEUE_H_ */
