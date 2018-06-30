#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

int isNumber(char c);
long count_bits(char *file_name);
int open_file(char *file_name, int flag);
int decode_file(char *file_name, int fd, long bits);
int decode_file2(char *file_name, int fd, long bits);
int count_bits_to_truncate(int fd);
int isLetter(char c);
int code_file(int fd, long bits);

int count_bits_to_truncate(int fd)
{
	printf("reading all file started. \n");
        char *buffer = (char *)malloc(1);
        ssize_t read_status;
        lseek(fd, SEEK_SET, 0);
        
	int sum = 0;
	int num_sum = 0;

	while( ( read_status = read( fd, buffer, 1) ) != 0 )
        {
                //printf("buffer: %s \n", buffer);
		if( isNumber(buffer[0]) )
		{
			int number = strtol(buffer,0,10);
			sum += (number -1);
			++num_sum;

		}
	}

	lseek(fd, SEEK_SET ,0);
	printf("sum: %d \n", sum);
	printf("sum_num: %d \n", num_sum);
	printf("sum-num_sum: %d \n", sum-num_sum);
	printf("reading all file ended \n");
	return sum-num_sum;
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
		perror("Open file error! \n");
		return -1;
	}

	printf("Opened file: %s with flag: %d \n", file_name, flag);
	
	return fd;
	
}

int read_all_file(int fd)
{
	printf("reading all file started. \n");
	char *buffer = (char *)malloc(1);
	ssize_t read_status;
	lseek(fd, SEEK_SET, 0);
	while( ( read_status = read( fd, buffer, 1) ) != 0 )
	{
		printf("buffer: %s \n", buffer);

	}
	lseek(fd, SEEK_SET,0);
	printf("reading all file ended \n");
	return 1;
}

int decode_file(char *file_name, int fd, long bits)
{
	char *buffer = (char *)malloc(1);
	char *sign = (char *)malloc(1);
	ssize_t read_status;

	long lSize;
	int act_pos;
	int bits_to_truncate = count_bits_to_truncate( fd);
	ftruncate( fd, bits_to_truncate + bits);
	printf("Size of file: %ld \n", bits_to_truncate + bits);
	while(  (read_status = read( fd, buffer, 1) ) != 0)
	{
		if( isNumber(buffer[0]))
		{
			act_pos = lseek( fd, 0, SEEK_CUR );
			int num_chars = strtol(buffer, 0, 10);
			printf("Number! %d act_pos: %d \n", num_chars, act_pos);	
			read( fd, sign, 1);
			act_pos = lseek( fd, 0, SEEK_CUR );
			printf("After number: %s act_pos: %d\n", sign, act_pos);
			printf("bits+bits_to_truncate: %ld \n", bits+bits_to_truncate);
			lSize = bits + bits_to_truncate - act_pos;
			printf("lSize: %ld \n", lSize);
			char *buff = (char *)malloc(sizeof(char)*lSize);
			printf("sizeof(buff)+1 after malloc: %ld \n", sizeof(buff)+1);
			ssize_t temp_read_results = read( fd, buff, lSize);
			printf("temp_read_results: %ld \n", temp_read_results);
			printf("buff: %s sizeof(buff)+1: %ld \n", buff, sizeof(buff)+1);
			int temp_pos = act_pos-2;
			printf("temp_pos: %d \n", temp_pos);
			act_pos = lseek( fd, 0, SEEK_CUR);
			printf("Position after buffer: %d \n", act_pos);
			act_pos = lseek( fd, temp_pos, SEEK_SET);
			printf("Position before inserting letters: %d \n", act_pos);
			unsigned long i;
			
			for( i=0; i<num_chars; ++i)
			{
				ssize_t temp_bytes_written = write( fd, sign, 1);
			}
		
			int length = strlen(buff);


			if(buff != NULL)
			{		
				ssize_t temp_bytes_written = write( fd, buff, length);
				printf("length: %ld Bytes_written: %ld \n", length, temp_bytes_written);
				
			}
			else if( length == 0 )
			{
				printf("petla null! \n");
				ssize_t temp_bytes_written = write( fd, '\0', 1 );
				printf("length: %ld Bytes_written: %ld \n", length, temp_bytes_written);			              	      }

			act_pos = lseek(fd , act_pos+i, SEEK_SET);
			printf("Position after write: %d \n", act_pos);	
			printf("====\n");
			return 1;
		}
	}
	printf("\nDecoded file: %s \n", file_name);
	return 1;
}

