/**********************
  LCD output via I2C (muBoard)
  
  Periodically print text to 2x16 char LCD attached to I2C
  Used display: Batron BTHQ21605V-COG-FSRE-I2C 2X16 (Farnell 1220409)
  connected via I2C pins PE1/SCL, PE2/SDA, and GPIO PE3 for LCD reset (see i2c_lcd.c)
  
  Functionality:
   - change CPU clock prescaler
   - configure TIM3 for sleep_ms()
   - initialize I2C bus
   - initialize and reset LCD display
   - periodically print to LCD 
 
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
  #include "i2c.h"
  #include "i2c_lcd.h"
  #include "timer3.h"
#undef _MAIN_


/////////////////
//	main routine
/////////////////
void main (void) {
  
  uint16_t    val=0;      // misc variable
  char        str[20];    // string buffer for LCD output

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
  
  // init and reset LCD display
  lcd_init();
    

  // print to LCD
  sprintf(str, "hello user...");
  lcd_print(1, 1, str);
  
  // enable interrupts
  ENABLE_INTERRUPTS;
  
    
  /////////////////
  //	main loop
  /////////////////
  while (1) {
    
    // print to LCD
    sprintf(str, "loop %d", (int) val++);
    lcd_print(2, 1, str);
      
    // wait 500ms
    sleep_ms(100);

  } // main loop

} // main

/*-----------------------------------------------------------------------------
    END OF MODULE
-----------------------------------------------------------------------------*/
