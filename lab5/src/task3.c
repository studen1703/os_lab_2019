#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mut1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut2 = PTHREAD_MUTEX_INITIALIZER;

void* fun1()
{
    printf("The first function is working!\n");
    pthread_mutex_lock(&mut1);
    printf("First function locked mut1\n");
    sleep(10);
    pthread_mutex_lock(&mut2);
    printf("The first function locked mut2\n");

    pthread_mutex_unlock(&mut1);
    printf("The first function unlocked mut1\n");
    pthread_mutex_unlock(&mut2);
    printf("The first function unlocked mut2\n");
    return NULL;
}
void* fun2()
{
    printf("The second function is working!\n");
    pthread_mutex_lock(&mut2);
    printf("The second function locked mut2\n");
    sleep(10);
    pthread_mutex_lock(&mut1);
    printf("The second function locked mut1\n");

    pthread_mutex_unlock(&mut2);
    printf("The second function unlocked mut2\n");
    pthread_mutex_unlock(&mut1);
    printf("The second function unlocked mut1\n");
    return NULL;
}

int main()
{
    pthread_t t1;
    pthread_t t2;
    
    if (pthread_create(&t1, NULL, fun1, NULL)) {
        perror("pthread_create");
        exit(1);
    }
    if (pthread_create(&t2, NULL, fun2, NULL)) {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_join(t1, NULL)) {
        perror("pthread_join");
        exit(1);
    }
    if (pthread_join(t2, NULL)) {
        perror("pthread_join");
        exit(1);
    }
    return 0;
}