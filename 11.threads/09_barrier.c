#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define MAX_COUNT 60

void heap_sort(int array[], int length);
void node_adjust_down(int array[], int index, int length);
void build_heap(int array[], int length);

void heap_sort(int array[], int length)
{
    if (length <= 1)
        return;
    build_heap(array, length);
    int i;
    for (i = length - 1; i > 0; --i) {
        int temp = array[0];
        array[0] = array[i];
        array[i] = temp;
        node_adjust_down(array, 0, i);
    }
}

void build_heap(int array[], int length)
{
    int last_index = length / 2 - 1;
    int i;
    for (i = last_index; i >= 0; --i) {
        node_adjust_down(array, i, length);
    }
}

void node_adjust_down(int array[], int index, int length)
{
    while (1) {
        int l_child = index * 2 + 1, r_child = index * 2 + 2, max_index = index;
        if (l_child < length && array[l_child] > array[max_index])
            max_index = l_child;
        if (r_child < length && array[r_child] > array[max_index])
            max_index = r_child;
        if (max_index == index)
            break;
        int temp = array[index];
        array[index] = array[max_index];
        array[max_index] = temp;
        index = max_index;
    }
}

#define THREAD_COUNT 8
#define NUMBER_COUNT 80000000
#define THREAD_NUMBER_COUNT (NUMBER_COUNT / THREAD_COUNT)
int random_numbers[NUMBER_COUNT];
int sorted_numbers[NUMBER_COUNT];
pthread_barrier_t barrier;

void* thread_function(void* arg)
{
    int thread_index = (int)arg;
    heap_sort(&random_numbers[thread_index * THREAD_NUMBER_COUNT], THREAD_NUMBER_COUNT);
    pthread_barrier_wait(&barrier);
}

void merge()
{
    int i, j;
    int thread_initial_index[THREAD_COUNT];
    for (i = 0; i < THREAD_COUNT; ++i) {
        thread_initial_index[i] = THREAD_NUMBER_COUNT * i;
    }
    for (i = 0; i < NUMBER_COUNT; ++i) {
        int min_thread_index = INT_MAX;
        int min_number = RAND_MAX;
        for (j = 0; j < THREAD_COUNT; ++j) {
            if (random_numbers[thread_initial_index[j]] < min_number
                && thread_initial_index[j] < (j + 1) * THREAD_NUMBER_COUNT) {
                min_thread_index = j;
                min_number = random_numbers[thread_initial_index[j]];
            }
        }
        ++thread_initial_index[min_thread_index];
        sorted_numbers[i] = min_number;
    }
}

int main(int argc, char* argv[])
{
    /*
    int i;
    int array[MAX_COUNT];
    srand(1);
    for (i = 0; i < MAX_COUNT; ++i)
        array[i] = random() % MAX_COUNT;
    heap_sort(array, MAX_COUNT);
    for (i = 0; i < MAX_COUNT; ++i)
        printf("%d,", array[i]);
    printf("\n");
    */
    int i;
    for (i = 0; i < NUMBER_COUNT; ++i) {
        random_numbers[i] = random() % NUMBER_COUNT;
    }
    pthread_t tid;
    pthread_barrier_init(&barrier, NULL, THREAD_COUNT + 1);
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (i = 0; i < THREAD_COUNT; ++i) {
        if (pthread_create(&tid, NULL, thread_function, (void*)i))
            printf("error create thread\n");
    }
    pthread_barrier_wait(&barrier);
    merge();
    gettimeofday(&end, NULL);
    long time_took = end.tv_sec - start.tv_sec;
    /*
    for (i = 0; i < NUMBER_COUNT; ++i)
        printf("%d\n", sorted_numbers[i]);
    */
    printf("sort time took:%ld\n", time_took);
    return 0;
}
