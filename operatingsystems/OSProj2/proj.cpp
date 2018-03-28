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
#include <cstdlib>
#include <ctime>

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
sem_t party_time;
int doctors;
int* patient_to_dr;
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
    };
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
            if (isEmpty()) exit(99);
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

};
Queue q0, q1, q2, waitingQ;
Queue qs[] = {q0, q1, q2};


int main(int argc, char *argv[])
{
    std::cout << "STARTING POGRAM" << std::endl;

    if( argc != 3 )
    {
        std::cout << "Pogram requires precisely 3 arguments (including executable name)" << std::endl;
    }
    
    doctors = atoi(argv[1]);
    int patients = atoi(argv[2]);
    patient_to_dr = new int[patients];
    pthread_t pids[patients+1];

    if (sem_init (&waiting_room_door, 0, 1) == -1) exit(1);
    if (sem_init (&waiting_room, 0, 0) == -1) exit(1);
    if (sem_init (&receptionist_assigned, 0, 0) == -1) exit(1);
    if (sem_init (&patient_accept, 0, 0) == -1) exit(1);
    //create an array of literals because the pthread create pass by reference
    //is liable to have the val at payload address change before it is used
    int literals[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29};
    for(int i = 0; i<doctors; i++){
        if (sem_init (&(nurses[i]), 0, 0) == -1) exit(10);
        if (sem_init (&(drs[i]), 0, 0) == -1) exit(11);
        if (sem_init (&(assignedQ[i]), 0, 0) == -1) exit(12);
        if (sem_init (&(entry[i]), 0, 0) == -1) exit(13);
        if (sem_init (&(ready2gab[i]), 0, 0) == -1) exit(14);
        if (sem_init (&(advice_given[i]), 0, 0) == -1) exit(15);
        pthread_t tid1;
        pthread_t tid2;
        pthread_create(&tid1, NULL, dr, literals+i);
        pthread_create(&tid2, NULL, nurse, literals+i);
    }
    printf("Done creating skilled workers\n");
    pthread_t xyz;
    pthread_create(&xyz, NULL, receptionist, literals);

    for(int i = 0; i < patients; i++){
        printf("Spawning patient%d\n", i);
        pthread_create(&(pids[i]), NULL, patient, literals+i);
    }

//    if (sem_init(&party_time, 0, 1-patients) == -1) exit(99);

//    std::cout << "Drs: " << doctors << "\tpatients: " << patients << "\tDrs * patients: " << doctors * patients << std::endl;

   /* for(int i = 0; i < patients; i++)
        pthread_join(pids[i], NULL);
*/
    sleep(13);
    printf("ALL PATIENTS GONE~~~ PARTY TIME");

}

void* patient(void* input){
    int id = *((int*)input);
    if (sem_wait(&waiting_room_door) == -1) exit(20);
    waitingQ.insert(id);
    printf("Patient%d has entered the waiting room\n",id);
    sleep(1);
    if (sem_post(&waiting_room) == -1) exit(21);
    if (sem_post(&waiting_room_door) == -1) exit(22);
        printf("Patient%d left the door open\n",id);
    if (sem_wait(&receptionist_assigned) == -1) exit(23);
    printf("Patient%d leaves receptionist and sits in waiting room\n", id);
    sleep(1);
    
    if (sem_wait(&(entry[patient_to_dr[id]])) == -1) exit(24);
    printf("Patient%d enters Doctor%d's office\n", id, patient_to_dr[id]);
    sleep(1);

    if (sem_post(&(ready2gab[patient_to_dr[id]])) == -1) exit(25);
    if (sem_wait(&(advice_given[patient_to_dr[id]])) == -1) exit(26);
    printf("Patient%d receives advice from doctor%d\n", id, patient_to_dr[id]);
    sleep(1);

    if (sem_post(&(drs[patient_to_dr[id]])) == -1) exit(26);
    printf("Patient%d leaves\n", id);
    sleep(1);
    if (sem_post(&party_time) == -1) exit(27);

    return NULL;
}
void* receptionist(void* input){
    printf("Receptionist initiated\n");
//    std:srand(std::time(NULL));
    int pid;
    int r;
    while(true){
        if (sem_wait(&waiting_room) == -1) exit(30);
        printf("Receptionist attempting poll\n");
        pid = waitingQ.poll();
        printf("Receptionist succeeded poll\n");
        r = 1; // std::rand() % doctors;
        printf("Dr assigned: %d", r);
        qs[r].insert(pid);                              //assign dr to patient
        patient_to_dr[pid] = r;                         //and vice versa
        printf("Receptionist updated dr structures\n");
        if (sem_post(&(assignedQ[r])) == -1) exit(31);    //and signal nurse
        printf("Receptionist registered patient%d\n", pid);
        sleep(1);
        if (sem_post(&receptionist_assigned) == -1) exit(32);
    }
    return NULL;
}
void* nurse(void* input){
    int id = *((int*)input);
   // printf("nurse %d created\n", id);
    while(true){
       // printf("nurse%d waiting for dr\n", id);
        if (sem_wait(&(drs[id])) == -1) exit(40);
       // printf("nurse%d waiting for patientQ\n", id);
        if (sem_wait(&(assignedQ[id])) == -1) exit(41);
        printf("Nurse%d takes patient%d to doctor's office\n", id, qs[id].peek());
        sleep(1);
        if (sem_wait(&(entry[id])) == -1) exit(42);
    }
    return NULL;
}
void* dr(void* input){
    int id = *((int*)input);
   // printf("dr %d created\n", id);
    if (sem_post(&(drs[id])) == -1) exit(50);
    while(true){
    //    printf("Dr%d waiting for gab\n", id);
        if (sem_wait(&(ready2gab[id])) == -1) exit(51);
        printf("Doctor%d listens to symptoms from patient%d\n", id, qs[id].poll());
        sleep(1);

        if (sem_post(&(advice_given[id])) == -1) exit(52);
        
    }
    return NULL;
}
