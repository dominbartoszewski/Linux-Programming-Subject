#include <getopt.h>
#include <stdio.h>

int main(int argc, char * argv[])
{
	int             c;
	const char    * short_opt = "hf:";
	
	struct option   long_opt[] =
	{
		{"help",          no_argument,       NULL, 'h'},
		{"file",          required_argument, NULL, 'f'},
		{NULL,            0,                 NULL, 0  }
	};

	while((c = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1)
	{
		switch(c)
		{
			case -1:       /* no more arguments */
			case 0:        /* long options toggles */
				break;

			case 'f':
				printf("you entered \"%s\"\n", optarg);
				break;

			case 'h':
				printf("Usage: %s [OPTIONS]\n", argv[0]);
				printf("  -f file                   file\n");
				printf("  -h, --help                print this help and exit\n");
				printf("\n");
				return(0);

			case ':':
			case '?':
				fprintf(stderr, "Try `%s --help' for more information.\n", argv[0]);
				return(-2);

			default:
				fprintf(stderr, "%s: invalid option -- %c\n", argv[0], c);
				fprintf(stderr, "Try `%s --help' for more information.\n", argv[0]);
				return(-2);
		};
	};

	return(0);
}
