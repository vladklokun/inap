#include "dictionary.h"

size_t 
filewc(FILE *fp)
{
	char ch = '\0';

	size_t wc = 0;

	if (!fp) {
		fputs("filewc(): NULL passed. Aborting.\n", stderr);
		return 0;
	}

	while ((ch = fgetc(fp)) != EOF) {
		if (ch == '\n')
			++wc;
	}

	rewind(fp);

	return wc;
}

struct Dictionary
*dictinit(struct Dictionary *dp, size_t capacity)
{
	if (capacity == 0)
		return NULL;

	dp = malloc(sizeof(*dp));
	if (dp == NULL) {
		perror("dictinit(): unable to allocate dict");
		return NULL;
	}

	dp->wordlist = malloc(capacity * sizeof(*dp->wordlist));
	if (dp->wordlist == NULL) {
		perror("dictinit(): unable to allocate wordlist");
		return NULL;
	}
	dp->capacity = capacity;

	for (size_t i = 0; i < dp->capacity; i++) {
		dp->wordlist[i] = NULL;
	}

	dp->wc = 0;

	return dp;
}

int
dictfree(struct Dictionary *dp)
{
	for (size_t i = 0; i < dp->wc; i++)
		free(dp->wordlist[i]);
	dp->wc = 0;

	free(dp->wordlist);

	free(dp);

	return 0;
}

int
dictwordins(struct Dictionary *dp, const char *s)
{
	if (!dp) {
		fputs("dictwordins(): dp cannot be NULL\n", stderr);
		return 1;
	}

	if (!s) {
		fputs("dictwordins(): s cannot be NULL\n", stderr);
		return 1;
	}


	dp->wordlist[dp->wc] = calloc(strlen(s) + 1,
                                      sizeof(*dp->wordlist[dp->wc]));
	if (dp->wordlist == NULL) {
		perror("dictwordins(): unable to allocate word inside dict");
		return 1;
	}

	strncpy(dp->wordlist[dp->wc], s,
                (strlen(s) + 1) * sizeof(*dp->wordlist[dp->wc]));
	dp->wc++;

	return 0;
}

int
dictloadfromfile(struct Dictionary *dp, FILE *fp)
{
	char s[BUFSIZ] = "";

	if (!dp) {
		fputs("dictloadfromfile(): dp cannot be NULL\n", stderr);
		return 1;
	}

	if (!fp) {
		fputs("dictloadfromfile: fp cannot be NULL\n", stderr);
		return 1;
	}

	while (fgets(s, BUFSIZ * sizeof(s), fp)) {
		strtrim(s);

		if (dictwordins(dp, s) == 1) {
			fputs("dictloadfromfile(): dictwordins() failed\n",
			       stderr);
			return 1;
		}
	}

	rewind(fp);

	return 0;
}

int
dictprint(struct Dictionary *dp)
{
	if (!dp) {
		fputs("dictprint(): dp cannot be NULL\n", stderr);
		return 1;
	}

	for (size_t i = 0; i < dp->wc; i++) {
		//printf("\"%s\"\n", dp->wordlist[i]);
		printf("%s\n", dp->wordlist[i]);
	}

	return 0;
}


int
strtrim(char *s)
{
	if (s == NULL) {
		return 0;
	}

	s[strcspn(s, "\r\n")] = '\0';

	return 0;
}
