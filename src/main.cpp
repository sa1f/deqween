
#include <stdlib.h>
#include <stdio.h>

#include "../include/memory_map.h"
#include "../include/touchscreen.h"
#include "../include/gps.h"


int main(void)
{
    // Init drivers
    TouchScreen touchScreen();
    GPS gps();

    while(1)
    {
        // do project stuff
    }

}