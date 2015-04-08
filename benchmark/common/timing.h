#include <sys/time.h>

double getCurrentTime() {
    struct timeval timstr;
    gettimeofday(&timstr, NULL);
    return timstr.tv_sec + (timstr.tv_usec / 1000000.0);
}
