//		commands.c
//********************************************
#include "commands.h"
#include "job.h"
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(pJob* jobs, char* lineSize, char* cmdString)
{
	char* cmd; 
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	char* delimiters = " \t\n";  
	int i = 0, num_arg = 0;
	bool illegal_cmd = FALSE; // illegal command
    cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0; 
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters); 
		if (args[i] != NULL) 
			num_arg++;
	}
	if (history_idx < MAX_COMMANDS){
        strcpy(history[history_idx], cmdString);
        history_idx++;
	}
	else{
	    for (int i =0 ; i < MAX_COMMANDS-1 ; i++){
            strcpy(history[i], history[i+1]);
	    }
        strcpy(history[history_idx], cmdString);
	}

/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
	if (!strcmp(cmd, "cd") ) 
	{
		int result = 0;
		char temp1[MAX_LINE_SIZE] = "\0";
		char* temp2 = NULL;
		if (num_arg != 1){
			illegal_cmd = TRUE;
		}
		//num of args is valid
		else{
			temp2 = getcwd(temp1, MAX_LINE_SIZE);
			if (temp2 == NULL){
				perror("");
			}
			//getcwd succeeded
			else{
				if (!strcmp(args[1], "-")){
					if (strcmp(prev_command, "\0") != 0){
						result = chdir(prev_command);
					}
				}
				else{
					result = chdir(args[1]);
				}
				//chdir failed
				if (result == -1){
					printf("smash error: > \"%s\" - path not found\n", args[1]);
				}
				else{
					strcpy(prev_command, temp2);
				}
			}
		}
	} 
	
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
        if (num_arg != 0){
            illegal_cmd = TRUE;
        }
        else{
            char temp1[MAX_LINE_SIZE] = "\0";
            char* temp2 = NULL;

			temp2 = getcwd(temp1, MAX_LINE_SIZE);
			if (temp2 == NULL){
				perror("");
			}
			else{
				printf("%s\n", temp2);
			}
        }
	}
	/*************************************************/
	else if (!strcmp(cmd, "history"))
    {
        if (num_arg != 0){
            illegal_cmd = TRUE;
        }
        else{
            if (history_idx<MAX_COMMANDS){
                for (int j = 0; j < history_idx - 1; j++){
                    printf("%s\n", history[j]);
                }
            }
            else{
                for (int j = 0; j < MAX_COMMANDS - 2; j++){
                    printf("%s\n", history[j]);
                }
            }
        }
    }

	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
		if (num_arg != 0){
			illegal_cmd = TRUE;
		}
		else{
			for (int t = 0; t < job_index; t++){
				printJob(jobs[t]);
			}
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
        if (num_arg != 0){
            illegal_cmd = TRUE;
        }
        else{
            printf("smash pid is %d\n", getpid());
        }
	}
	/*************************************************/
	else if (!strcmp(cmd, "kill"))
	{
		if (num_arg != 2){
			illegal_cmd = TRUE;
		}

		else{
			char* signal = malloc(sizeof(args[1])); //no need for +1 because cutting short in next command
			strcpy(signal,args[1]+1); //need to make sure no /n for atoi and that starts without "-"
			printf("OMER CHECK got %s",signal); //omer check

			for (int i = 0; i < job_index; i++){
				int PID = returnPID(jobs[i]);
				if (PID==args[2]){
					int res = kill(args[2], atoi(signal);
					printf("signal %s was sent to pid %d",signal,PID);
					if (!res){
						printf("smash error: > kill \"%s\" - cannot send signal\n", args[2]);
					}
					break;
				}

			}
			if (i==job_index) printf("smash error: > kill \"%s\" - job does not exist\n",args[2]); //did not find job
			free(signal);
		}

	}

	/*************************************************/
	else if (!strcmp(cmd, "mv"))
	{
		if (num_arg != 2){
			illegal_cmd = TRUE;
		}
		else{
			int result = rename(args[1], args[2]);
			if (result == 0){
				printf("%s has been renamed to %s\n", args[1], args[2]);
			}
			if (result == -1){
				perror("");
			}
		}
	}

	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{
		
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
  		
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
   		if (num_arg>1)
   			illegal_cmd=TRUE;
   		else{
   			if (num_arg==1 && !strcmp(args[1],"kill")) { //first kill all processes
				for (int i=0;i<job_index;i++){
					int PID =  returnPID(jobs[i]);
					kill(PID,SIGTERM);
					printf("signal SIGTERM was sent to pid %d",PID);
					for (int i=0;i<5;i++){
						if (waitpid(PID,NULL,WNOHANG)!=0) //terminated!
							break;
						else sleep(1);
					}
					if (i==5){ //pid not terminated after 5 seconds
						kill(PID,SIGKILL) //force sigkill
					}

				}
   			}
   			//done killing children if requested. now:
			printf("signal SIGTERM was sent to pid %d",getpid());
   			kill(getpid(),SIGKILL) //kill current process;
   		}
	} 
	/*************************************************/
	else // external command
	{
 		ExeExternal(args, cmdString);
	 	return 0;
	}
	if (illegal_cmd == TRUE)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], char* cmdString)
{
	int pid;
	pid = fork();
	//son has created, now we will execute the command in the background
	if (pid == 0) {
		setpgrp(); //change the group-ID
		execv(args[0], args);
		perror("");
		exit(-1);
	}
	//parent- the shell. we will wait until the program is executed
	if (pid > 0) {
		wait(NULL);
		perror("");
		return;
	}
	//fork failed
	else {
		perror("");
		exit(-1);
	}
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
int ExeComp(char* lineSize)
{
	char ExtCmd[MAX_LINE_SIZE+2];
	char *args[MAX_ARG];
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*"))
		|| (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
		return 0;
	} 
	return -1;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, pJob* jobs) {
	int pid;
	char *Command;
	char *delimiters = " \t\n";
	char *args[MAX_ARG];
	if (lineSize[strlen(lineSize) - 2] == '&') {
		lineSize[strlen(lineSize) - 2] = '\0';
		int i = 0, num_arg = 0;
		bool illegal_cmd = FALSE; // illegal command
		Command = strtok(lineSize, delimiters);
		if (Command == NULL)
			return 0;
		args[0] = Command;
		for (i = 1; i < MAX_ARG; i++) {
			args[i] = strtok(NULL, delimiters);
			if (args[i] != NULL)
				num_arg++;
		}
		pid = fork();
		//son has created, now we will execute the command in the background
		if (pid == 0) {
			setpgrp(); //change the group-ID
			execv(args[0], args);
			perror("");
			exit(-1);
		}
		//parent- the shell. we will insert the son to the jobs array
		if (pid > 0) {
			time_t cur_time = time(NULL);
			pJob newJob = NewJob(job_index + 1, pid, cur_time, Command);
			jobs[job_index] = newJob;
			job_index++;
			return 0;
		}
		//fork failed
		else {
			perror("");
			exit(-1);
		}
	}
	return -1;
}


