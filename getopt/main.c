#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

int main( int argc, char **argv )
{
	while(1)
	{
		char opt;
		opt = getopt(argc,argv, "ab:");
		if( opt == -1)
		{
			break;
		}

		switch(opt)
		{
		case'a':
			printf("Podano argument -a. \n");
			break;
		case 'b':
			printf("Podano argument -b %s. \n", optarg);
			break;
		case '?':
		default:
			printf("Użycie: %s [-a] [-b <coś>].\n",  argv[0]);

		}
	}
	
	printf("optind: %d \n", optind);
	printf("argc: %d \n", argc);

	argc -= optind;
	argv += optind; // przesuniecie wskaznika do miejca gdzie juz nie ma -

	if( argc > 0 )
	{
		printf("Zostalo jeszcze %d argumentow poza minusem: \n", argc);

		for( int i =0; i < argc; i++)
		{
			printf("Argument %d: %s \n", i, argv[i]);
		}

	}

	return 0;
}
