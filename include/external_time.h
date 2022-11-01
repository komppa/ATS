#ifndef EXTERNAL_TIME_H
#define EXTERNAL_TIME_H

#include "gmock/gmock.h"


class ExternalTime {
    private:
        int time;
    public:
        ExternalTime();
        virtual int get_time();
};



class MockExternalTime: public ExternalTime {
public:
    MOCK_METHOD(int, get_time, ());
};

#endif
