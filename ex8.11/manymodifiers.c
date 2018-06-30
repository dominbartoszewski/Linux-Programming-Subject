#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

int spawn( char *program, char **arg_list )
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

int main (int argc, char *argv[])
{
	char *args_path[]={ "./modifier.out", "-f testfile.txt", NULL };
	char *args_desc[]={ "cat testfile.txt | ./modifier.out", "-d 0,1", NULL };
	
	int opt;
	int endl_flag = 0;
	int path_flag;
	int desc_flag;

	char *loaded_num_instances = 0;
	char *endInt = 0;
	int num_instances;
	char *loaded_delay = 0;
	char *endIntDelay = 0;
	long num_delay;
	
	while( (opt = getopt( argc, argv, "ZDFd:n::")) != -1)
	{	
		switch(opt){
		case 'Z':
			endl_flag = 1;
			break;
		case 'D':
			desc_flag = 1;
			path_flag = 0;
			break;
		case 'F':
			path_flag = 1;
			desc_flag = 0;
			break;
		case 'd':
			loaded_delay = optarg;
			num_delay = strtol(loaded_delay, &endIntDelay, 10);
			printf("Podane %ld ms opóźnienia między procesami. \n", num_delay);
			break;
		case 'n':
			if( optarg == NULL )
			{
				loaded_num_instances = argv[optind];
				if( loaded_num_instances == NULL )
				{
						perror("Podano zla liczbe instancji! \n"); 
						exit(-1);
				}
				
				num_instances = (int)strtol(loaded_num_instances, &endInt, 10);
				printf("Podano %d liczbę instancji. \n", num_instances);
				break;
			}
			num_instances = (int)strtol(loaded_num_instances, &endInt, 10);
			printf("Podano %d liczbę instancji. \n", num_instances);
			break;
		case '?':
		default:
			perror("Zły parametr! \n");
			break;
		}
	}
	
	struct timespec time;
	time.tv_sec = 0;
	time.tv_nsec = 100000000L;
	
	char buffer[255];
	int read_bytes;
	
	if(path_flag)
	{
		if(endl_flag)
		{
			for( int i=0; i<num_instances; i++)
			{
				spawn(args_path[0], args_path);
				nanosleep(&time, NULL);
			}

			while ((read_bytes = read(STDIN_FILENO,buffer,255)) > 0) 
			{
	                	write(STDOUT_FILENO,buffer,read_bytes);
				fprintf(stderr,"read %d bytes\n",read_bytes);
			}
		}
		else
		{
			for( int i=0; i<num_instances; i++)
			{
				spawn(args_path[0], args_path);
			}
		}
	}

	if(desc_flag)
	{
		for( int i=0; i<num_instances; i++)
		{
			spawn(args_desc[0], args_desc);
			nanosleep(&time, NULL);
		}

		while ((read_bytes = read(STDIN_FILENO,buffer,255)) > 0) 
		{
	                write(STDOUT_FILENO,buffer,read_bytes);
			fprintf(stderr,"read %d bytes\n",read_bytes);
		}
	}
	else
	{
		for( int i=0; i<num_instances; i++)
		{
			spawn(args_desc[0], args_desc);
		}
	}
		
	printf("Ending \n");
	return 0;
}
