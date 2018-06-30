#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <signal.h>

struct timeval t0;

void mySignal( int sigNum, siginfo_t *siginfo, void *ptrToVoid)
{
	printf("Procedura obsługi sygnału SIGUSR1. \n");
	struct timeval t1;
	gettimeofday( &t1, NULL);	
	printf("mySignal: %lf\n",  (t1.tv_sec - t0.tv_sec ) + ( t1.tv_usec - t0.tv_usec) * 1e-6 );
	t0 = t1;
	
}

int main( int argc, char *argv[])
{
	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = mySignal;
	
	struct timeval tv1;
	struct timeval tv2;
	
	struct timespec rem;
	struct timespec tc;
	
	printf("PID: %d \n", getpid());
	
	char *ptrArg;
	double sec = strtod( argv[1], &ptrArg);
	sec *= 0.01;
	
	if( sigaction(SIGUSR1, &sa, NULL) == -1)
		perror("sigaction ERROR! \n");
		
	tc.tv_sec = (time_t)sec;
	tc.tv_nsec = -((time_t)sec - sec) * (long int)1e9;

	gettimeofday( &tv1, NULL);
	
	t0 = tv1;

	while(1)
	{
		int nanosleep_status = nanosleep( &tc, &rem );
		if( nanosleep_status == -1 )
			tc=rem;
		else
			break;
	}

	gettimeofday( &tv2, NULL );

	printf("main: %lf\n",  (tv2.tv_sec - tv1.tv_sec ) + ( tv2.tv_usec - tv1.tv_usec) * 1e-6 );

	return 0;
}
