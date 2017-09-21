#include <pthread.h>
#include <stdio.h>

struct test_struct {
    int a;
    int b;
    int c;
};

void* thread1_func(void*);
void* thread2_func(void*);
void print_struct(const struct test_struct*);

int main(int argc, char* argv[])
{
    pthread_t thread1, thread2;
    if (pthread_create(&thread1, NULL, thread1_func, NULL) != 0) {
        return -1;
    }
    struct test_struct* p;
    if (pthread_join(thread1, (void**)&p) != 0) {
        return -1;
    }
    print_struct(p);
    if (pthread_create(&thread2, NULL, thread2_func, NULL) != 0)
        return -1;
    if (pthread_join(thread2, NULL) != 0) {
        return -1;
    }
    print_struct(p);
    return 0;
}

void* thread1_func(void* arg)
{
    struct test_struct test = { 1, 2, 3 };
    print_struct(&test);
    pthread_exit((void*)&test);
}

void* thread2_func(void* arg) { pthread_exit(NULL); }

void print_struct(const struct test_struct* p) { printf("a:%d,b:%d,c:%d\n", p->a, p->b, p->c); }
