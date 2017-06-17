#include <stdio.h>
#include <getopt.h>
#include <ctype.h> /* isprint() */

#include "password.h"

int main(int argc, char *argv[])
{
	uint16_t wordnum = 6;
	uint16_t pwnum = 1;
	const char *dictpath = "dict/english";
	const char *sep = " ";

	struct Dictionary *dp = NULL;
	struct Password *pp = NULL;

	FILE *dfp = NULL;

	int c;

	while ((c = getopt(argc, argv, "d:l:")) != -1 ) {
		switch (c) {
		case 'd':
			dictpath = optarg;
			break;
		case '?':
			if (optopt == 'd') {
				fprintf(stderr,
					"Option -%c requires an argument\n",
					optopt);
			} else if (isprint(optopt)) {
				fprintf(stderr, "Unknown option -%c\n", optopt);
			} else {
				fprintf(stderr,
					"Unknown option char `\\x%x`", optopt);
			}
			return 1;
		default:
			abort();
		}
	}

#if !defined(HAVE_GETRANDOM) && !defined(HAVE_SAFE_ARC4RANDOM)
	if (sysranddevopen() == -1) {
		perror("unable to open urandom");
		return 1;
	}
#endif
	dfp = fopen(dictpath, "r");
	if (!dfp) {
		perror("unable to open dictionary");

		return 1;
	}

	dp = dictinit(dp, filewc(dfp));
	if (!dp) {
		fputs("main(): dictinit() failed\n", stderr);
		return 1;
	}

	if (dictloadfromfile(dp, dfp) == 1) {
		fputs("main(): dictloadfromfile() failed\n", stderr);
		return 1;
	}

	for (uint16_t i = 0; i < pwnum; i++) {
		pp = passinit(pp);
		
		passgen(pp, dp, wordnum, sep);
		passprint(pp);
		
		passfree(pp);
	}
	dictfree(dp);

	if (fclose(dfp) == EOF) {
		perror("unable to close dictionary");
		
		return 1;
	}

#if !defined(HAVE_GETRANDOM) && !defined(HAVE_SAFE_ARC4RANDOM)
	sysranddevclose();
#endif

	return 0;
}
