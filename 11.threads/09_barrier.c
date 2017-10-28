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

int main(int argc, char* argv[])
{
    int i;
    int array[MAX_COUNT];
    srand(1);
    for (i = 0; i < MAX_COUNT; ++i)
        array[i] = random() % MAX_COUNT;
    heap_sort(array, MAX_COUNT);
    for (i = 0; i < MAX_COUNT; ++i)
        printf("%d,", array[i]);
    printf("\n");
    return 0;
}
