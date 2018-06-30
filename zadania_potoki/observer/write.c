#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

sigset_t mask;

char *str1 = "CLOCK_MONOTONIC: ";
char *str2 = "CLOCK_MONOTONIC_COARSE: ";
char *str3 = "CLOCK_PROCESS_CPUTIME_ID: ";

char buf_sec[5] = {0};
char buf_nsec[10] = {0};
char buf_all[35]  = {0};

void writeClock(int fd, struct timespec tc1, struct timespec tc2, struct timespec tc3)
{
	sprintf(buf_sec, "%ld", tc1.tv_sec);
	sprintf(buf_nsec, "%ld", tc1.tv_nsec);
			
	strcat(buf_all, str1);
	strcat(buf_all, buf_sec);
	strcat(buf_all, " ");
	strcat(buf_all, buf_nsec);

	write(fd, buf_all, strlen(buf_all)+1);
	
	sprintf(buf_sec, "%ld", tc2.tv_sec);
	sprintf(buf_nsec, "%ld", tc2.tv_nsec);

	strcat(buf_all, str2);
	strcat(buf_all, buf_sec);
	strcat(buf_all, " ");
	strcat(buf_all, buf_nsec);

	write(fd, buf_all, strlen(buf_all)+1);
	
	sprintf(buf_sec, "%ld", tc3.tv_sec);
	sprintf(buf_nsec, "%ld", tc3.tv_nsec);

	strcat(buf_all, str3);
	strcat(buf_all, buf_sec);
	strcat(buf_all, " ");
	strcat(buf_all, buf_nsec);

	write(fd, buf_all, strlen(buf_all)+1);

}

int main(int argc, char * argv[])
{
	int fd;
	int count;
	char *myfifo = argv[1];

	struct timespec tc1;
	struct timespec tc2;
	struct timespec tc3;

	char write_buffer[4095];

	printf("I'm the write program. \n");
		
	sigemptyset(&mask);
	sigaddset(&mask, SIGPIPE);
	sigprocmask(SIG_SETMASK, &mask, NULL);

	while(1)
	{
		fd = open(myfifo, O_WRONLY);
		fgets(write_buffer, 4095, stdin);
		write(fd, write_buffer, strlen(write_buffer)+1);
		
		ioctl(fd, FIONREAD, &count);
		printf("ioctl() said %d bytes are available to read from pipe\n", count);
		
		if( count <= 2) 
		{
			clock_gettime(CLOCK_MONOTONIC, &tc1);	
			clock_gettime(CLOCK_MONOTONIC_COARSE, &tc2);
			clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tc3);
			
			writeClock(fd, tc1, tc2, tc3);
		}
		
	
		close(fd);
	}
	return 0;
}
