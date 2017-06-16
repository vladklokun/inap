#include "password.h"

struct Password
*passinit(struct Password *pp)
{
	pp = malloc(sizeof(*pp));
	if (!pp) {
		perror("passinit(): malloc failed");
		return NULL;
	}

	pp->password = calloc(INITPASSSIZE, sizeof(*pp->password));
	if (!pp->password) {
		perror("passinit(): calloc failed");
		return NULL;
	}

	pp->capacity = INITPASSSIZE * sizeof(*pp->password);

	pp->size = 0;

	pp->entropy = 0;

	return pp;
}

int
passfree(struct Password *pp)
{
	if (!pp)
		return 0;

	free(pp->password);

	pp->capacity = 0;
	pp->size = 0;
	pp->entropy = 0;

	free(pp);

	return 0;
}


int
passcanfit(struct Password *pp, const char *s)
{
	if (!pp) {
		fputs("passcanfit(): pp cannot be NULL", stderr);
		return 1;
	}

	return (pp->capacity > (pp->size + strlen(s)));
}

int
passgrow(struct Password *pp)
{
	char *tmp = NULL;

	if (!pp) {
		fputs("passgrow(): pp cannot be NULL", stderr);
		return 1;
	}

	if (pp->capacity < 1)
		pp->capacity = 1;

	tmp = realloc(pp->password, GROWTHFACTOR * pp->capacity);
	if (!tmp) {
		perror("passgrow(): realloc failed");
		return 1;
	}

	pp->capacity *= GROWTHFACTOR;
	pp->password = tmp;

	return 0;
}

int
passadd(struct Password *pp, const char *s)
{
	if (!pp) {
		fputs("passadd(): pp cannot be NULL\n", stderr);
		return 1;
	}

	if (!s) {
		fputs("passadd(): s cannot be NULL\n", stderr);
		return 1;
	}

	char *tmp = calloc(BUFSIZ, sizeof(*tmp));
	if (!tmp) {
		fputs("passadd(): unable to calloc() tmp", stderr);
		return 1;
	}

	strncpy(tmp, s, BUFSIZ * sizeof(*tmp) - 1);

	while (!passcanfit(pp, tmp)) {
		if (passgrow(pp) == 1) {
			fputs("passadd(): unable to grow password\n", stderr);
			return 1;
		}
	}

	/* 
	 * Trim newline ONLY IF target characters are found.
	 */
	if (strcspn(tmp, "\r\n") != strlen(tmp))
		tmp[strcspn(tmp, "\r\n")] = '\0';

	if (!strncat(pp->password, tmp, pp->capacity - pp->size)) {
		fputs("passadd(): unable to insert element\n", stderr);
		return 1;
	}
	pp->size += (strlen(tmp) + 1);

	free(tmp);

	return 0;
}

int
passprint(struct Password *pp)
{
	if (!pp) {
		fputs("passprint(): pp cannot be NULL\n", stderr);
		return 1;
	}

	puts(pp->password);

	return 0;
}

int
passgen(struct Password *pp, struct Dictionary *dp,
	uint16_t wordnum, const char *sep)
{
	if (!pp) {
		fputs("passgen(): pp cannot be NULL", stderr);
		return 1;
	}

	if (!dp) {
		fputs("passgen(): dp cannot be NULL", stderr);
		return 1;
	}

	if (!sep) {
		fputs("passgen(): sep cannot be NULL", stderr);
		return 1;
	}

	for (uint16_t i = 0; i < wordnum; i++) {
		passadd(pp, dp->wordlist[sysrandu(dp->wc)]);
		passadd(pp, sep);
	}

	return 0;
}
