#include <unistd.h>
#include <inttypes.h>
#include <errno.h>
#include <assert.h>
#include <sys/syscall.h>

#if !defined(HAVE_GETRANDOM)
# include <sys/stat.h> /* open() */
# include <fcntl.h> /* open() */

# include <limits.h> /* SSIZE_MAX */
#endif

#if !defined(HAVE_GETRANDOM)
int sysranddevopen(void);
int sysranddevclose(void);
ssize_t getdevrandom(const int fd, void *buf, size_t buflen);
#endif

uint32_t sysrand(void);
uint32_t sysrandu(uint32_t up);
