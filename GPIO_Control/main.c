/**********************
  Simple example for controlling GPIO pins.
  
  Control LED via an input pin.
  
  STM8S Discovery board (see UM0817 on www.st.com): via jumper between CN3/pins2+4
  muBoard: via button for io1
  
  Functionality:
   - configure GPIOs as input & output
   - mirror status of input pin to output pin (connected to LED)
  
**********************/

/*----------------------------------------------------------
    INCLUDE FILES
----------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>
#define _MAIN_
  #include "stm8as.h"
  #include "gpio.h"
#undef _MAIN_


/////////////////
//	main routine
/////////////////
void main (void) {
  
  uint8_t   state;
  

  /////////////////
  //	init peripherals
  /////////////////
  
  // disable interrupts
  DISABLE_INTERRUPTS;
  
  #ifdef STM8S105     // STM8S Discovery Board
  
    // configure input pin PB6 (=CN3/pin4)
    gpio_init(&PORT_B, PIN_6, INPUT_PULLUP_NOEXINT);
    
    // configure output pin PD0 (=LED)
    gpio_init(&PORT_D, PIN_0, OUTPUT_PUSHPULL_FAST);

  #else               // muBoard
  
    // configure input pin PE5 (=button)
    gpio_init(&PORT_E, PIN_5, INPUT_PULLUP_NOEXINT);
    
    // configure output pin PH3 (=LED)
    gpio_init(&PORT_H, PIN_3, OUTPUT_PUSHPULL_FAST);

  #endif
  
  // enable interrupts
  ENABLE_INTERRUPTS;
    

  /////////////////
  //	main loop
  /////////////////
  while (1) {
    
    
    #ifdef STM8S105     // STM8S Discovery Board
    
      // read input pin PB6 (=CN3/pin4)
      state = GPIO_READ(PORT_B, PIN_6);

      // set output pin PD0 (=LED)
      GPIO_SET(PORT_D, PIN_0, state);
    
      // alternatively use direct register access
      //PD.ODR.bit.b0 = PB.IDR.bit.b6;

    #else               // muBoard
    
      // read input pin PE5 (=button)
      state = GPIO_READ(PORT_E, PIN_5);

      // set output pin PH3 (=LED)
      GPIO_SET(PORT_H, PIN_3, state);
    
      // alternatively use direct register access
      //PH.ODR.bit.b3 = PE.IDR.bit.b5;

    #endif
        
  } // main loop

} // main

/*-----------------------------------------------------------------------------
    END OF MODULE
-----------------------------------------------------------------------------*/
