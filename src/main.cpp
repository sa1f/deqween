
#include <stdlib.h>
#include <stdio.h>

#include "../include/memory_map.h"
#include "../include/touchscreen.h"
#include "../include/gps.h"
#include "../include/wifi.h"
#include "../include/graphics.h"

int main(void)
{
    TouchScreen touchScreen(0x84000230);
    GPS gps(0x84000210);
    Wifi wifi(0x84000210);
    Graphics graphics;

	graphics.FrontPanel();
	graphics.LightPanel();
    wifi.test_wifi();
    while(true)
    {
    	if(gps.scanGPSData())
    	{
    		gps.getLat();
    		gps.getLong();
    	}
    }
}
