//this is the project file for OS Project 2
//Goal: demonstrate an understanding of thread synchronization using semaphores
//
#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>

void* print_thread(void*);

int main(int argc, char *argv[])
{
    std::cout << "STARTING POGRAM" << std::endl;

    if( argc != 3 )
    {
        std::cout << "Pogram requires precisely 3 arguments (including executable name)" << std::endl;
    }
    
    int doctors = atoi(argv[1]);
    int patients = atoi(argv[2]);

    std::cout << "Drs: " << doctors << "\tpatients: " << patients << "\tDrs * patients: " << doctors * patients << std::endl;

    pthread_t tid;
    int i = 7;
    pthread_create(&tid, NULL, print_thread, &i);

    pthread_join(tid, NULL);
}

void* print_thread(void* input){
    printf("%d\n", 5 * *((int*)input));
    return NULL;
}
