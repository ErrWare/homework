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
sem_t receptionist_assigned;
sem_t patient_accept;
sem_t drs[3];
sem_t entry[3];
sem_t ready2gab[3];
sem_t advice_given[3];
sem_t assignedQ[3];

int doctors;
int* patient_to_dr;
int pidQs[3][30];
int pidQfront[] = {0, 0, 0};
int pidQback[] = {-1, -1, -1};
int waitingQ[30];
int waitingQfront = 0;
int waitingQback = -1;

int main(int argc, char *argv[])
{
    //std::cout << "STARTING POGRAM" << std::endl;

    if( argc != 3 )
    {
        std::cout << "Pogram requires precisely 3 arguments (including executable name)" << std::endl;
    }
    //initialize data and data structure 
    doctors = atoi(argv[1]);
    int patients = atoi(argv[2]);
    patient_to_dr = new int[patients];
    pthread_t pids[patients];
    
    //initialize semaphores
    if (sem_init (&waiting_room_door, 0, 1) == -1) exit(1);
    if (sem_init (&waiting_room, 0, 0) == -1) exit(1);
    if (sem_init (&receptionist_assigned, 0, 0) == -1) exit(1);
    if (sem_init (&patient_accept, 0, 0) == -1) exit(1);
    //create an array of literals because the pthread create pass by reference
    //is liable to have the val at payload address change before it is used
    int literals[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29};
    for(int i = 0; i<doctors; i++){
        if (sem_init (&(drs[i]), 0, 1) == -1) exit(11);
        if (sem_init (&(assignedQ[i]), 0, 0) == -1) exit(12);
        if (sem_init (&(entry[i]), 0, 0) == -1) exit(13);
        if (sem_init (&(ready2gab[i]), 0, 0) == -1) exit(14);
        if (sem_init (&(advice_given[i]), 0, 0) == -1) exit(15);
        //spawn nurse and dr threads
        pthread_t tid1;
        pthread_t tid2;
        pthread_create(&tid1, NULL, dr, literals+i);
        pthread_create(&tid2, NULL, nurse, literals+i);
    }

    //spawn receptionist thread
    pthread_t xyz;
    pthread_create(&xyz, NULL, receptionist, literals);

    //spawn patient threads
    for(int i = 0; i < patients; i++)
        pthread_create(&(pids[i]), NULL, patient, literals+i);
    
    //join patient threads then terminate program
    for(int i = 0; i < patients; i++)
        pthread_join(pids[i], NULL);

    printf("ALL PATIENTS GONE~~~ PARTY TIME\n");

}

void* patient(void* input){
    int id = *((int*)input);

    if (sem_wait(&waiting_room_door) == -1) exit(20);
    waitingQ[waitingQfront++] = id;
    printf("Patient %d enters the waiting room, waits for receptionist\n",id);
    sleep(1);
    if (sem_post(&waiting_room) == -1) exit(21);
    if (sem_post(&waiting_room_door) == -1) exit(22);

    if (sem_wait(&receptionist_assigned) == -1) exit(23);
    printf("Patient %d leaves receptionist and sits in waiting room\n", id);
    sleep(1);
    if (sem_post(&patient_accept) == -1) exit(24);

    if (sem_wait(&(entry[patient_to_dr[id]])) == -1) exit(24);
    printf("Patient %d enters doctor %d's office\n", id, patient_to_dr[id]);
    sleep(1);
    if (sem_post(&(ready2gab[patient_to_dr[id]])) == -1) exit(25);

    if (sem_wait(&(advice_given[patient_to_dr[id]])) == -1) exit(26);
    printf("Patient %d receives advice from doctor %d\n", id, patient_to_dr[id]);
    sleep(1);
    if (sem_post(&(drs[patient_to_dr[id]])) == -1) exit(26);
    printf("Patient %d leaves\n", id);
    sleep(1);

    return NULL;
}
void* receptionist(void* input){
    std:srand(std::time(NULL));
    int pid;
    int r;

    while(true){
        if (sem_wait(&waiting_room) == -1) exit(30);
        pid = waitingQ[++waitingQback];                 //get next patient ID
        r = std::rand() % doctors;                      //select random dr
        pidQs[r][++pidQback[r]] = pid;                  //assign dr to patient
        patient_to_dr[pid] = r;                         //and vice versa
        printf("Receptionist registered patient %d\n", pid);
        sleep(1);
        if (sem_post(&receptionist_assigned) == -1) exit(32);
        if (sem_wait(&patient_accept) == -1) exit(33);
        if (sem_post(&(assignedQ[r])) == -1) exit(31);  //and signal nurse
    }
    return NULL;
}
void* nurse(void* input){
    int id = *((int*)input);
   
    while(true){
        if (sem_wait(&(drs[id])) == -1) exit(40);
        if (sem_wait(&(assignedQ[id])) == -1) exit(41);
        printf("Nurse %d takes patient %d to doctor's office\n", id, pidQs[id][pidQfront[id]]);
        sleep(1);
        if (sem_post(&(entry[id])) == -1) exit(42);
    }
    return NULL;
}
void* dr(void* input){
    int id = *((int*)input);
  
    while(true){
        if (sem_wait(&(ready2gab[id])) == -1) exit(51);
        printf("Doctor %d listens to symptoms from patient %d\n", id, pidQs[id][pidQfront[id]++]);
        sleep(1);
        if (sem_post(&(advice_given[id])) == -1) exit(52);
    }
    return NULL;
}
