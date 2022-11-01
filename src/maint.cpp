#include "maint.h"


int maint() {

    int a;

    ExternalTime extTime;

    a = extTime.get_time();

    return a;
}
