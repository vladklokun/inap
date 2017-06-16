#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef PASSWORD_H
#define PASSWORD_H
#include "password.h"
#endif

int main(void)
{
	struct Password *pp = NULL;

	pp = passinit(pp);

	//char h[] = {'h', 'e', 'l', 'l', 'o', 0};
	char h[] = "hhellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohellohelloello";
	passadd(pp, h);

	passprint(pp);
	puts("");

	passfree(pp);
	return 0;
}

