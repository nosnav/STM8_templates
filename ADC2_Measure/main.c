/**********************
  Measure via ADC2 and send result via UART1 (muBoard)

  Functionality:
   - change CPU clock prescaler
   - initialize ADC2 for single shot mode
   - initialize UART1 for 115.2kBaud
   - periodically measure voltage on PB5 and send via UART1
   - periodically blink LED
  
**********************/

/*----------------------------------------------------------
    INCLUDE FILES
----------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#define _MAIN_
  #include "stm8as.h"
  #include "adc2.h"
  #include "gpio.h"
  #include "timer3.h"
  #include "uart1.h"
#undef _MAIN_

// ADC channel to measure (=PB5)
#define ADC_POTI      5


/////////////////
//	main routine
/////////////////
void main (void) {
  
  uint16_t  result;


  /////////////////
  //	init peripherals
  /////////////////
  
  // disable interrupts
  DISABLE_INTERRUPTS;

  // switch to 16MHz (default is 2MHz)
  CLK.CKDIVR.byte = 0x00;  
  
  // init timer TIM3 for for sleep_x()
  tim3_init();
  
  // init UART1 (connected to PC on muBoard)
  uart1_init(115200L);
  
  // configure LED pin (PH3)
  gpio_init(&PORT_H, PIN_3, OUTPUT_PUSHPULL_FAST);
    
  // configure ADC poti input (PB5)
  gpio_init(&PORT_B, PIN_5, INPUT_FLOAT_NOEXINT);

  // init ADC2 for single shot mode
  adc2_init();
  
  // enable interrupts
  ENABLE_INTERRUPTS;
    

  /////////////////
  //	main loop
  /////////////////
  while (1) {
    
    // toggle output pin PD0 (=LED)
    GPIO_TOGGLE(PORT_H, PIN_3);
    //PORT_H.ODR.bit.b3 ^= 1;
    
    // measure ADC voltage on pin 
    result = adc2_measure(ADC_POTI);
    
    // send result via UART1
    printf("ADC2 = %d\n", (int) result);
    
    // wait 500ms (blocking)
    sleep_ms(500);
    
  } // main loop

} // main

/*-----------------------------------------------------------------------------
    END OF MODULE
-----------------------------------------------------------------------------*/
