// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"

void ctrlC_handler(){
    kill(FG_PID,SIGINT)
}

struct sigaction actC;
actC.sa_handler =&crtlC_handler;
sigfillset(actC.sa_mask);
sigaction(SIGINT,&actC,NULL);

void ctrlZ_handler(){

}


struc sigaction actZ;
actZ.sa_handler =&ctrlZ_handler;
sigfillset(actZ.sa_mask);
sigaction(SIGTSTP,&actZ,NULL);

