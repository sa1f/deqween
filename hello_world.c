/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>
#include "system.h"
#include "altera_avalon_timer_regs.h"
#include "alt_types.h"
#include "sys/alt_irq.h"
#include "sys/alt_stdio.h"

void isr_check_weather(void *context, alt_u32 id){
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0);
	alt_putstr("ISR Test\n");
}

int main()
{
  alt_putstr("Hello from Nios II!\n");

  //Initialize Inteveral Timer
  IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, ALTERA_AVALON_TIMER_CONTROL_CONT_MSK
                                                   | ALTERA_AVALON_TIMER_CONTROL_START_MSK
                                                   | ALTERA_AVALON_TIMER_CONTROL_ITO_MSK );


  // Register the ISR using Timer_0
  alt_irq_register(TIMER_0_IRQ, 0, isr_check_weather);

  // Start timer
  IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, 0x0007);

  /* Event loop never exits. */
  while (1)
  {
  }

  return 0;
}
