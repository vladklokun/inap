#include <unistd.h>
#include <inttypes.h>
#include <errno.h>
#include <assert.h>
#include <sys/syscall.h>

uint32_t sysrand(void);
uint32_t sysrandu(uint32_t up);
