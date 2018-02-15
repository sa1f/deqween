
#include <stdlib.h>
#include <stdio.h>

#include "altera_avalon_timer_regs.h"
#include "altera_avalon_pio_regs.h"
#include "alt_types.h"
#include "sys/alt_irq.h"
#include "sys/alt_stdio.h"
#include "priv/alt_legacy_irq.h"
#include "system.h"
#include "../include/memory_map.h"
#include "../include/touchscreen.h"
#include "../include/gps.h"
#include "../include/wifi.h"
#include "../include/graphics.h"
#include "../include/servo.h"
#include "../include/globals.h"

volatile int x = 0x0;
volatile int i = 0x01;


Graphics graphics;
Servo servo(0x80002030);
Wifi wifi(0x84000220);
GPS gps(0x84000210);

void isr_check_weather(void *context, alt_u32 id){
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0); //Clear IRQ status

	//Currently increments LEDs. Once wifi is connected this will be changed
	x = x + i;
	IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE, x);
	//wifi.send_message_nowait("check_weather()\r\n");
}



int main(void)
{

    TouchScreen touchScreen(0x84000230);

    // home page
    graphics.FrontPanel();

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
    }

    //wifi.test_wifi();
    //while(true)
    // {
    // 	if(gps.scanGPSData())
    // 	{
    // 		gps.getLat();
    // 		gps.getLong();
    //         printf("Time: %d:%d\n", (gps.getHours() - 8) % 24, gps.getMinutes());
    // 	}
    // }
}
