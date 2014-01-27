
#ifndef _UTIL_H_
#define _UTIL_H_

#define max(a, b) ((a > b) ? a : b)
#define min(a, b) ((a < b) ? a : b)

void ENGINE_DEBUG_LOG(char * fmt, ...);

void ENGINE_DEBUG_LOG_ERROR(char * fmt, ...);

#endif
