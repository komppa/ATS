#ifndef EXTERNAL_TIME_H
#define EXTERNAL_TIME_H

#ifdef __GNUC__
#include <stdio.h>
#include <time.h>
#include <math.h>
#ifndef ARDUINO
#include <sys/time.h>
#endif
#endif


unsigned long get_time();


#endif