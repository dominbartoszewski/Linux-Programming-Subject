#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

int main( int argc, char *argv[] )
{	
	struct timeval tv1;
	struct timeval tv2;
	
	int fd2;
	char *myfifo = argv[1];
	
	printf("Path to pipe: %s \n", myfifo);
	char read_buffer[4096];

	//sigemptyset(&mask);
	//sigaddset(&mask, SIGPIPE);
	//sigprocmask(SIG_SETMASK, &mask, NULL);
	
	while(1)
	{
		gettimeofday(&tv1, NULL);
	
		if( ( fd2 = open(myfifo, O_RDONLY)) == -1 )
		{
			perror("error opening pipe. \n");
			_exit(-1);
		}
		
		read( fd2, read_buffer, 4096 );
		printf("Pipka zawiera w sobie: %s \n", read_buffer);
		

		close(fd2);		
		gettimeofday(&tv2, NULL);
		
		printf("Size of sended bytes: %ld \n", sizeof(read_buffer));
		printf("Time in microseconds: %ld microseconds\n", ((tv2.tv_sec - tv1.tv_sec)*1000000L
					 +tv2.tv_usec) - tv1.tv_usec );
	}
	return 0;
}
