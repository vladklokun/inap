#include "sysrand.h"

#if !defined(HAVE_GETRANDOM) && !defined(HAVE_SAFE_ARC4RANDOM)

# ifndef SSIZE_MAX
#  define SSIZE_MAX (SIZE_MAX/2 -1)
# endif
/*
 * This global variable is a file descriptor for urandom.
 */
static int urandomfd;

int
sysranddevopen(void)
{
	urandomfd = open("/dev/urandom", O_RDONLY);

	return urandomfd;
}

int
sysranddevclose(void)
{
	return close(urandomfd);
}

/* 
 * This function fills buffer buf with buflen bytes from /dev/urandom. Intended
 * for any OS that doesn't have an entropy supplying syscall (e.g macOS).
 * Assumes POSIX environment.
 */
ssize_t
getdevrandom(const int fd, void *buf, size_t buflen)
{
	ssize_t readbcnt = 0;

	assert(buflen > (ssize_t) 0U);
	assert(buflen < SSIZE_MAX);

	while ((readbcnt = read(fd, buf, buflen)) < (ssize_t) 0 ||
	        (errno == EAGAIN || errno == EINTR));

	return readbcnt;
}
#endif

uint32_t
sysrand(void)
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

/*
 * This function returns a uint32_t which is uniformly distributed on [0, up).
 */
uint32_t
sysrandu(uint32_t up)
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
