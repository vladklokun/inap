#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef DICTIONARY_H
#define DICTIONARY_H
#include "dictionary.h"
#endif

int main(int argc, char *argv[])
{
	struct Dictionary	*dp = NULL;

	FILE *fp = NULL;

	if (argc < 2) {
		fputs("Filename!\n", stderr);
		return 1;
	}

	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		perror("main(): unable to open file");
		return 1;
	}

	//printf("filewc(): %zu\n", filewc(fp));
	//printf("filewc(): %zu\n", filewc(fp));

	dp = dictinit(dp, filewc(fp));

	dictloadfromfile(dp, fp);

	//printf("filewc(): %zu\n", filewc(fp));

	dictprint(dp);

	dictfree(dp);

	if (fclose(fp) == EOF) {
		perror("Unable to close fp");
		return 1;
	}

	return 0;
}
