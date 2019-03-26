#include <time.h>

int main(){
    //int milisec = 1000000; // length of time to sleep, in miliseconds

    //struct timespec req = {0};
    //req.tv_sec = 0;
    //req.tv_nsec = milisec * 1000000L;
    //nanosleep(&req, (struct timespec *)NULL);

    #define NANO_SECOND_MULTIPLIER  1000000  // 1 millisecond = 1,000,000 Nanoseconds
    const long INTERVAL_MS = 5 * NANO_SECOND_MULTIPLIER;

    timespec sleepValue = {0};
    sleepValue.tv_nsec = INTERVAL_MS;

    nanosleep(&sleepValue, NULL);
    
    return 0;
}
