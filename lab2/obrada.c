#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#define N 6 /*broj razina prekida*/

int OZNAKA_CEKANJA[N];
int PRIORITET[N];
int TEKUCI_PRIORITET=0;

int sig[]={SIGQUIT, SIGFPE,SIGALRM, SIGTERM, SIGINT};

void zabrani_prekidanje()
{
	int i;
	for(i=0; i<5;i++)
		sighold(sig[i]);
}
void dozvoli_prekidanje()
{
	int i;
	for(i=0;i<5;i++)
		sigrelse(sig[i]);
}
void obrada_prekida(int i){
	int t=0;
	for(int j=0;j<6;++j)
		{
			if(j==i) printf("P\t");
			else
				printf("-\t");
		}
		printf("\n");
	while(t<5)
	{
		t++;
		for(int j=0;j<6;++j)
		{
			if(j==i) printf("%d\t",t);
			else
				printf("-\t");
		}
		printf("\n");
		sleep(1);
	}
	for(int j=0;j<6;++j)
		{
			if(j==i) printf("K\t");
			else
				printf("-\t");
		}
		printf("\n");
}
void prekidna_rutina(int sig)  /* pokreće se pojavom prekida uz zabranu daljih prekida */
{ 
	int n=0; 
	zabrani_prekidanje();
	switch(sig){
		case SIGQUIT: 
			n=1; 
			printf("-\tX\t-\t-\t-\t-\n");
			break;
		case SIGFPE: 
			n=2; 
			printf("-\t-\tX\t-\t-\t-\n");
			break;
		case SIGALRM: 
			n=3; 
			printf("-\t-\t-\tX\t-\n");
			break;
		case SIGTERM: 
			n=4; 
			printf(	"-\t-\t-\t-\tX\t-\n");
			break;
		case SIGINT: 
			n=2; 
			printf("-\t-\t-\t-\t-\tX\n");
			break;
		}
	OZNAKA_CEKANJA[n] +=1;
	unsigned int max = 1;
	do{
		/* odredi prekid najveceg prioriteta koji ceka na obradu */
		max = 0;
		for(unsigned int j = TEKUCI_PRIORITET+1;j<=N;++j) 
		{
			if(OZNAKA_CEKANJA[j]!=0)
			{
				max = j;
			}
		}
      /* ako postoji prekid koji ceka i prioritetniji je od trenutnog posla, idi u obradu */
		if(max > 0) 
		{
			OZNAKA_CEKANJA[max] -= 1;
			PRIORITET[max] = TEKUCI_PRIORITET;
			TEKUCI_PRIORITET = max;
			dozvoli_prekidanje();
			obrada_prekida(max);
			zabrani_prekidanje();
			TEKUCI_PRIORITET = PRIORITET[max];
		}

	} while(max > 0);
	dozvoli_prekidanje();
} /* povratkom se automatski dozvoljavaju prekidi (signali) koji su uzrokovali poziv ove f-je */

int main ( void )
{
	sigset (SIGQUIT, prekidna_rutina);
	sigset (SIGFPE, prekidna_rutina);
	sigset (SIGALRM, prekidna_rutina);
	sigset (SIGTERM, prekidna_rutina);
	sigset (SIGINT, prekidna_rutina);

 	printf("Poceo osnovni program PID=%d\n", getpid());
 	printf("G\tS1\tS2\tS3\tS4\tS5\n");
	int i=1;
	printf("P\t-\t-\t-\t-\t-\n");
	while(i<20)
	{
		printf("%d\t-\t-\t-\t-\t-\n",i);
		sleep(1);
		i++;
	}
	printf("Zavrsio osnovni program\n");
	return 0;
}
