/**********************
  External interrupts (muBoard)
  
  count number of external interrupts on PE5 and
  send via UART1
    
  Functionality:
   - change CPU clock prescaler
   - configure PE5 for interrupt (count falling edges)
   - initialize UART1 for 115.2kBaud
   - configure TIM4 as 1ms clock with interrupt
   - send events via printf(), see putchar.c
   - periodically blink LED
 
**********************/

/*----------------------------------------------------------
    INCLUDE FILES
----------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#define _MAIN_
  #include "stm8as.h"
  #include "gpio.h"
  #include "exint.h"
  #include "timer4.h"
  #include "uart1.h"
#undef _MAIN_


/////////////////
//	main routine
/////////////////
void main (void) {
  

  /////////////////
  //	init peripherals
  /////////////////
  
  // disable interrupts
  DISABLE_INTERRUPTS;

  // switch to 16MHz (default is 2MHz)
  CLK.CKDIVR.byte = 0x00;  
  
  // configure green LED pin (PH2)
  gpio_init(&PORT_H, PIN_2, OUTPUT_PUSHPULL_FAST);

  // configure pin PE5 for interrupts on falling edge (is per port)
  exint_init(&PORT_E, EXINT_FALL_EDGE);
  gpio_init(&PORT_E, PIN_5, INPUT_PULLUP_EXINT);

  // init timer TIM4 for 1ms clock with interrupts
  tim4_init();
  
  // init UART1 (connected to PC on muBoard)
  uart1_init(115200L);
  
  // enable interrupts
  ENABLE_INTERRUPTS;
    

  /////////////////
  //	main loop
  /////////////////
  while (1) {
    
    // every 1ms do
    if (g_flagClock) {
      g_flagClock = 0;
    
      // every 500ms toggle LED & print text
      if (g_clock > 500) {
        g_clock = 0;
        
        // toggle LED
        PORT_H.ODR.bit.b2 ^= 1;
        
        // print number of events to UART1 (blocking)
        printf("events: %d\n", (int) (g_countExintE));
      
      } // loop 500ms
      
    } // loop 1ms
    
  } // main loop

} // main

/*-----------------------------------------------------------------------------
    END OF MODULE
-----------------------------------------------------------------------------*/
