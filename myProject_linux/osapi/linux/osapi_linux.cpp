#include "osapi.h"

namespace osapi {

unsigned int getSystemTime() {
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (ts.tv_nsec / 1000000) + (ts.tv_sec * 1000ull);
}

} // namespace osapi
