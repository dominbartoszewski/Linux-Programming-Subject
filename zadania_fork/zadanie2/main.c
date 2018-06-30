#include <err.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <string.h>

int main( int argc, char *argv[] )
{
	char *endPtr = NULL;
	
	long seconds = strtol( argv[1], &endPtr, 10);
	long nanoseconds = strtol( endPtr, &endPtr, 10);
	
	printf("Interval: %ld seconds %ld nanoseconds. \n", seconds, nanoseconds);

	unsigned int restarts = 0;
	struct timespec tc;
	// poki co beda tylko sekundy bo mi sie nie chce z tym meczyc nanosekundami
	
	tc.tv_sec = seconds;
	tc.tv_nsec = 0;
	
	
	for( int i=2; i<argc; i++)
	{	
		printf("Parametr %s \n",  argv[i]);
		nanosleep(&tc, NULL);	
		if( execvp(argv[0], argv) )
		{
			printf("Zrestartowano program... \n");
	
		}
		else
		{
			perror("Error in execvp! \n");

		}
		restarts++;
	}

	/*
	for( int i=2; i < argc; i++)
	{	
		printf("Parametr: %s \n", argv[i]); 
		argv[0] = 
		char *args[] = { argv[0], argv[i], (char *)NULL };
		execvp(args[0], args);
	}	
	*/

	return 0;
}
