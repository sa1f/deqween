
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "altera_avalon_timer_regs.h"
#include "altera_avalon_pio_regs.h"
#include "alt_types.h"
#include "sys/alt_irq.h"
#include "sys/alt_stdio.h"
#include "priv/alt_legacy_irq.h"
#include "system.h"
#include "../include/memory_map.h"
#include "../include/touchscreen.h"
#include "../include/wifi.h"
#include "../include/graphics.h"
#include "../include/servo.h"
#include "../include/globals.h"
#include "../include/gps.h"

volatile int x = 0;
volatile int i = 0x01;
uint32_t longitude;
uint32_t latitude;
std::vector<std::string> weather_data;

Graphics graphics;
Servo servo(0x80002030);
Wifi wifi(0x84000220);
GPS gps(0x84000210);

void isr_check_weather(void *context, alt_u32 id){
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0); //Clear IRQ status

	//Currently increments LEDs. Once wifi is connected this will be changed
	//x = x + i;
	//IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE, x);
	if(x == 12){
		gps.scanGPSData();
		longitude = gps.getLong();
		latitude = gps.getLat();
		weather_data = wifi.get_weather_data(longitude, latitude);
		x = 0;
	}
	else{
		x++;
	}

	//weather_type = wifi.get_weather_data().front();
	//std::string weather_type = v.front();
	//std::string temperature = v.back();


}



int main(void)
{
    TouchScreen touchScreen(0x84000230);

    //Register isr with TIMER_0
    alt_irq_register(TIMER_0_IRQ, 0, (alt_isr_func) isr_check_weather);

    /* Set period of interrupt. PeriodL is lower 16 bits [15:0] and PeriodH is higher 16 bits [31:16]
     * 0000_1F40 is interrupt period of 100 us.
     * Currently using a period of 0.25 seconds (0x0131_2D00) to show proper incrementing of LEDs
     * Will change later for weather checking
     */
     IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_0_BASE, 0x0800);
     IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_0_BASE, 0x2FAF);


      // Start timer
     IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, 7);


    // home page\
	gps.scanGPSData();
    gps.scanGPSData();
	longitude = gps.getLong();
	latitude = gps.getLat();
	usleep(5000000);
	weather_data = wifi.get_weather_data(longitude, latitude);
	printf("%s %s", weather_data[0].c_str(), weather_data[1].c_str());
    graphics.FrontPanel();



    //wifi.test_wifi();
    //printf("Just tested wifi\n");
    //weather_type = wifi.get_weather_data().front();


    while(true) {
    	touchScreen.GetPress();
    	TouchScreen::Point p = touchScreen.GetRelease();
    	std::vector<Graphics::FuncButton> buttons = graphics.getFuncButtons();
    	for(std::vector<Graphics::FuncButton>::iterator it = buttons.begin(); it != buttons.end(); ++it)
    		if (p.x >= it->x && p.x <= it->x + it->width &&
    			p.y >= it->y && p.y <= it->y + it->height) {
    			it->funcPtr(it->x, it->y, it->width, it->height);
    			break;
    		}
    	}
    return 0;
    }

    //while(true)
    //{
    //	if(gps.scanGPSData())
    //	{
    //		gps.getLat();
    //		gps.getLong();
    //	}
    //}
