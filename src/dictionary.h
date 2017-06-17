#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Dictionary {
	/* dynamically allocated array of c-strings */
	char **wordlist;
	/* number of words currently in dictionary */
	size_t wc;
	/* maximum current capacity */
	size_t capacity;
};

size_t filewc(FILE *fp);

struct Dictionary *dictinit(struct Dictionary *dp, size_t capacity);
int dictfree(struct Dictionary *dp);
int dictwordins(struct Dictionary *dp, const char *s);
int dictloadfromfile(struct Dictionary *dp, FILE *fp);
int dictprint(struct Dictionary *dp);

int strtrim(char *s);
