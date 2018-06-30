#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <signal.h>

int main( int argc, char *argv[])
{
	if( argc < 2)
	{
		perror("Podales mniej niz wymagane 2 argumenty! \n");
		_exit(1);
	}
	
	pid_t child_pid, wait_pid;
	int status = 0;
	
	printf("PID rodzica: %d \n", getpid());
	
	char *args[3];

	args[0] = "./potomek.out";
	args[1] = argv[1];
	args[2] = (char *)NULL;
	
	struct timespec tc;

	for( int i=2; i<argc; i++ )
	{
		if( (child_pid = fork()) == 0 )
		{
			printf("PID dziecka %d, PID rodzica: %d. Parametr dla dziecka: %s \n", getpid(), getppid(), argv[i]);
			execvp( args[0], args );
			perror("Jakis blad przy execvp! \n");
			exit(0);
		}
	}

	do
	{
		wait_pid = waitpid( -1, &status, WUNTRACED | WCONTINUED );
		
		if( wait_pid == -1)
		{
			perror("waitpid error");
			exit(EXIT_FAILURE);
		}

		if( WIFEXITED(status) )
		{
			printf("Zawiadamiamy, że PID: %d umarł i zwrócił status %d \n", wait_pid, WEXITSTATUS(status));

			int result = clock_gettime(CLOCK_MONOTONIC, &tc);
			printf("tc.tv_sec: %lld \n", tc.tv_sec);
			printf("tc.tv_nsec: %ld \n", tc.tv_nsec);

		}
		else if( WIFSIGNALED(status) )
		{
			printf("Proces o PID: %d został zabity przez sygnał o numerze: %d \n", wait_pid, WTERMSIG(status));
		
			int result = clock_gettime(CLOCK_MONOTONIC, &tc);
			printf("tc.tv_sec: %lld \n", tc.tv_sec);
			printf("tc.tv_nsec: %ld \n", tc.tv_nsec);
		}
		else if( WIFSTOPPED(status) )
		{
			printf("Proces o PID: %d został zamrożony przez sygnał o numerze: %d \n", wait_pid, WSTOPSIG(status));
			int result = clock_gettime(CLOCK_MONOTONIC, &tc);
			printf("tc.tv_sec: %lld \n", tc.tv_sec);
			printf("tc.tv_nsec: %ld \n", tc.tv_nsec);

			kill(wait_pid, 18);
		}
		else if( WIFCONTINUED(status) )
		{
			printf("Proces o PID: %d zmartwychwstał za sprawą boskiego sygnału alleluja!! \n", wait_pid);
			
			int result = clock_gettime(CLOCK_MONOTONIC, &tc);
			printf("tc.tv_sec: %lld \n", tc.tv_sec);
			printf("tc.tv_nsec: %ld \n", tc.tv_nsec);
		}


	} while( !WIFEXITED(status) && !WIFSIGNALED(status) ); // jak wyslemy -TERM to juz sie zabije
	
	
	return 0;
}
