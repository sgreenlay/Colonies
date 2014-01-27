
#ifndef _UTIL_H_
#define _UTIL_H_

//is point x1,y1 inside {x2,y2} - {x2+w2, y2+h2}
int point_intersect_box(int x1, int y1, int x2, int y2, int w2, int h2);

void ENGINE_DEBUG_LOG(char * fmt, ...);

void ENGINE_DEBUG_LOG_ERROR(char * fmt, ...);

#endif
