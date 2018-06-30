#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

void makeChildren( char* argument, int intfd[], int outfd[] )
{
	int pid;
	for( int i=0; i<strlen(argument); i++ )
	{	
		char buf[20];
		sprintf(buf, "%c", argument[i]);

		if( ( pid = fork()) == 0) 
		{
			dup2(outfd[0], STDIN_FILENO);
			dup2(intfd[1], STDOUT_FILENO);

			close(outfd[0]); 
			close(outfd[1]);
			close(intfd[0]);
			close(intfd[1]);	

			fprintf( stderr, "%s: \n", buf);
			char *argv[]={ "/usr/bin/tr", "-d" , buf,  (char *)NULL};
			execvp(argv[0], argv);
			perror("execvp error \n");
		}
	}
}

char* parent_read_stdin(char *argument, int intfd[], int outfd[])
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

void write_to_child( char *buffer_read_stdin, int outfd[] )
{
	write(outfd[1], buffer_read_stdin, sizeof(buffer_read_stdin));
	close(outfd[1]);

	printf("Kom po write\n");		
}

void parent_read_from_tr(int intfd[])
{
	char buffer_read_parent[128];
	int count_read_parent = read(intfd[0], buffer_read_parent, sizeof(buffer_read_parent));

	printf("Kom po read\n");

	if( count_read_parent >=0 ) 
	{
		buffer_read_parent[count_read_parent] = 0;
		printf("buffer_read_parent: %s", buffer_read_parent);
	} 
	else 
		printf("buffer_read_parent error\n");
}

int main( int argc, char *argv[] )
{
	check_arg_valid(argc);

	char *argument = argv[1];	
	printf("strlen: %ld \n", strlen(argument));
	
	// aby nie bylo zombie
	struct sigaction sigchld_action = {
  		.sa_handler = SIG_DFL,
  		.sa_flags = SA_NOCLDWAIT
	};

	sigaction(SIGCHLD, &sigchld_action, NULL);

	while(1)
	{
		int outfd[2];
		int intfd[2];
		
		pipe(outfd);
		pipe(intfd);
		
		makeChildren( argument, intfd, outfd );
		
		close(outfd[0]);
		close(intfd[1]);
		
		char *buffer_read_stdin = parent_read_stdin( argument, intfd, outfd );
		write_to_child( buffer_read_stdin, outfd );
		parent_read_from_tr( intfd );
		
		sleep(1);
	}
}
