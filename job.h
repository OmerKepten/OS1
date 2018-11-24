//
// Created by Lotem114 on 11/20/2018.
//

#ifndef SMASH_JOB_H
#define SMASH_JOB_H


#include <fcntl.h>
#include <stdlib.h>
#define MAX_COMMAND_LENGTH 50

typedef struct Job *pJob;
pJob NewJob(int number, int pID, time_t start, char* name);
void deleteJob(pJob j);
void printJob(pJob j);
int getPID (pJob j);
void setIdx(pJob j, int new_idx);
#endif //SMASH_JOB_H
