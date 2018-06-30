#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>

sigset_t mask_usr1;

timer_t timerid;
timer_t timerid_usr1;
timer_t timerid_info_loop;

struct sigevent sev;
struct sigevent sev_usr1;
struct sigevent sev_info_loop;

struct itimerspec its;
struct itimerspec its_usr1;
struct itimerspec its_left;
struct itimerspec its_info_loop;

struct itimerspec its_left;
struct itimerspec its_usr1_left;
struct itimerspec its_info_loop_left;

long nanosec = 141592653;
time_t sec = 3;

long nanosec_usr1 = 648700000;
time_t sec_usr1 = 1;

long nanosec_info_loop = 718281000;
time_t sec_info_loop = 2;

volatile int flag = 0;

void sigHandler( int sigNum, siginfo_t *si, void *pVoid)
{
	if(sigNum == SIGUSR1)
	{
		sigemptyset(&mask_usr1);
		sigaddset(&mask_usr1, SIGUSR1);
		sigprocmask(SIG_SETMASK, &mask_usr1, NULL);
		timer_settime(timerid_usr1,0, &its_usr1, NULL);
		
	}
	else if( sigNum == SIGUSR2)
	{
		if(flag)
		{
			timer_gettime(timerid, &its_left);
			timer_gettime(timerid_usr1, &its_usr1_left);
			timer_gettime(timerid_info_loop, &its_info_loop_left);

			write(STDERR_FILENO, "Mode: toUpper()- ON \n", 23);
			printf("sec: %ld \n", its_left.it_value.tv_sec); // tak wiem zle
			printf("nsec: %ld \n", its_left.it_value.tv_nsec); // nie wolno printf
		}
		else
		{
				write(STDERR_FILENO, "Mode: toUpper()- OFF \n", 24);
		}
	}
	else if( sigNum == SIGRTMIN )
	{
		sigprocmask(SIG_UNBLOCK, &mask_usr1, NULL);
		flag = 0;
	}
	else if( sigNum == SIGRTMIN+1)
	{
		flag = 1;	
		timer_settime(timerid, 0, &its, NULL);
	}
	else if( sigNum == SIGRTMIN+2)
	{
		if(flag)
		{
			write(STDERR_FILENO, "Mode: toUpper()- ON \n", 23);
		}
		else
		{
			write(STDERR_FILENO, "Mode: toUpper() - OFF \n", 24);
		}
	}
}

void sigFunc()
{
	struct sigaction sa;
	memset( &sa, '\0', sizeof(sa));
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = sigHandler;

	if( sigaction(SIGUSR1, &sa, NULL) == -1 
			|| sigaction(SIGUSR2, &sa, NULL) == -1
			|| sigaction(SIGRTMIN, &sa, NULL) == -1
			|| sigaction(SIGRTMIN+1, &sa, NULL) == -1
			|| sigaction(SIGRTMIN+2, &sa, NULL) == -1)
	{
		perror("sigaction error! \n");
		exit(9);


	}
}

int main( int argc, char *argv[] )
{
	sigFunc();
	
	char c;
	char new_c;
	int r;
	
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = SIGRTMIN;
	sev.sigev_value.sival_ptr = &timerid;

	sev_usr1.sigev_notify = SIGEV_SIGNAL;
	sev_usr1.sigev_signo = SIGRTMIN+1;
	sev_usr1.sigev_value.sival_ptr = &timerid_usr1;
	
	its.it_value.tv_sec = sec;
	its.it_value.tv_nsec = nanosec;
	its.it_interval.tv_sec = its.it_interval.tv_nsec = 0;
	
	its_usr1.it_value.tv_sec = sec_usr1;
       	its_usr1.it_value.tv_nsec = nanosec_usr1;
	its_usr1.it_interval.tv_sec = its_usr1.it_interval.tv_nsec = 0;
	
	sev_info_loop.sigev_notify = SIGEV_SIGNAL;
	sev_info_loop.sigev_signo = SIGRTMIN+2;
	sev_info_loop.sigev_value.sival_ptr = &timerid_info_loop;
	
	its_info_loop.it_interval.tv_sec = its_info_loop.it_value.tv_sec = sec_info_loop;
	its_info_loop.it_interval.tv_nsec = its_info_loop.it_value.tv_nsec = nanosec_info_loop;
	
	if((timer_create(CLOCK_MONOTONIC, &sev, &timerid) == -1) 
			|| timer_create(CLOCK_MONOTONIC, &sev_usr1, &timerid_usr1) == -1
			|| timer_create(CLOCK_MONOTONIC, &sev_info_loop, &timerid_info_loop) == -1)
	{
		perror("timer_create error! \n");
		_exit(-1);
	}

	timer_settime( timerid_info_loop, 0, &its_info_loop, NULL);

	while( (r = read(STDIN_FILENO, &c, 1)))
	{
		if( r < 0 && errno == EINTR )
			continue;

		if(flag)
		{
			new_c = toupper(c);
		}
		else
		{
			new_c = c;
		}

		write(STDOUT_FILENO, &new_c, 1);
	}

	return 0;
}
