
#include <stdlib.h>
#include <stdio.h>

#include "../include/memory_map.h"
#include "../include/touchscreen.h"
#include "../include/gps.h"
#include "../include/wifi.h"
#include "../include/graphics.h"
#include "../include/servo.h"
#include "../include/globals.h"

Graphics graphics;
Servo servo(0x80002030);

int main(void)
{
    TouchScreen touchScreen(0x84000230);
    GPS gps(0x84000210);
    Wifi wifi(0x84000210);

    // home page
    graphics.FrontPanel();

    while(true) {
    	TouchScreen::Point p = touchScreen.GetRelease();
    	std::vector<Graphics::FuncButton> buttons = graphics.getFuncButtons();
    	for(std::vector<Graphics::FuncButton>::iterator it = buttons.begin(); it != buttons.end(); ++it)
    		if (p.x >= it->x && p.x <= it->x + it->width &&
    			p.y >= it->y && p.y <= it->y + it->height) {
    			it->funcPtr();
    		}
    	}
    }

    //wifi.test_wifi();
    //while(true)
    //{
    //	if(gps.scanGPSData())
    //	{
    //		gps.getLat();
    //		gps.getLong();
    //	}
    //}
