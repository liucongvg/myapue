#include <pthread.h>
#include <stdlib.h>

#define HASH_NUMBER 29
#define HASH(id) (((unsigned long)id) % HASH_NUMBER)

struct foo {
    int f_reference;
    pthread_mutex_t f_lock;
    int f_id;
    struct foo* f_next;
    /*more member here*/
};

struct foo* foo_hash[HASH_NUMBER];
pthread_mutex_t foo_hash_lock = PTHREAD_MUTEX_INITIALIZER;

struct foo* foo_alloc(int id)
{
    int index = HASH(id);
    struct foo* new_foo = malloc(sizeof(struct foo));
    if (!new_foo)
        return new_foo;
    if (pthread_mutex_init(&new_foo->f_lock, NULL) != 0) {
        free(new_foo);
        new_foo = NULL;
        return new_foo;
    }
    new_foo->f_id = id;
    new_foo->f_reference = 1;
    pthread_mutex_lock(&foo_hash_lock);
    new_foo->f_next = foo_hash[index];
    foo_hash[index] = new_foo;
    pthread_mutex_unlock(&foo_hash_lock);
    pthread_mutex_lock(&new_foo->f_lock);
    // continue initialize other member
    pthread_mutex_unlock(&new_foo->f_lock);
    return new_foo;
}

struct foo* foo_find_and_hold(int id)
{
    int index = HASH(id);
    struct foo* current_foo;
    pthread_mutex_lock(&foo_hash_lock);
    for (current_foo = foo_hash[index]; current_foo; current_foo = current_foo->f_next) {
        if (current_foo->f_id == id) {
            ++(current_foo->f_reference);
            break;
        }
    }
    pthread_mutex_unlock(&foo_hash_lock);
    return current_foo;
}

void foo_release(struct foo* fp)
{
    pthread_mutex_lock(&foo_hash_lock);
    if (--(fp->f_reference) == 0) {
        int id = fp->f_id;
        int index = HASH(id);
        struct foo* current_foo = foo_hash[index];
        if (current_foo == fp) {
            foo_hash[index] = current_foo->f_next;
        } else {
            while (current_foo->f_next != fp)
                current_foo = current_foo->f_next;
            current_foo->f_next = fp->f_next;
        }
        pthread_mutex_unlock(&foo_hash_lock);
        pthread_mutex_destroy(&fp->f_lock);
        free(fp);
    } else {
        pthread_mutex_unlock(&foo_hash_lock);
    }
}

int main(int argc, char* argv[]) { return 0; }
