#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <getopt.h>

int main(int argc, char **argv)
{
	int c;
	int digit_optind = 0;

	while (1) {
		int option_index = 0;
		
		static struct option long_options[] = {
			{"delay",   required_argument, 0, 'd'},
			{0,         0,                 0,  0 }
		};

		c = getopt_long(argc, argv, "c:",
				long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
			case 'd':
				printf("option c with value '%s'\n", optarg);
				break;
			case '?':
				break;

			default:
				printf("?? getopt returned character code 0%o ??\n", c);
		}
	}

	if (optind < argc) {
		printf("non-option ARGV-elements: ");
		while (optind < argc)
			printf("%s ", argv[optind++]);
		printf("\n");
	}

	exit(EXIT_SUCCESS);
}
