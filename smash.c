/*	smash.c
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "commands.h"
#include "signals.h"
#include "job.h"

char* L_Fg_Cmd;
pJob jobs[MAX_JOBS]; //This represents the list of jobs. Please change to a preferred type (e.g array of char*)
char lineSize[MAX_LINE_SIZE];
char prev_command[MAX_LINE_SIZE] = "\0";
char history[MAX_COMMANDS][MAX_LINE_SIZE] = {{"\0"}};
int history_idx = 0;
int job_index = 0;

//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
	//printf("job index = %d\n", job_index);
    char cmdString[MAX_LINE_SIZE]; 	   

	
	//signal declaretions
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	 /* add your code here */
	
	/************************************/
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	//set your signal handlers here
	/* add your code here */

	/************************************/


	/************************************/
	// Init globals 

	L_Fg_Cmd =(char*)malloc(sizeof(char)*(MAX_LINE_SIZE+1));
	if (L_Fg_Cmd == NULL) 
			exit (-1); 
	L_Fg_Cmd[0] = '\0';
	
    	while (1)
    	{

        pid_t childPid = waitpid(-1, NULL, WNOHANG);
        if (childPid != 0 && childPid!= -1){
            for (int i = 0; i < job_index; i++){
                if (childPid == getPID(jobs[i])){
                    deleteJob(jobs[i]);
                    for (int j = i ; j < job_index - 1; j++){
                        setIdx(jobs[j+1], j+1);
                        jobs[j] = jobs[j+1];
                    }
                    job_index--;
                }
            }
        }

	 	printf("smash > ");
		fgets(lineSize, MAX_LINE_SIZE, stdin);
		strcpy(cmdString, lineSize);    	
		cmdString[strlen(lineSize)-1]='\0';
					// perform a complicated Command
		if(!ExeComp(lineSize)) continue; 
					// background command	
	 	if(!BgCmd(lineSize, jobs)) continue; 
					// built in commands
		ExeCmd(jobs, lineSize, cmdString);
		
		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
    return 0;
}

