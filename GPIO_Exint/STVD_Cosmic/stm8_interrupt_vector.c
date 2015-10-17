/**
  \file stm8_interrupt_vector.c
   
  \author G. Icking-Konert
  \date 2013-11-22
  \version 0.1
   
  \brief declaration if interrupt vector table (Cosmic compiler only)
   
  declaration if interrupt vector table for Cosmic compiler. If not required, replace below ISRs 
  with NonHandledInterrupt (=dummy) service routine. For SDCC just skip the implementation of 
  not required ISR.
*/

// to avoid compiler warning in SDCC
#include <stdlib.h>

// only for COSMIC compiler, else skip
#if defined(__CSMC__)

// include headers for interrupts
#include "timer4.h"
#include "exint.h"

// declaration of below ISR handler
typedef void @far (*interrupt_handler_t)(void);
struct interrupt_vector {
	unsigned char interrupt_instruction;
	interrupt_handler_t interrupt_handler;
};

// dummy ISR --> do nothing
@far @interrupt void NonHandledInterrupt (void){
	return;
}

// startup routine
extern void _stext();

struct interrupt_vector const _vectab[] = {
  {0x82, (interrupt_handler_t) _stext},           /* reset */
  {0x82, NonHandledInterrupt},                    /* trap - Software interrupt  */
  {0x82, NonHandledInterrupt},                    /* irq0 - External Top Level interrupt (TLI)  */
  {0x82, NonHandledInterrupt},                    /* irq1 - Auto Wake Up from Halt interrupt  */
  {0x82, NonHandledInterrupt},                    /* irq2 - Clock Controller interrupt  */
  {0x82, (interrupt_handler_t) EXINT_PA_ISR},     /* irq3 - External interrupt 0 (GPIOA) */
  {0x82, (interrupt_handler_t) EXINT_PB_ISR},     /* irq4 - External interrupt 1 (GPIOB) */
  {0x82, (interrupt_handler_t) EXINT_PC_ISR},     /* irq5 - External interrupt 2 (GPIOC) */
  {0x82, (interrupt_handler_t) EXINT_PD_ISR},     /* irq6 - External interrupt 3 (GPIOD) */
  {0x82, (interrupt_handler_t) EXINT_PE_ISR},     /* irq7 - External interrupt 4 (GPIOE) */
  {0x82, NonHandledInterrupt},                    /* irq8 - External interrupt 5 (GPIOF) */
  {0x82, NonHandledInterrupt},                    /* irq9 - Reserved */
  {0x82, NonHandledInterrupt},                    /* irq10 - SPI End of transfer interrupt */
  {0x82, NonHandledInterrupt},                    /* irq11 - TIM1 Update/Overflow/Trigger/Break interrupt */
  {0x82, NonHandledInterrupt},                    /* irq12 - TIM1 Capture/Compare interrupt */
  {0x82, NonHandledInterrupt},                    /* irq13 - TIM2 Update/Overflow/Break interrupt */
  {0x82, NonHandledInterrupt},                    /* irq14 - TIM2 Capture/Compare interrupt */
  {0x82, NonHandledInterrupt},                    /* irq15 - Reserved */
  {0x82, NonHandledInterrupt},                    /* irq16 - Reserved */
  {0x82, NonHandledInterrupt},                    /* irq17 - UART1(=LINUART) Tx complete interrupt */
  {0x82, NonHandledInterrupt},                    /* irq18 - UART1(=LINUART) Rx interrupt */
  {0x82, NonHandledInterrupt},                    /* irq19 - I2C interrupt */
  {0x82, NonHandledInterrupt},                    /* irq20 - UART2 Tx interrupt */
  {0x82, NonHandledInterrupt},                    /* irq21 - UART2 Rx interrupt */
  {0x82, NonHandledInterrupt},                    /* irq22 - ADC1 end of conversion/Analog watchdog interrupts */
  {0x82, (interrupt_handler_t) tim4_ISR},         /* irq23 - Timer 4 interrupt (SW master clock) */
  {0x82, NonHandledInterrupt},                    /* irq24 - FLASH interrupt */
  {0x82, NonHandledInterrupt},                    /* irq25 - Reserved */
  {0x82, NonHandledInterrupt},                    /* irq26 - Reserved */
  {0x82, NonHandledInterrupt},                    /* irq27 - Reserved */
  {0x82, NonHandledInterrupt},                    /* irq28 - Reserved */
  {0x82, NonHandledInterrupt},                    /* irq29 - Reserved */
};

// end only for Cosmic
#endif // __CSMC__

/*-----------------------------------------------------------------------------
    END OF MODULE 
-----------------------------------------------------------------------------*/
