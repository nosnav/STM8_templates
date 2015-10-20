/**********************
  Beeper control (muBoard)
  
  Periodically operate beeper attached to pin PD4.
  Used speaker: Kingstate KPEG-167 (Farnell 1193665)
  
  Functionality:
   - change CPU clock prescaler
   - assert option byte for alternate beeper function. If required trigger SW reset
   - configure TIM3 for sleep_ms()
   - beep periodically
 
**********************/

/*----------------------------------------------------------
    INCLUDE FILES
----------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>
#define _MAIN_
  #include "stm8as.h"
  #include "gpio.h"
  #include "flash.h"
  #include "beeper.h"
  #include "timer3.h"
#undef _MAIN_


/////////////////
//	main routine
/////////////////
void main (void) {
  
  uint8_t   val;    // misc variable

  /////////////////
  //	init peripherals
  /////////////////
  
  // disable interrupts
  DISABLE_INTERRUPTS;

  // copy flash block w/e routines to RAM (if required)
  #if defined(FLASH_BLOCK_OPS) && defined(__CSMC__)
    _fctcpy('R');
  #endif
  
  // switch to 16MHz (default is 2MHz)
  CLK.CKDIVR.byte = 0x00;  
  
  // set option byte for PD4 as beeper output. On modification trigger SW reset 
  val = 0;
  val += flash_write_option_byte(OPT2,   0x80);   // option byte
  val += flash_write_option_byte(NOPT2,  0x7F);   // complementary option byte
  if (val)  WWDG.CR.byte = 0xBF;                  // on change trigger reset
  
  // init timer TIM3 for sleep_x()
  tim3_init();

  // configure beeper pin as output (PD4)
  gpio_init(&PORT_D, PIN_4, OUTPUT_PUSHPULL_FAST);
  
  // enable interrupts
  ENABLE_INTERRUPTS;
    
    
  /////////////////
  //	main loop
  /////////////////
  val = 1;
  while (1) {
    
    // beeper control
    beep(val, 8);     // short bleep
    beep(0, 1000);    // 1s pause
    
    // change frequency [kHz] (supported: 1,2,4)
    if ((val*=2) >4) val=1;
    
  } // main loop

} // main

/*-----------------------------------------------------------------------------
    END OF MODULE
-----------------------------------------------------------------------------*/
