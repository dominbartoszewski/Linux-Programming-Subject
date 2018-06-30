#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

int usr1_flag;
int usr2_flag;
double probability = 0.2;

void sigHandler( int sigNum, siginfo_t *siginfo, void *ptrVoid)
{
	if( sigNum == SIGCHLD )
	{
		//printf("siginfo->si_pid: %d \n", siginfo->si_pid);
		//printf("siginfo->si_signo: %d \n", siginfo->si_signo);
		//printf("siginfo->si_status: %d \n", siginfo->si_status);
		
		pid_t p = siginfo->si_pid;
		int status = siginfo->si_status;

		if( status == SIGSTOP )
		{
			kill(p, SIGCONT);
			
			usr1_flag = rand() <  probability * ((double)RAND_MAX + 1.0);
			usr2_flag = rand() <  probability * ((double)RAND_MAX + 1.0);
				
			if( usr1_flag )
			{
				kill(p, SIGUSR1);
			}
			else if( usr2_flag )
			{
				kill(p, SIGUSR2);
			}

		}
		else if( status == SIGTERM )
		{
			
		}

	}
}


void sigFunc()
{
	struct sigaction sa;
	memset( &sa, '\0', sizeof(sa));
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = sigHandler;

	if( sigaction( SIGCHLD, &sa, NULL ) == -1)
	{
		perror("sigaction error! \n");
		_exit(-1);
	}
}

int main( int argc, char *argv[] )
{
	srand((unsigned int)time(NULL));
	sigFunc();
	pid_t child_pid;
	
	printf("PID rodzica: %d \n", getpid());
	
	char *endInt = 0;
	int num_instances = (int)strtol(argv[2], &endInt, 10);
	printf("Podano %d liczbę instancji. \n", num_instances);

	int tab_childs[num_instances];
		
	for( int i=0; i<num_instances; i++)
	{
		char *args[4];
		
		char buf[50];
		sprintf(buf, "%d", 30+(4*i));
			
		args[0] = "./rotor.out";
		args[1] = argv[1]; // Y - wiersz
		args[2] = buf;
		args[3] = (char *)NULL;
		
		if( (child_pid = fork()) == 0 )
		{
			printf("getpid(): %d \n", getpid());
			execvp( args[0], args );
			perror("Jakis blad przy execvp! \n");
			_exit(-1);
		}
	}
	
	int i =0;
	while(1)
	{
		i++;
	}
	return 0;
}
