#include <getopt.h>
#include <stdio.h>

int main(int argc, char * argv[])
{
	int c;
	const char *short_opt = "hd:";

	struct option long_opt[] =
	{
		{ "help",	no_argument,       NULL, 'h'},
		{ "delay",	required_argument, NULL, 'd'},
		{ NULL,		0,                 NULL,  0 }
	};

	while((c = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1)
	{
		switch(c)
		{
			case 'd':
				printf("Entered delay: \"%s\"\n", optarg);
				


				break;

			case 'h':
				printf("Usage: %s [OPTIONS]\n", argv[0]);
				printf("  -d <float>, --delay=<float>  		delay\n");
				printf("  -h, --help  				Print help \n");
				printf("\n");
				return(0);

			case ':':
			case '?':
				fprintf(stderr, "Try `%s --help' for some help.\n", argv[0]);
				return(-2);

			default:
				fprintf(stderr, "%s: invalid option -- %c\n", argv[0], c);
				fprintf(stderr, "Try `%s --help' for some help.\n", argv[0]);
				return(-2);
		};
	};

	if (optind < argc) {
		printf("FIFO paths: \n");
		while (optind < argc)
			printf("%s \n", argv[optind++]);
	}

	return(0);
}
