#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#define NANO 1000000000

void sleepNano( float timeSleep )
{
	struct timespec tc;
	tc.tv_sec = (time_t)timeSleep;
	tc.tv_nsec = (timeSleep - (time_t)timeSleep)*NANO;
	nanosleep( &tc, NULL);
}


int main( int argc, char *argv[] )
{
	srand((unsigned int)time(NULL));
	
	char *buffer[4096];

	int write_fd[6];
	
	char *fifoNames[] = {
		"f1",
		"f2",
		"f3",
		"f4",
		"f5",
		"f6"
	};

	int pantad_fd = open( "pantadeusz.txt", O_RDONLY );

	for( int i =0; i<6; i++ )
	{
		write_fd[i] = open( fifoNames[i], O_WRONLY );	
	}

	while(1)
	{
		int desc = rand() % 6;
		char c;
		int readed_bytes = read( pantad_fd, &c, 1 );

		if( readed_bytes )
		{
			write( write_fd[desc], &c, 1 );

			float slipTime = ( rand() % 3000 )/1000.0f; 
			sleepNano( slipTime);
		}
		else
		{
			return -1;
		}
	}

		return 0;
}
