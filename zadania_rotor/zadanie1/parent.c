#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

int main( int argc, char *argv[] )
{
	pid_t child_pid, wait_pid;
	int status = 0;
	
	printf("PID rodzica: %d \n", getpid());
	
	char *args[4];

	args[0] = "./rotor.out";
	args[1] = argv[1];
	args[2] = argv[2];
	args[3] = (char *)NULL;

	if( (child_pid = fork()) == 0 )
	{
		printf("PID dziecka %d, PID rodzica: %d, Wiersz %s, Kolumna %s \n", getpid(), getppid(), args[1], args[2]);
		execvp( args[0], args );
		perror("Jakis blad przy execvp! \n");
		exit(0);
	}
	
	for( int i =0; i<6; i++)
	{
		sleep(1);
		kill(child_pid, 10);
	}

	return 0;
}
