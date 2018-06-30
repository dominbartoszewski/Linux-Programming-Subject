#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int spawn( char *program, char **arg_list)
{
	pid_t child_pid;
	child_pid = fork();

	if( child_pid != 0)
		return child_pid;
	else{
		execvp(program, arg_list);
		perror("Error in execvp! \n");
		abort();
	}
}

int main( int argc, char *argv[] )
{
	char * arg_list[] = {
		"ls",
		"-l",
		"/",
		NULL
	};
	
	spawn("ls", arg_list);
	printf("Done with main program! \n");
	
	return 0;
}
