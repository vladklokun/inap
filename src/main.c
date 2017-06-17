#include <stdio.h>
#include <getopt.h>
#include <ctype.h> /* isprint() */

#include "password.h"

void usage(void);

void usage(void)
{
	fputs("Usage: inap [ OPTIONS ] [ elnum ] [ pwnum ]\n\n", stderr);

	fputs("Options:\n", stderr);
	fputs("-d /path/to/dictionary\n", stderr);
	fputs("\tUse given path as a dictionary file.\n", stderr);

	fputs("-l {en, ru}\n", stderr);
	fputs("\tUse predefined dictionary of specified language.\n", stderr);

	fputs("elnum\n", stderr);
	fputs("\tNumber of elements that comprise a password.\n", stderr);

	fputs("pwnum\n", stderr);
	fputs("\tNumber of passwords to generate.\n", stderr);

}

int main(int argc, char *argv[])
{
	uint16_t elnum = 6;
	uint16_t pwnum = 1;
	const char *dictpath = "dict/english";
	const char *sep = " ";
	char *tmp;

	struct Dictionary *dp = NULL;
	struct Password *pp = NULL;

	FILE *dfp = NULL;

	int c;

	while ((c = getopt(argc, argv, "d:hl:")) != -1 ) {
		switch (c) {
		case 'd':
			dictpath = optarg;
			break;
		case 'l':
			if (strncmp(optarg, "en", 2) == 0) {
				/* dictpath stays as english */
			} else if (strncmp(optarg, "ru", 2) == 0) {
				dictpath = "dict/russian";
			} else {
				fputs("unsupported dictionary\n", stderr);
				return 1;
			}
			break;
		case 'h':
		case '?':
			usage();
			return 1;
		default:
			abort();
		}
	}

	if (optind < argc) {
		elnum = strtol(argv[optind], &tmp, 0);
		if (*tmp) {
			fputs("Invalid number of passwords", stderr);
			return 1;
		}
		optind++;
	}

	if (optind < argc) {
		pwnum = strtol(argv[optind], &tmp, 0);
		if (*tmp) {
			fputs("Invalid number of elements in password", stderr);
			return 1;
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
		
		passgen(pp, dp, elnum, sep);
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
