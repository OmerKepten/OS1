//
// Created by Lotem114 on 11/20/2018.
//

#include <stdio.h>
#include <time.h>
#include "job.h"


typedef struct Job{
    int number;
    int pID;
    time_t start_time;
    char process_name[MAX_COMMAND_LENGTH];
}*pJob;

pJob NewJob(int number, int pID, time_t start, char* name){
    pJob j = malloc(sizeof(*j));
    if (j == NULL){
        exit(-1);
    }
    j->number = number;
    j->pID = pID;
    j->start_time = start;
    strcpy(j->process_name, name);
    return j;
}

int getPID (pJob j){
    return j->pID;
}

void setIdx(pJob j, int new_idx){
    j->number = new_idx;
}

void deleteJob(pJob j){
    free(j);
}

void printJob (pJob j){
    printf("[%d] %s : %d %d secs\n", j->number, j->process_name, j->pID, (int)difftime(time(NULL), j->start_time));
}

