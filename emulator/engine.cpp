#include "engine.h"

#ifdef _MSC_VER
#include "io.h"
#else
#include <fcntl.h>
#endif


size_t s_strlcpy(char *dst, const char *src, size_t siz)
{
	char *d = dst;
	const char *s = src;
	size_t n = siz;

	/* Copy as many bytes as will fit */
	if (n != 0) {
		while (--n != 0) {
			if ((*d++ = *s++) == '\0')
				break;
		}
	}

	/* Not enough room in dst, add NUL and traverse rest of src */
	if (n == 0) {
		if (siz != 0)
			*d = '\0';		/* NUL-terminate dst */
		while (*s++)
			;
	}

	return(s - src - 1);	/* count does not include NUL */
}

int s_open( const char * pathname, int flags)
{
#ifdef _MSC_VER
#ifdef WINCE
	return 0;
#else
	return open(pathname,flags);
#endif
#else
	return open(pathname,flags);
#endif
}