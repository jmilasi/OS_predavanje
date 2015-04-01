#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
int sig[]={SIGQUIT, SIGFPE,SIGALRM, SIGTERM, SIGINT};
int pid=0;

void prekidna_rutina(int si){
   /* pošalji SIGKILL procesu 'pid'*/
   kill(pid, SIGKILL);
   exit(0);
}

int main(int argc, char *argv[]){
	if(argc!=2)
	{
		printf("Niste unijeli PID preko komande linije\n");
		exit(0);
	}
	pid=atoi(argv[1]);	
	sigset(SIGINT, prekidna_rutina);
	int v;
	while(1){
		/* odspavaj 3-5 sekundi */
		sleep(3);
		v=rand()%4;
		kill(pid, sig[v]);
		/* pošalji odabrani signal procesu 'pid' funkcijom kill*/
	}
   return 0;
}