int code_file( int fd, long bits)
{
	printf("bits: %ld \n", bits);

	char *buffer = (char *)malloc(1);
	char *next_sign = (char *)malloc(1);
	ssize_t read_status;
	int act_pos;
	int sum_num = 0;
	long lSize;

	while(  (read_status = read( fd, buffer, 1) ) != 0)
	{		
		++sum_num;
		act_pos = lseek( fd, 0, SEEK_CUR );
		printf("buffer: %s sum_num: %d act_pos %d \n", buffer, sum_num, act_pos);
		read_status = read( fd, next_sign, 1);
		printf("next_sign: %s \n", next_sign);
		act_pos = lseek( fd, 0, SEEK_CUR );
		int temp_pos = act_pos;
		printf("next_sign position: %d \n", act_pos);
		
		if( act_pos == bits )
			return 1;
		
		int i=0;
		if( next_sign[0] != buffer[0])
		{
			printf("%c i %c sa rozne! \n", buffer[0], next_sign[0]);
			lSize = bits - act_pos;
		        printf("lSize: %ld \n", lSize);
			char *buff = (char *)malloc(sizeof(char)*lSize);
			act_pos = lseek( fd, -1, SEEK_CUR);
			ssize_t temp_read_results = read( fd, buff, lSize);
			printf("temp_read_results: %ld \n", temp_read_results);
			printf("buff: %s \n", buff);
			act_pos = lseek( fd, act_pos, SEEK_SET );
			printf("position after reading rest: %d \n", act_pos);
			char *temp_sum_num = (char *)malloc(1);
			sprintf(temp_sum_num, "%d", sum_num);
			printf("sum_num before coding: %s \n", temp_sum_num);
			printf("temp_pos: %d \n", temp_pos);
			act_pos = lseek(fd, (-1)*(temp_pos-1), SEEK_CUR );
			printf("position before coding: %d \n", act_pos);
			ssize_t writen_byte_number = write( fd, temp_sum_num, 1 );
			act_pos = lseek( fd, 1, SEEK_CUR);
			printf("position after coding: %d \n", act_pos);
			writen_byte_number = write( fd, buff, sizeof(buff));
			act_pos = lseek( fd, (-1)*writen_byte_number, SEEK_CUR);
			printf("position after inserting buffer: %d \n", act_pos);
			sum_num = 0;	
			return 1;	
		}
		else
		{
			printf("%c i %c sa takie same! \n", buffer[0], next_sign[0]);
			act_pos = lseek( fd, -1, SEEK_CUR);
			printf("pos after checking next sign: %d \n", act_pos);
		
		}

		printf("====\n");

		//return 1;	
	}



	return 1;

}
int write_file(char *file_name, int fd, long bits, char *buffer)
{
	


	return 1;
}

int isNumber(char c)
{
	if ((c >= '0' && c <= '9'))
		return 1;
	else
		return 0;

}

int isLetter(char c)
{
	if( (c >= 'a'  &&  c <= 'z') )
		return 1;
	if( (c >= 'A'  && c <= 'Z') )
		return 1;
	else
		return 0;
}

int move_eof(int fd, long size, int bit_eof)
{
	char *buffer = (char *)malloc(1);
	ssize_t read_status;
        int act_pos = lseek(fd, SEEK_SET, bit_eof);
	printf("Position EOF: %d \n", act_pos);
	read( fd, buffer, 1);
	printf("buffer EOF: %s \n", buffer);
	lseek(fd, SEEK_SET,0);
       	return 1;
}

int decode_file2(char *file_name, int fd, long bits)
{
	char *buffer = (char *)malloc(1);
        char *sign = (char *)malloc(1);
        ssize_t read_status;
	int bits_to_move_eof = count_bits_to_truncate(fd);
	ftruncate( fd, bits);
	//move_eof(fd, bits, bits_to_move_eof);
        long lSize;
        int act_pos;
	printf("Size of file: %ld \n", bits);
	while(  ( read_status = read( fd, buffer, 1) ) != 0)
	{
		if( isNumber(buffer[0]))
		{
			act_pos = lseek( fd, 0, SEEK_CUR );
			int num_chars = strtol( buffer, 0, 10);
			printf("Number! %d act_pos: %d \n", num_chars, act_pos);        
			read( fd, sign, 1);
                        act_pos = lseek( fd, 0, SEEK_CUR );
                        printf("After number: %s act_pos: %d\n", sign, act_pos);
			lSize = bits - act_pos;
			int temp_pos = act_pos;		
			printf("lSize: %ld \n", lSize);
			char *buff = (char *)malloc(sizeof(char)*lSize);
			ssize_t temp_read_results = read( fd, buff, lSize);
			printf("temp_read_results: %ld \n", temp_read_results);
			printf("buff: %s \n", buff);
			act_pos = lseek( fd, 0, SEEK_CUR);
			printf("Position after reading buffer: %d \n", act_pos);
			act_pos = lseek( fd, temp_pos-2, SEEK_SET);
			printf("Position before inserting letters: %d \n", act_pos);
			printf("======\n");

			for( unsigned int i=0; i<num_chars; ++i)
			{
				ssize_t temp_bytes_written = write( fd, sign , 1 );
			}
				
			int length = strlen(buff);

			printf("Length: %d \n", length);
			
			if( buff != NULL)
			{
				write( fd, buff, length );
			}

			printf("koniec!");
		}
	}
	printf("\nDecoded file: %s \n", file_name);

}


int main( int argc, char *argv[] )
{
	int opt;
	char *filepath = (char*)malloc(255);
	long bits;
	
	while( (opt = getopt( argc, argv, "cdt:f:")) != -1 )
	{
		switch(opt)
		{
			case 'c':
				printf("Kodowanie... \n");
				int fd_ru = open_file(filepath, O_RDWR);
				printf("fd_ru = %d \n", fd_ru);
				code_file( fd_ru, bits);
				//printf("before reading file %s \n", filepath);
				//read_all_file(fd_ru);
				//printf("after reading file %s \n", filepath);
				close( fd_ru);
				break;
			case 'd':
				printf("Dekodowanie... \n");
				int fd_rd = open_file(filepath, O_RDWR);
				printf("fd_rd = %d \n", fd_rd);
				int bits_to_truncate = count_bits_to_truncate( fd_rd );
				decode_file(filepath, fd_rd, bits);
				//decode_file2( filepath, fd_rd, bits+bits_to_truncate);
				read_all_file(fd_rd);
				close( fd_rd );
				break;
			case 'f':
				filepath = strcpy( filepath, optarg );
				printf("filepath: %s \n", filepath);
				break;
			case 't':
				printf("Test... \n");
				bits = count_bits(filepath);
				break;
			default :
				perror("Usage: ./a.out [-c] [-d] [-t] \n");
				exit(EXIT_FAILURE);
		}
	}
	return 0;
}
