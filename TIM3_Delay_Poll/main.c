/**********************
  Delays via TIM3 and polling (STM8S Discovery Board)

  Blink LED on STM8S Discovery board (see UM0817 on www.st.com)
  or muBoard using TIM3 with polling flag
  
  Functionality:
   - change CPU clock prescaler
   - configure GPIO as output
   - configure TIM3 for delays with polling
   - blink LED output pin every 500ms
  
**********************/

/*----------------------------------------------------------
    INCLUDE FILES
----------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>
#define _MAIN_
  #include "stm8as.h"
  #include "gpio.h"
  #include "timer3.h"
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
  
  // init timer TIM3 for for sleep_x()
  tim3_init();
  
  // configure LED pin
  #ifdef STM8S105     // STM8S Discovery Board (PD0)
    gpio_init(&PORT_D, PIN_0, OUTPUT_PUSHPULL_FAST);
  #else               // muBoard (PH3)
    gpio_init(&PORT_H, PIN_3, OUTPUT_PUSHPULL_FAST);
  #endif
  
  // enable interrupts
  ENABLE_INTERRUPTS;
    

  /////////////////
  //	main loop
  /////////////////
  while (1) {
    
    // toggle LED on STM8S Discovery Board (PD0)
    #ifdef STM8S105
      GPIO_TOGGLE(PORT_D, PIN_0);
      //PORT_D.ODR.bit.b0 ^= 1;
      
    // toggle LED on muBoard (PH3)
    #else
      //GPIO_TOGGLE(PORT_H, PIN_3);
      PORT_H.ODR.bit.b3 ^= 1;
    #endif

    // wait 500ms (blocking)
    sleep_ms(500);
    
  } // main loop

} // main

/*-----------------------------------------------------------------------------
    END OF MODULE
-----------------------------------------------------------------------------*/
