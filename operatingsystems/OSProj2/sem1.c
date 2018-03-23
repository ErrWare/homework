//when compiling use gcc -pthread -o outputexename sem1.c
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define NUM_THREADS 10

sem_t semaphore;

void *sem_demo (void *arg)
{
    int *pnum = (int *) arg;
    int num = *pnum;
    free(arg);

    printf("Thread %d waiting\n", num);
    if (sem_wait (&semaphore) == -1)
    {
        printf("Wait on semaphore\n");
        exit(1);
    }

    printf("Thread %d resuming\n", num);
    return NULL;
}

//clarification: I am copying code from an example given in class
int main (int argc, char *argv[])
{
    int thread_count;
    pthread_t sem_waiters[NUM_THREADS];
    int status;

    if (sem_init (&semaphore, 0, 0) == -1)
    {
        printf("Init semaphore\n");
        exit(1);
    }

    for (thread_count = 0; thread_count < NUM_THREADS; thread_count++)
    {
        int *pnum = (int*) malloc(sizeof(int));
        *pnum = thread_count;
        status = pthread_create (
                &sem_waiters[thread_count], NULL,
                sem_demo, (void*)pnum);
        if (status != 0)
        {
            printf("Create thread\n");
            exit(1);
        }
    }

    sleep(2);

    for (thread_count = 0; thread_count < NUM_THREADS; thread_count++)
    {
        printf("Posting from main\n");
        if (sem_post (&semaphore) == -1)
        {
            printf("Post semaphore\n");
            exit(1);
        }

    }

    for (thread_count = 0; thread_count < NUM_THREADS; thread_count++)
    {
        status = pthread_join (sem_waiters[thread_count], NULL);
        if (status != 0)
        {
            printf("Join thread\n");
            exit(1);
        }
    }
    return 0;
}

