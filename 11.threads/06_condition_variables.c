#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct message_queue {
    struct message_queue* next;
    int count;
} * tasks;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

struct message_queue* process_message(void* thread_num)
{
    pthread_mutex_lock(&mutex);
    while (!tasks)
        pthread_cond_wait(&condition, &mutex);
    printf("thread:%ld processing message...\n", (long)thread_num + 1);
    struct message_queue* current = tasks;
    tasks = current->next;
    pthread_mutex_unlock(&mutex);
    // process current here, suppose this work can be done 3 seconds later
    if (sleep(1) != 0) {
        perror("sleep error");
    }
    printf("thread:%ld processing message done...\n", (long)thread_num + 1);
    return current;
}

void enqueue_message(struct message_queue* message)
{
    pthread_mutex_lock(&mutex);
    message->next = tasks;
    tasks = message;
    pthread_mutex_unlock(&mutex);
    pthread_cond_broadcast(&condition);
}

#define THREAD_NUM 4

void* thread_function(void* arg)
{
    while (1) {
        struct message_queue* done_message = process_message(arg);
        free(done_message);
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t threads[THREAD_NUM];
    long i;
    for (i = 0; i < THREAD_NUM; ++i)
        pthread_create(&threads[i], NULL, thread_function, (void*)i);
    int task_count = 10;
    while (task_count) {
        struct message_queue* entry = (struct message_queue*)malloc(sizeof(struct message_queue));
        entry->count = 0;
        int current_task = task_count--;
        printf("start enqueue message:%d...\n", current_task);
        enqueue_message(entry);
        printf("enqueue message done:%d\n", current_task);
    }
    for (i = 0; i < THREAD_NUM; ++i)
        pthread_join(threads[i], NULL);
    return 0;
}
