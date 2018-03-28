//this is the project file for OS Project 2
//Goal: demonstrate an understanding of thread synchronization using semaphores
//
//when compiling use g++ -pthread -o outputexename proj2.cpp
#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>

void* print_thread(void*);
void* patient(void*);
void* nurse(void*);
void* receptionist(void*);
void* dr(void*);

sem_t waiting_room_door;
sem_t waiting_room;
sem_t receptionist_admit;
sem_t receptionist_assigned;
sem_t patient_accept;
sem_t nurses[3];
sem_t drs[3];
sem_t entry[3];
sem_t ready2gab[3];
sem_t advice_given[3];
sem_t assignedQ[3];

int* patient_to_dr;

int main(int argc, char *argv[])
{
    std::cout << "STARTING POGRAM" << std::endl;

    if( argc != 3 )
    {
        std::cout << "Pogram requires precisely 3 arguments (including executable name)" << std::endl;
    }
    
    int doctors = atoi(argv[1]);
    int patients = atoi(argv[2]);
    patient_to_dr = new int[patients];


    if (sem_init (&waiting_room_door, 0, 1) == -1) exit(1);
    if (sem_init (&waiting_room, 0, 0) == -1) exit(1);
    if (sem_init (&receptionist_assigned, 0, 0) == -1) exit(1);
    if (sem_init (&patient_accept, 0, 0) == -1) exit(1);
    //create an array of literals because the pthread create pass by reference
    //is liable to have the val at payload address change before it is used
    int literals[] = {0,1,2};
    for(int i = 0; i<doctors; i++){
        if (sem_init (&(nurses[i]), 0, 0) == -1) exit(1);
        if (sem_init (&(drs[i]), 0, 0) == -1) exit(1);
        if (sem_init (&(assignedQ[i]), 0, 0) == -1) exit(1);
        if (sem_init (&(entry[i]), 0, 0) == -1) exit(1);
        if (sem_init (&(ready2gab[i]), 0, 0) == -1) exit(1);
        if (sem_init (&(advice_given[i]), 0, 0) == -1) exit(1);
        pthread_t tid1;
        pthread_t tid2;
        pthread_create(&tid1, NULL, dr, literals+i);
        pthread_create(&tid2, NULL, nurse, literals+i);
    }



//    std::cout << "Drs: " << doctors << "\tpatients: " << patients << "\tDrs * patients: " << doctors * patients << std::endl;

    pthread_t tid;
    int i = 7;
    pthread_create(&tid, NULL, print_thread, &i);

    pthread_join(tid, NULL);

    sleep(patients);

}

void* print_thread(void* input){
  //  printf("%d\n", 5 * *((int*)input));
    return NULL;
}

void* patient(void* input){

    return NULL;
}
void* receptionist(void* input){

    return NULL;
}
void* nurse(void* input){
    printf("nurse %d create\n", *((int*)input));
    return NULL;
}
void* dr(void* input){
    printf("dr %d created\n", *((int*)input));
    return NULL;
}

class Queue {
    class Node{
        Node* next;
        int i;
        public:
        Node(int j){
            i = j;
            next = NULL;
        }
        void setNext(Node* n){
            next = n;
        }
        Node* getNext(){
            return next;
        }
        int getNum(){
            return i;
        }
        Node* insert(int j){
            next = new Node(j);
            return next;
        }
    }
    Node* head = NULL;
    Node* tail = NULL;

    public:
        bool isEmpty(){
            return head == NULL;
        }
        int peek(){
            return (*head).getNum();
        }
        int poll(){
            int i = peek();
            Node* n = (*head).getNext();
            delete(head);
            head = n;
        }
        void insert(int i){
            if(isEmpty()){
                head = new Node(i);
                tail = head;
            } else {
                tail = (*tail).insert(i);
            }
        }

}
