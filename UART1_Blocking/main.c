/**********************
  UART1 output (muBoard)
  
  send/receive via UART1 using polling and blocking send
  (i.e. no background communication)
  
  Functionality:
   - change CPU clock prescaler
   - initialize UART1 for 115.2kBaud (no SW interrupts)
   - reset on command 'Re5eT!' received (see uart1.c and Makefile)
   - configure TIM4 as 1ms clock with interrupt
   - echo received bytes via UART1
   - periodically send text via printf(), see putchar.c
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
  #include "timer4.h"
  #include "uart1.h"
#undef _MAIN_


/////////////////
//	main routine
/////////////////
void main (void) {
  
  uint8_t   Rx, loop;
  
  
  /////////////////
  //	init peripherals
  /////////////////

  // disable interrupts
  DISABLE_INTERRUPTS;

  // switch to 16MHz (default is 2MHz)
  CLK.CKDIVR.byte = 0x00;  
  
  // configure green LED pin (PH2)
  gpio_init(&PORT_H, PIN_2, OUTPUT_PUSHPULL_FAST);

  // init timer TIM4 for 1ms clock with interrupts
  tim4_init();
  
  // init UART1 (connected to PC on muBoard)
  uart1_init(115200L);
  
  // enable interrupts
  ENABLE_INTERRUPTS;

    
  /////////////////
  //	main loop
  /////////////////
  loop = 0;
  while (1) {
    
    // if byte received via UART1, echo byte+1
    if (uart1_check_Rx()) {
      Rx = uart1_receive();        // grab oldest byte
      putchar((char) (Rx+1));
    }
    
    // every 1ms do
    if (g_flagClock) {
      g_flagClock = 0;
    
      // every 500ms toggle LED & print text
      if (g_clock > 500) {
        g_clock = 0;
        
        // toggle LED
        PORT_H.ODR.bit.b2 ^= 1;
        
        // print something to UART (blocking)
        printf("loop: %d\n", (int) (++loop));
      
      } // loop 500ms
      
    } // loop 1ms
    
  } // main loop

} // main

/*-----------------------------------------------------------------------------
    END OF MODULE
-----------------------------------------------------------------------------*/
