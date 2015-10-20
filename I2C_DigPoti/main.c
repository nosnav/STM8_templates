/**********************
  control of digital potentiometer via I2C (muBoard)
  
  Sweep resistance of digital I2C potentiometer.
  Used potentiometer: 20kR/8-bit AD5280BRUZ20 (Farnell 1438441)
  connected via I2C pins PE1/SCL, PE2/SDA
  
  Functionality:
   - change CPU clock prescaler
   - configure TIM3 for sleep_ms()
   - initialize I2C bus
   - sweep resistance of digital resistor in ~10s
 
**********************/

/*----------------------------------------------------------
    INCLUDE FILES
----------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#define _MAIN_
  #include "stm8as.h"
  #include "i2c.h"
  #include "i2c_poti.h"
  #include "timer3.h"
#undef _MAIN_


/////////////////
//	main routine
/////////////////
void main (void) {
  
  uint16_t     res=0;


  /////////////////
  //	init peripherals
  /////////////////
  
  // disable interrupts
  DISABLE_INTERRUPTS;

  // switch to 16MHz (default is 2MHz)
  CLK.CKDIVR.byte = 0x00;  

  // init timer TIM3 for sleep_x()
  tim3_init();
  
  // init I2C bus for LCD
  i2c_init();
    
  // enable interrupts
  ENABLE_INTERRUPTS;
  
    
  /////////////////
  //	main loop
  /////////////////
  while (1) {
    
    // loop resistance
    for (res=0; res<256; res++) {
      
      // set resistance of potentiometer
      set_dig_poti((uint8_t) res);
      
      // wait a bit
      sleep_ms(50);

    } // loop res

  } // main loop

} // main

/*-----------------------------------------------------------------------------
    END OF MODULE
-----------------------------------------------------------------------------*/
