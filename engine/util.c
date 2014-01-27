
#include "util.h"

#include <stdio.h>
#include <stdarg.h>

int point_intersect_box(int x1, int y1, int x2, int y2, int w2, int h2)
{
	if ((x1 >= x2) && (x1 <= x2+w2))
		if ((y1 >= y2) && (y1 <= y2 + h2))
			return 1;
	return 0;
}


void ENGINE_DEBUG_LOG(char * fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    va_end(ap);
}

void ENGINE_DEBUG_LOG_ERROR(char * fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
}
