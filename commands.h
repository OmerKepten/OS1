#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "job.h"
#define MAX_LINE_SIZE 80
#define MAX_ARG 20
#define MAX_COMMANDS 50
#define MAX_COMMAND_LENGTH 50
#define MAX_JOBS 100

typedef enum { FALSE , TRUE } bool;
int ExeComp(char* lineSize);
int BgCmd(char* lineSize, pJob* jobs);
int ExeCmd(pJob* jobs, char* lineSize, char* cmdString);
void ExeExternal(char *args[MAX_ARG], char* cmdString);
extern char prev_command[MAX_LINE_SIZE];
extern char history[MAX_COMMANDS][MAX_LINE_SIZE];
extern int history_idx;
extern int job_index;
extern pJob jobs[MAX_JOBS];
#endif

