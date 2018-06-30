#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>

int readFromPath(char *path)
{
	int fd_rd = open(path, O_RDONLY);
	int fd_rw = open(path, O_WRONLY);
	
	char buffer[512] = { 0 };
	
	struct timespec tim = { 0 };
	tim.tv_sec = 2;
	tim.tv_nsec = 500000000L;

	ssize_t readed_bytes;
	ssize_t writed_bytes;

	lseek(fd_rd, 0, SEEK_SET);
	lseek(fd_rw, 0, SEEK_SET);


	while(1)
	{
		readed_bytes = read(fd_rd, buffer, 512);
		
		if(!readed_bytes)
			break;

		for( unsigned int i = 0; i < readed_bytes; i++)
		{
			buffer[i]++;
		}

		writed_bytes = write( fd_rw, buffer, readed_bytes);
		//nanosleep(&tim, NULL);
	}

	return 1;
}

int checkFdValid( int fd )
{
	return fcntl(fd, F_GETFL) != -1 || errno != EBADF;

}

int readFromDescriptors(int fd1, int fd2)
{
	if( checkFdValid(fd1))
	{
		perror("checkfd1valid: %d \n");
	}

	if( checkFdValid(fd2))
	{
		perror("checkfd2valid: %d \n");
	}

	char buffer[512] = { 0 };
	ssize_t readed_bytes;
	ssize_t writed_bytes;

	lseek(fd1, 0, SEEK_SET);
	lseek(fd2, 0, SEEK_SET);

	while(1)
	{
		readed_bytes = read(fd1, buffer, 512);
		if(!readed_bytes)
			break;

		for( unsigned int i = 0; i < readed_bytes; i++)
		{
			buffer[i]++;
		}
		writed_bytes = write( fd2, buffer, readed_bytes);
	}
	return 1;
}


int main( int argc, char *argv[] )
{
	int opt;
	char *path = 0;
	int firstInt;
	int secondInt;
	char *loadedInt = 0;
	char *endInt = 0;

	while( (opt = getopt( argc, argv, "d::f::")) != -1)
	{
		switch(opt){
		case 'f':
			if( optarg == NULL)
			{
				path = argv[optind];
				if( path == NULL )
				{
					perror("Podano zla sciezke! \n");
					exit(-1);
				}

				printf("Podano sciezke %s \n", path);
				readFromPath(path);
				break;
			}
			printf("Podano sciezke: %s \n", optarg);
			path = optarg;
			readFromPath(path);
			break;
		case 'd':
			if( optarg == NULL )
			{
				loadedInt = argv[optind];
				
				if( loadedInt == NULL )
				{
					perror("Podano zle numery deskryptorów! \n");
					exit(-1);
				}

				firstInt = (int)strtol(loadedInt, &endInt, 10);

				if( *endInt++ != ',')
				{	
					perror("Zły parametr! <int>,<int> \n ");
					exit(-1);
				}

				secondInt = (int)strtol(endInt, &loadedInt, 10);
				printf("Wczytane liczby: %d , %d \n", firstInt, secondInt);
				readFromDescriptors(firstInt, secondInt);
				break;
			}

			loadedInt = optarg;
			firstInt = (int)strtol(loadedInt, &endInt, 10);

			if( *endInt++ != ',')
			{
				perror("Zły parametr! <int>,<int> \n ");
				exit(-1);
			}

			secondInt = (int)strtol(endInt, &loadedInt, 10);
			printf("Wczytane liczby: %d , %d \n", firstInt, secondInt);
			readFromDescriptors(firstInt, secondInt);
			break;
		case '?':
		default:
			perror("Zły parametr! \n");
			break;
		}
	}
	return 0;
}
