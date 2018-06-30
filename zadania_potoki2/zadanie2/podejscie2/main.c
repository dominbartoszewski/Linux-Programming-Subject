#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#define CHILD_READ outfd[0]
#define CHILD_WRITE intfd[1]
#define PARENT_READ intfd[0]
#define PARENT_WRITE outfd[1]

void check_arg_valid(int argc)
{
	if(argc != 2)
	{
		perror("Podano złą liczbę argumentów! \n");
		_exit(-1);
	}
}

void makeChildren( char* argument, int intfd[][2], int outfd[][2] )
{
	int pid;
	long length = strlen(argument);
	for( int i=0; i<length; i++ )
	{	
		char buf[20];
		sprintf(buf, "%c", argument[i]);

		if( ( pid = fork()) == 0) 
		{
			dup2(outfd[i][0], STDIN_FILENO);
			dup2(intfd[i][1], STDOUT_FILENO);
			
			//dup2(STDIN_FILENO, outfd[i][0]);
			//dup2(STDOUT_FILENO, intfd[i][1]);

			close(outfd[i][0]); 
			close(outfd[i][1]);
			close(intfd[i][0]);
			close(intfd[i][1]);	

			fprintf( stderr, "%s: \n", buf);
			char *argv[]={ "/usr/bin/tr", "-d" , buf,  (char *)NULL};
			execvp(argv[0], argv);
			perror("execvp error \n");
		}
	}
}

char* parent_read_stdin(char *argument)
{
	char *buffer_read_stdin = (char*)malloc(128);

	int count_read_stdin = read( STDIN_FILENO, buffer_read_stdin, sizeof(buffer_read_stdin));

	if( count_read_stdin >= 0 )
	{
		buffer_read_stdin[count_read_stdin] = 0;
		printf("buffer_read_stdin: %s", buffer_read_stdin);
	} 
	else 
		printf("buffer_read_stdin error! \n");

	return buffer_read_stdin;
}

void parent_read_from_tr(int intfd[][2], int fd_number)
{
	char buffer_read_parent[128];
	
	//int flags = fcntl( intfd[fd_number][0], F_GETFL, 0);
	//fcntl( intfd[fd_number][0], F_SETFL, flags | O_NONBLOCK );

	printf("Kom przed readem \n");
	int count_read_parent = read( intfd[fd_number][0], buffer_read_parent, sizeof(buffer_read_parent));

	printf("Kom po read\n");

	if( count_read_parent >=0 ) 
	{
		buffer_read_parent[count_read_parent] = 0;
		printf("buffer_read_parent: %s\n", buffer_read_parent);
	} 
	else 
		printf("buffer_read_parent error\n");
}

void write_to_child( char *buffer_read_stdin,  int outfd[][2], int fd_number )
{
	long length = strlen(buffer_read_stdin) - 1;
	write(outfd[fd_number][1], buffer_read_stdin, length);
	close(outfd[fd_number][1]);

	printf("Kom po write\n");
}


int main( int argc, char *argv[] )
{
	check_arg_valid(argc);

	char *argument = argv[1];	
	long length = strlen(argument);

	printf("strlen: %ld \n", length);
	
	// aby nie bylo zombie
	struct sigaction sigchld_action = {
  		.sa_handler = SIG_DFL,
  		.sa_flags = SA_NOCLDWAIT
	};

	sigaction(SIGCHLD, &sigchld_action, NULL);

	while(1)
	{
		int outfd[length][2];
		int intfd[length][2];
		
		for( int i=0; i<length; i++ )
		{
			pipe(outfd[i]);
			pipe(intfd[i]);
		}
		
		makeChildren( argument, intfd, outfd );
		
		for( int i=0; i<length; i++)
		{
			close(outfd[i][0]);
			close(intfd[i][1]);
		}
		
		char *buffer_read_stdin = parent_read_stdin( argument );
		
		for( int i=0; i<length; i++ )
		{
			write_to_child( buffer_read_stdin, outfd, i );			
			parent_read_from_tr( intfd, i );		
		}			

		sleep(1);
	}
}
