/**********************
  generate PWM via TIM1_CC1 (=PC1) pin (muBoard)

  Generate PWM signal via TIM1 on GPIO pin.
  Control frequency and duty cycle via ADC
  
  Functionality:
   - change CPU clock prescaler
   - configure GPIO as output (PWM + LED) and input (ADC)
   - measure ADC to set freq/DC. Selection via io_1 button
   - print freq & duty cycle on LCD
   - blink LED output pin every 500ms
  
**********************/

/*----------------------------------------------------------
    INCLUDE FILES
----------------------------------------------------------*/
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#define _MAIN_
  #include "stm8as.h"
  #include "misc.h"
  #include "gpio.h"
  #include "timer1.h"
  #include "timer2.h"
  #include "adc2.h"
  #include "timer3.h"
  #include "uart1.h"
#undef _MAIN_

// ADC channel to measure (=PB5)
#define ADC_POTI      5


/////////////////
//	main routine
/////////////////
void main (void) {
  
  uint16_t    val=0;        // ADC variable [INC]
  uint32_t    centHz_1=0;   // generated PWM frequency [0.01Hz]
  uint16_t    deciPrc_1=0;  // generated PWM duty cycle [0.1%]
  uint32_t    centHz_2=0;   // measured PWM frequency [0.01Hz]
  uint16_t    deciPrc_2=0;  // measured PWM duty cycle [0.1%]

  
  /////////////////
  //	init peripherals
  /////////////////
  
  // disable interrupts
  DISABLE_INTERRUPTS;

  // switch to 16MHz (default is 2MHz)
  CLK.CKDIVR.byte = 0x00;  
  
  // configure LED pin (PD0)
  gpio_init(&PORT_D, PIN_0, OUTPUT_PUSHPULL_FAST);
  
  // configure ADC poti input (PB5)
  gpio_init(&PORT_B, PIN_5, INPUT_FLOAT_NOEXINT);

  // configure input for io_1 button (PE5)
  gpio_init(&PORT_E, PIN_5, INPUT_PULLUP_NOEXINT);

  // init ADC2 for single shot mode
  adc2_init();
  
  // init timer TIM1 for PWM
  tim1_init();
  
  // init timer TIM2 for PWM
  tim2_init();
  
  // init timer TIM3 for sleep_ms()
  tim3_init();
  
  // init UART1 (connected to PC on muBoard)
  uart1_init(115200L);

  // enable interrupts
  ENABLE_INTERRUPTS;
    

  // initialize generator to 1kHz/50% PWM
  centHz_1  = 100000L;  // [0.01Hz]
  deciPrc_1 = 5000;     // [0.1%]
    
  
  /////////////////
  //	main loop
  /////////////////
  while (1) {
     
    // toggle output pin PD0 (=LED)
    GPIO_TOGGLE(PORT_D, PIN_0);
    
    // measure ADC voltage on pin 
    val = adc2_measure(ADC_POTI);
    
    // depending on io_1 button (=PE5) set duty cycle (default) or frequency (pressed)
    if (GPIO_READ(PORT_E,PIN_5))
      deciPrc_1 = MIN(val,1000);                 // 0..1023 [INC] -> 0-1000 [0.1%] (just clip)
    else
      centHz_1 = (uint32_t) val * 1000L;       // 0..1023 [INC] -> 0- 1.023e6 [0.01Hz] (~100kHz)
    
    // update PWM generation via TIM1
    tim1_set_pwm(centHz_1, deciPrc_1);
    
    // measure PWM signal via TIM2
    tim2_get_pwm(&centHz_2, &deciPrc_2);
    
    // print to UART1
    printf("out: %ldHz/%d%c; ", (centHz_1/100L), (int) (deciPrc_1/10), 37);
    printf("in: %ldHz/%d%c\n",  (centHz_2/100L), (int) (deciPrc_2/10), 37);
    
    // wait a bit
    sleep_ms(500);

  } // main loop

} // main

/*-----------------------------------------------------------------------------
    END OF MODULE
-----------------------------------------------------------------------------*/
