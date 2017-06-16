#define INITPASSSIZE 128

#define GROWTHFACTOR 2

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "dictionary.h"
#include "sysrand.h"

struct Password {
	/* password contents */
	char *password;
	/* current maximum capacity */
	size_t capacity;
	/* current size */
	size_t size;
	/* current entropy */
	double entropy;
};

struct Password *passinit(struct Password *pp);
int passfree(struct Password *pp);

int passcanfit(struct Password *pp, const char *s);
int passgrow(struct Password *pp);

int passadd(struct Password *pp, const char *s);
int passgen(struct Password *pp, struct Dictionary *dp,
            uint16_t wordnum, const char *s);

int passprint(struct Password *pp);
