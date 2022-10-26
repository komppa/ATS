#include "external_time.h"

unsigned long get_time() {
#ifdef __GNUC__
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
#else
    return millis();
#endif
}