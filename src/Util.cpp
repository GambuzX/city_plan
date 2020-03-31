#include <random>
#include "Util.h"

int getRandomValue(){
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    srand((long)ts.tv_sec * 1000000000L + ts.tv_nsec);
    return rand(); //Avoiding the limitations of using time(NULL) as a seed for rand()
}