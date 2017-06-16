#include <stdio.h>

#include "sysrand.h"

int main(void)
{
	printf("sysrand() = %"PRIu32"\n", sysrand());
	printf("sysrandu(128) = %"PRIu32"\n", sysrandu(128));

	return 0;
}
