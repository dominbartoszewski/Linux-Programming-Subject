#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#define NANO 1000000000

void sleepNano( float time )
{
	struct timespec tc;
	tc.tv_sec = (time_t)time;
	tc.tv_nsec = (time - (time_t)time)*NANO;
	nanosleep( &tc, NULL);
}

int main( int argc, char *argv[] )
{
	if( argc != 2 )
	{
		perror("Zla liczba argumentow! \n");
		_exit(-1);
	}
	
	char *endPtr = NULL;
	float time = strtof( argv[1], &endPtr);
	printf("Czas: %f \n", time);
	
	char *buffer[4096];

	int read_fd;

	char *fifoNames[] = {
		"f1",
		"f2",
		"f3",
		"f4",
		"f5",
		"f6"
	};

	for( int i=0; i<6; i++ )
	{
		read_fd = open( fifoNames[i], O_RDONLY | O_NONBLOCK );

		read( read_fd, buffer, sizeof(buffer));
		write( STDOUT_FILENO, buffer, sizeof(buffer));
		close(read_fd);	
		sleepNano( time);
	}

	return 0;
}
