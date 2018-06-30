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
	if( argc != 2 )
	{
		perror("Musisz podac tylko jeden wymagany argument!");
		_exit(1);
	}

	char *endPtr;
	unsigned int delay = strtol( argv[1], &endPtr, 10);
	while(1)
	{
		sleep(delay);
		raise(19);
	}

	return 0;
}
