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
  #include "timer3.h"
  #include "adc2.h"
  #include "i2c.h"
  #include "lcd.h"
#undef _MAIN_

// ADC channel to measure (=PB5)
#define ADC_POTI      5


/////////////////
//	main routine
/////////////////
void main (void) {
  
  uint16_t    val=0;      // ADC variable [INC]
  uint32_t    centHz=0;   // PWM frequency [0.01Hz]
  uint16_t    deciPrc=0;  // PWM duty cycle [0.1%]
  char        str[20];    // string buffer for LCD output

  
  /////////////////
  //	init peripherals
  /////////////////
  
  // disable interrupts
  DISABLE_INTERRUPTS;

  // switch to 16MHz (default is 2MHz)
  CLK.CKDIVR.byte = 0x00;  
  
  // init timer TIM1 for PWM
  tim1_init();
  
  // init timer TIM3 for sleep_x()
  tim3_init();
  
  // configure LED pin (PD0)
  gpio_init(&PORT_D, PIN_0, OUTPUT_PUSHPULL_FAST);
  
  // configure ADC poti input (PB5)
  gpio_init(&PORT_B, PIN_5, INPUT_FLOAT_NOEXINT);

  // configure input for io_1 button (PE5)
  gpio_init(&PORT_E, PIN_5, INPUT_PULLUP_NOEXINT);

  // init ADC2 for single shot mode
  adc2_init();
  
  // init I2C bus for LCD
  i2c_init();
  
  // init and reset LCD display
  lcd_init();

  // enable interrupts
  ENABLE_INTERRUPTS;
    

  // initialize to 1kHz/50% PWM
  centHz  = 100000L;  // [0.01Hz]
  deciPrc = 5000;     // [0.1%]
    
  
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
      deciPrc = MIN(val,1000);                 // 0..1023 [INC] -> 0-1000 [0.1%] (just clip)
    else
      centHz = (uint32_t) val * 1000L;       // 0..1023 [INC] -> 0- 1.023e6 [0.01Hz] (~100kHz)
    
    // update PWM setting
    tim1_set_pwm(centHz, deciPrc);
    
    // print to LCD
    sprintf(str, "freq %ld Hz", (centHz/100L));
    lcd_print(1, 1, str);
    sprintf(str, "duty %d %c", (int) (deciPrc/10), 37);
    lcd_print(2, 1, str);
     
    // wait a bit
    sleep_ms(100);

  } // main loop

} // main

/*-----------------------------------------------------------------------------
    END OF MODULE
-----------------------------------------------------------------------------*/
