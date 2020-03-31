#include <random>
#include "Util.h"

uint getRandomValue(){
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return rand() + (long)ts.tv_sec * 1000000000L + ts.tv_nsec; //Avoiding the limitations of using time(NULL) as a seed for rand() (not very elegant, just a simple workaround)
}