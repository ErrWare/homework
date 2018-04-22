//Sim for process-dispatch scheduling
//Simulates FCFS, RR, SPN, SRT, HRRN, FB

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <queue>

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
    double approx_RR(int t){return ((double)(t - arrival_time + total_time))/total_time;}
    void print(){
        std::cout << id << " : " << arrival_time << "\t" << total_time << std::endl;
    }

};
int Job::total_jobs = 0;
int total_work_time = 0;
int* schedule;
Job jobs[27];   //might accidentally get if there are really 26

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

void serviceJob(int &t, Job &j){
    schedule[t] = j.id;
    j.serviced_time++;
    t++;
}
void resetJobs(){
    for(int i = 0; i < Job::total_jobs; i++)
        jobs[i].serviced_time = 0;
}

void FCFS(){
    int time = 0;
    for(int i = 0; i < Job::total_jobs; i++){    //relies on assumption jobs listed in chronological order in jobs.txt
        while(jobs[i].remaining_time() > 0) {
            serviceJob(time, jobs[i]);
        }
    }
}
void RR(){
    int time = 0;
    int min_job_not_in_queue = 1;
    std::queue<Job> jq;
    jq.push(jobs[0]);
    Job* j;
    while(time < total_work_time){
        //pop job from RR queue and service
        *j = jq.front();
        jq.pop();
        serviceJob(time, *j);
        //enqueue any newly arrived jobs
        for(int i = min_job_not_in_queue; i < Job::total_jobs; i++){
            if(jobs[i].arrival_time <= time){
                jq.push(jobs[i]);
                min_job_not_in_queue++;
            }
        }
        //push serviced job back into queue if needs more time
        if(j->remaining_time() > 0)
            jq.push(*j);
    }
}
void SRT(){
    int time = 0;
    while(time < total_work_time){
        //Choose next job by SRT metric
        int jobid = 0;
        int min = total_work_time + 1;
        for(int i = 0; i < Job::total_jobs; i++){
            if(jobs[i].arrival_time <= time && jobs[i].remaining_time() < min && jobs[i].remaining_time() > 0){
                min = jobs[i].remaining_time();
                jobid = i;
            }
        }
        //Service job one time step
        serviceJob(time, jobs[jobid]);
    }
}
void SPN(){
    int time = 0;
    while(time < total_work_time){
        //Choose next job by SPN metric
        int jobid = 0;
        int min = 9999999;
        for(int i = 0; i < Job::total_jobs; i++){
            if(jobs[i].arrival_time <= time && jobs[i].remaining_time() < min && jobs[i].remaining_time() > 0){
                min = jobs[i].remaining_time();
                jobid = i;
            }
        }
        //Service job to completion
        for(int i = 0; i < jobs[jobid].total_time; i++){
            serviceJob(time, jobs[jobid]);
        }
    }
}
void HRRN(){
    int time = 0;
    while(time < total_work_time){
        //Choose next job by HRRN metric
        int jobid = 0;
        double max = -1;
        for(int i = 0; i < Job::total_jobs; i++){
            if(jobs[i].arrival_time <= time && jobs[i].approx_RR(time) > max && jobs[i].remaining_time() > 0){
                jobid = i;
                max = jobs[i].approx_RR(time);
            }
        }

        //Service job to completion
        for(int i = 0; i < jobs[jobid].total_time; i++){
            serviceJob(time, jobs[jobid]);
        }
    }
}
void FB(){
    Job q1[27];
    int q1_f = 0, q1_b = 0;
    Job q2[27];
    int q2_f = 0, q2_b = 0;
    Job q3[27];
    int q3_f = 0, q3_b = 0;
    Job * j;
    int time = 0;
    while(time < total_work_time){
        for(int i = 0; i < Job::total_jobs; i++){
            if(jobs[i].arrival_time == time){
                q1[q1_f++] = jobs[i];
                q1_f %= 27;
            }
        }

        int q_used = 0;
        if(q1_f != q1_b){
            q_used = 1;
            *j = q1[q1_b++];
            q1_b %= 27;
        }
        else if(q2_f != q2_b){
            q_used = 2;
            *j = q2[q2_b++];
            q2_b %= 27;
        }
        else if(q3_f != q3_b){
            q_used = 3;
            *j = q3[q3_b++];
            q3_b %= 27;
        }
        else{
            std::cout << "ERROR IN FB: NO Q POPPED FROM" << std::endl;
        }

        serviceJob(time, *j);

        if(j->remaining_time() > 0){
            if(q_used == 1){
                q2[q2_f++] = *j;
                q2_f %= 27;
            }
            else{
                q3[q3_f++] = *j;
                q3_f %= 27;
            }
        }
    }
}
//ASSUMPTION: no mandatory down time
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
    
    std::cout << "FCFS" << std::endl;
    FCFS();
    printschedule();
    resetJobs();
    std::cout << "RR" << std::endl;
    RR();
    printschedule();
    resetJobs();
    std::cout << "SRT" << std::endl;
    SRT();
    printschedule();
    resetJobs();
    std::cout << "SPN" << std::endl;
    SPN();
    printschedule();
    resetJobs();
    std::cout << "HRRN" << std::endl;
    HRRN();
    printschedule();
    resetJobs();
    std::cout << "FB" << std::endl;
    FB();
    std::cout << "FB DONE:" <<std::endl;
    printschedule();
    std::cout << "ALL DONE" <<std::endl;
    //ifs.close();

}
