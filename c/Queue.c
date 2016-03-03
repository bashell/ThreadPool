#include <assert.h>
#include "Queue.h"

void queue_init(Queue *Q) {
    Q->head_ = NULL;
    Q->tail_ = NULL;
    Q->size_ = 0;
}

void queue_destroy(Queue *Q) {
    queue_clear(Q);
}

void queue_push(Queue *Q, task_t data) {
    pNode p = (pNode)malloc(sizeof(qNode));
    if(NULL == p) 
        ERR_EXIT("malloc failed");
    p->data_ = data;
    p->next_ = NULL;
    if(queue_is_empty(Q)) {
        Q->head_ = Q->tail_ = p;
    } else {
        Q->tail_->next_ = p;
        Q->tail_ = p;
    }
    ++Q->size_;
}

void queue_pop(Queue *Q) {
    assert(!queue_is_empty(Q));
    pNode p = Q->head_;
    Q->head_ = Q->head_->next_;
    free(p);
    p = NULL;
    --Q->size_;
}

task_t queue_top(const Queue *Q) {
    assert(!queue_is_empty(Q));
    return Q->head_->data_;
}

void queue_clear(Queue *Q) {
    while(!queue_is_empty(Q))
        queue_pop(Q);
}

BOOL queue_is_empty(const Queue *Q) {
    return NULL == Q->head_;
}

size_t queue_size(const Queue *Q) {
    return Q->size_;
}
