#include "osapi.h"

namespace osapi {

unsigned int getSystemTime() {
    struct timespec ts;

    clock_gettime(CLOCK_REALTIME, &ts);

    return (ts.tv_nsec / 1000000) + (ts.tv_sec * 1000ull);
}

void calcDelayedTimespec(struct timespec *ts, unsigned int milliseconds)
{

    clock_gettime(CLOCK_REALTIME, ts);

    unsigned int seconds = milliseconds / 1000;
    ts->tv_sec += seconds;
    milliseconds -= seconds * 1000;;

    long nanoseconds = milliseconds * 1000000;

    if (ts->tv_nsec + nanoseconds > 1000000000)
    {
        ++ts->tv_sec;
        nanoseconds -= 1000000000;
    }

    ts->tv_nsec = ts->tv_nsec + nanoseconds;
}

} // namespace osapi
