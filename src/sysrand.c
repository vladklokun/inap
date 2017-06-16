#include "sysrand.h"

uint32_t sysrand(void)
{
	uint32_t r = 0;
	int readb = 0;

	for (;;) {
		assert(sizeof(r) <= 256U);
		readb = syscall(SYS_getrandom, &r, sizeof(r), 0);

		if (readb >= 0 && (errno != EINTR || errno != EAGAIN))
			break;
	}

	return r;
}

uint32_t sysrandu(uint32_t up)
{
	uint32_t min = 0;
	uint32_t r = 0;

	if (up < 2)
		return 0;

	min = ~up % up;

	for (;;) {
		r = sysrand();

		if (r >= min)
			break;
	}

	return r % up;
}
