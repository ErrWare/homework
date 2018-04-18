//Sim for process-dispatch scheduling
//Simulates FCFS, RR, SPN, SRT, HRRN, FB

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <cstdio>

class Job{
    public:
    static int total_jobs;
    int id;
    int arrival_time;
    int total_time;
    int serviced_time;
    Job(){}
    Job(int arrival, int total){
        arrival_time = arrival;
        total_time = total;
        serviced_time = 0;
        id = total_jobs++;
    }
    int remaining_time(){return total_time - serviced_time;}

    void print(){
        std::cout << id << " : " << arrival_time << "\t" << total_time << std::endl;
    }

};
int Job::total_jobs = 0;
int total_work_time = 0;
int* schedule;
Job jobs[26];

void printschedule(){
    for(int i = 0; i < Job::total_jobs; i++)
        std::cout << (char)(65+i) << "\t";
    std::cout << std::endl;
    
    for(int i = 0; i < total_work_time; i++){
        for(int j = 0; j < schedule[i]; j++)
            std::cout << "\t";
        std::cout << "X" << std::endl;
    }
}


int main(int argc, char *argv[]){
    FILE* f = fopen("jobs.txt", "r");
    int a,b;
    char s[6];
    while(!std::feof(f)){
        fscanf(f,"%s%d%d",s,&a,&b);
        jobs[Job::total_jobs] = Job(a,b);

    }
    std::fclose(f);
    Job::total_jobs -= 1;   //because I'm bad at reading from files and get an extra line in the while loop

    for(int i = 0; i < Job::total_jobs; i++){
        jobs[i].print();
        total_work_time += jobs[i].total_time;
    }
    std::cout << "Total time : " << total_work_time << std::endl;
    schedule = new int[total_work_time];

    printschedule();
    //ifs.close();

}
