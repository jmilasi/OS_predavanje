/*
Zadatak
Računati proste brojeve te preko signala periodički ispisivati status. Koristiti setitimer sučelje za periodički alarm
(vidjeti Primjer periodičke obrade pri dnu stranice). Na signal SIGINT privremeno zaustaviti rad na idućim brojevima
(programski ostvariti zaustavljanje), odnosno, nastaviti s radom ako je program prethodno bio zaustavljen. Na signal
SIGTERM ispisati zadnji broj koji se provjerava (ili će biti idući) i završiti s radom.
*/
/*
SIGINT        Šalje se svakom procesu za koji je to kontrolni terminal u trenutku kada je pritisnuta tipka za prekid (standardno DEL, najčešće izmijenjeno u Ctrl-C).

SIGQUIT       Slično kao SIGINT, ali se odnosi na tipku za kraj izvršavanja (standardno Ctrl-\, najčešće izmijenjeno u Ctrl-X).

SIGKILL       Jedini siguran način uništavanja procesa je da mu se pošalje ovaj signal, jer ne može biti ignoriran niti uhvaćen.

SIGALRM       Šalje se kada istekne traženo vrijeme čekanja procesa.

SIGTERM       Ovo je standardni signal za uništavanje procesa. Koristi se i kod rušenja operacijskog sustava da ubije sve aktivne procese. Očekuje se da će proces koji ga primi spremiti svoje aktualno stanje i zatvoriti otvorene datoteke prije završetka.

SIGFPE        Signal koji se događa zbog aritmetičke pogreške (dijeljenje s nulom, floating point overflow ili underflow)

SIGUSR1 i SIGUSR2 signali slobodni u korisničke svrhe
*/
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include<stdlib.h> //za funkciju exit()

int pauza = 0;
int broj = 1000000001;
int zadnji = 1000000001;
/*globalne varijable koje se smiju i promijeniti*/
int prost(unsigned long n)
{ 
	unsigned long i, max;	
	if((n%2)==0) 
		return 0;
	max=sqrt(n);
	for(i=3;i<max;i+=2)
		if((n%i)==0) return 0;
	return 1;
}
/* funkcija koja provjerava jel broj prost i u tom slucaju vraca 1 */
void prekini(int sig)
{
	printf("Zadnji broj koji se provjerava: %d\n", broj );
	exit(0);
}
void postavi_pauzu(int sig)
{
	pauza=1-pauza;
}
void periodicki_ispis(int sig)
{
	printf("Zadnji prosti broj: %d\n",zadnji);
}

int main()
{	printf("SIGINT-->postavi_pauzu()\n SIGTERM->prekini()\n");
	sigset(SIGINT,postavi_pauzu);
	sigset(SIGTERM, prekini);
	/* povezivanje obrade signala SIGALRM sa funkcijom "prekini" */
	
	struct itimerval t;
	sigset (SIGALRM, periodicki_ispis);
	/* definiranje periodičkog slanja signala */
	/* prvi puta nakon: */
	t.it_value.tv_sec = 5;
	t.it_value.tv_usec =0;
	/* nakon prvog puta, periodicki sa periodom: */
	t.it_interval.tv_sec = 5;
	t.it_interval.tv_usec = 0;
	/* pokretanje sata s pridruženim slanjem signala prema "t" */
	setitimer (ITIMER_REAL, &t, NULL );
	while(1)
	{
		if(prost(broj)==1)
			zadnji=broj;
		broj++;
		while(pauza==1)
			pause();
	}	
	return 0;
}
