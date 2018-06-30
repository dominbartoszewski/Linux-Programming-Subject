#include <time.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>


int read_all_file(int fd)
{
        char *buffer = (char *)malloc(1);
        ssize_t read_status;
        lseek(fd, SEEK_SET, 0);
        while( ( read_status = read( fd, buffer, 1) ) != 0 )
        {
		int act_pos = lseek(fd, 0, SEEK_CUR);
                printf("buffer%d: %s \n", act_pos, buffer);
        }
        lseek(fd, SEEK_SET,0);
	return 1;
}

long count_bits(char *file_name)
{
	struct stat st;
	int status = stat(file_name, &st);
        printf("File %s size: %ld \n", file_name, st.st_size);
        return st.st_size;
}

int open_file(char *file_name, int flag)
{
        int fd = open(file_name, flag);
        if( fd < 0 )
        {
        	fprintf( stderr, "Open file %s error! \n", file_name);
                return -1;
        }

        printf("Opened file: %s with flag: %d \n", file_name, flag);

        return fd;

}

int main( int argc, char *argv[])
{
	int opt;
	char *filepath = (char *)malloc(255);
	long bits;
	long number;
	int act_pos;

	while( (opt = getopt( argc, argv, "f:n:")) != -1 )
	{	
		switch(opt)
		{
			case 'f':
                                filepath = strcpy( filepath, optarg );
                                //printf("filepath: %s \n", filepath);
				break;
			case 'n':
				number = strtol( optarg, NULL, 10 );
				printf("number: %ld \n", number);
				
				if( filepath )
				{
					//printf("filepath: %s \n", filepath);
					bits = count_bits( filepath);
				}
				int fd = open_file(filepath, O_RDWR);
				act_pos = lseek( fd, bits/2, SEEK_SET);
				printf("Pozycja w połowie pliku: %d \n", act_pos);			
				srand((unsigned int)time(NULL)); 
				for( int i = 0; i<number; i++)
				{
					int random_number = rand() % 6;
					printf("random_number: %d \n", random_number);
					int sign = rand() % 2 -1;
					if( sign == 0)
						sign = 1;

					printf("sign: %d \n", sign);
					int position_change = sign*(2<<random_number);		
					printf("position_change: %d \n", position_change);
					act_pos = lseek( fd, position_change, SEEK_CUR);
					printf("Pozycja po potędze: %d \n", act_pos );
					char *buffer = (char *)malloc(1);
					ssize_t read_status = read( fd, buffer, 1);
						     
					if( read_status == 0 )
					{
						printf("Koniec pliku! \n");
						ssize_t write_status = write( fd, "a", 1);
						printf("Status zapisania: %ld \n", write_status);
					}
					else if( buffer[0] == '\0' )
					{
						printf("Trafiono na bajt 0! \n");
						ssize_t write_status = write( fd, "a", 1);
                                                printf("Status zapisania: %ld \n", write_status);


					}
					else if( buffer[0] >= 'b' || buffer[0] <= 'y' )
					{
						printf("Trafiono na bajt od b do y a dokładniej: %s! \n", buffer);
						//lseek( fd, 1, SEEK_CUR);
						buffer[0] += 1;
                                                ssize_t write_status = write( fd, buffer, 1);
                                                printf("Status zapisania: %ld \n", write_status);
						read_all_file(fd);
					}
					else
					{
						printf("KOncze program! \n");
						exit(1);
					}
				}
				//printf("filepath: %s \n", filepath);
				//bits = count_bits( filepath);
				break;
			default:
				perror("Error! \n");
				exit(EXIT_FAILURE);
			

		}

	}

	return 0;
}
