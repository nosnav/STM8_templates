/**
  \file putchar.c
   
  \author G. Icking-Konert
  \date 2015-04-09
  \version 0.1
   
  \brief implementation of required putchar() function
   
  declaration of putchar() function required for stdio.h
  functions, e.g. printf().
*/

/*-----------------------------------------------------------------------------
    INCLUDE FILES
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "uart1.h"


/*----------------------------------------------------------
    FUNCTIONS
----------------------------------------------------------*/

/**
  \fn void putchar(char byte)
   
  \brief send byte via UART1
  
  \param[in]  byte   data to send
  
  \return  always zero (SDCC only)

  implementation of putchar() for printf(), using selected output channel
  Return type depends on used compiler (see respective stdio.h)
*/
#if defined(__CSMC__)
  char putchar(char byte) {
#elif defined(__SDCC)
  void putchar(char byte) {
#else
  int putchar(char byte) {
#endif
  
  // use USART1 send routine
  if (byte=='\n') {
    uart1_send(13);
    uart1_send(10);
  }
  else
    uart1_send(byte);

  // avoid warning message
#if !defined(__SDCC)
  return(0);
#endif

} // putchar


/*-----------------------------------------------------------------------------
    END OF MODULE
-----------------------------------------------------------------------------*/
