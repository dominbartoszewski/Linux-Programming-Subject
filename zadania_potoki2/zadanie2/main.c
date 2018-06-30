#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define	PARENT_READ	readpipe[0]
#define	CHILD_WRITE	readpipe[1]
#define CHILD_READ	writepipe[0]
#define PARENT_WRITE	writepipe[1]

int main( int argc, char *argv[]){

	int writepipe[2];
	int readpipe[2];

	char *text = argv[1];
	
	if( pipe(readpipe) < 0 || pipe(writepipe) < 0 )
	{
		perror("pipe \n");
		return -1;
	}

	pid_t child_pid;
	
	for( int i=0; i<strlen(argv[1]); i++ )
	{
		if( (child_pid = fork()) == 0 )
		{
			dup2(STDIN_FILENO, CHILD_READ);
			dup2(STDOUT_FILENO, CHILD_WRITE);
			close(PARENT_WRITE);
			close(PARENT_READ);
			
			char buff_child_read[32];
			ssize_t readed_bytes = read( CHILD_READ, buff_child_read, sizeof(buff_child_read));
			//printf("buff_child_read: %s \n", buff_child_read);
			
			//write(CHILD_WRITE, buff_child_read, sizeof(buff_child_read));
			
			char buf[1];
			sprintf(buf, "%c", text[i]);
			//printf("buf: %s \n", buf);
			char *arg_list[] = {"/usr/bin/tr","-d", buf, (char *)NULL };
			execvp( arg_list[0] , arg_list );
			perror("execvp error \n");
		}
	}

	close(CHILD_READ);
	close(CHILD_WRITE);
	
	while(1)
	{
		char buff_stdin[32];
		ssize_t readed_bytes = read( STDIN_FILENO, buff_stdin, sizeof(buff_stdin));
		write(PARENT_WRITE, buff_stdin, sizeof(buff_stdin));
	
		char buff_stdout[32];
		readed_bytes = read( PARENT_READ, buff_stdout, sizeof(buff_stdout));	
		printf("buff_stdout: %s \n", buff_stdout);
	}
}

