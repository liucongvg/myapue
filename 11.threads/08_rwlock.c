#include <pthread.h>
#include <stdlib.h>

struct job {
    struct job* prev;
    struct job* next;
    pthread_t job_tid;
};

struct job_queue {
    struct job* head;
    struct job* tail;
    pthread_rwlock_t queue_lock;
};

int job_queue_init(struct job_queue* q)
{
    struct job* head = malloc(sizeof(struct job));
    struct job* tail = malloc(sizeof(struct job));
    head->prev = NULL;
    tail->next = NULL;
    head->next = tail;
    tail->prev = head;
    int res = pthread_rwlock_init(&q->queue_lock, NULL);
    if (res != 0)
        return res;
    return 0;
}

int job_queue_destroy(struct job_queue* q)
{
    pthread_rwlock_wrlock(&q->queue_lock);
    free(q->head);
    q->head = NULL;
    free(q->tail);
    q->tail = NULL;
    pthread_rwlock_unlock(&q->queue_lock);
    int res = pthread_rwlock_destroy(&q->queue_lock);
    if (res != 0)
        return res;
    return 0;
}

void job_insert_front(struct job_queue* q, struct job* j)
{
    pthread_rwlock_wrlock(&q->queue_lock);
    j->prev = q->head;
    j->next = q->head->next;
    q->head->next->prev = j;
    q->head->next = j;
    pthread_rwlock_unlock(&q->queue_lock);
}

void job_append(struct job_queue* q, struct job* j)
{
    pthread_rwlock_wrlock(&q->queue_lock);
    j->next = q->tail;
    j->prev = q->tail->prev;
    q->tail->prev->next = j;
    q->tail->prev = j;
    pthread_rwlock_unlock(&q->queue_lock);
}

void job_remove(struct job_queue* q, struct job* j)
{
    pthread_rwlock_wrlock(&q->queue_lock);
    j->prev->next = j->next;
    j->next->prev = j->prev;
    pthread_rwlock_unlock(&q->queue_lock);
}

struct job* job_find(struct job_queue* q, pthread_t tid)
{
    pthread_rwlock_rdlock(&q->queue_lock);
    struct job* current;
    for (current = q->head->next; current != q->tail; current = current->next) {
        if (current->job_tid == tid)
            return current;
    }
    return NULL;
    pthread_rwlock_unlock(&q->queue_lock);
}

int main(int argc, char* argv[]) { return 0; }
