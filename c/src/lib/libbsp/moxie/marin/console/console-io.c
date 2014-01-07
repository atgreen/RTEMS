/*
 *  This file contains the hardware specific portions of the TTY driver
 *  for the Moxie Marin board.
 *
 *  COPYRIGHT (c) 2011.
 *  Anthony Green.
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 */

#include <bsp.h>
#include <rtems/libio.h>
#include <stdlib.h>
#include <assert.h>

/*
 *  console_initialize_hardware
 *
 *  This routine initializes the console hardware.
 *
 */

void console_initialize_hardware(void)
{
  return;
}

/*
 *  console_outbyte_polled
 *
 *  This routine transmits a character using polling.
 */
#define MARIN_UART_BASE    0xF0000008
#define MARIN_UART_RXRDY   (MARIN_UART_BASE + 0)
#define MARIN_UART_TXRDY   (MARIN_UART_BASE + 2)
#define MARIN_UART_RXDATA  (MARIN_UART_BASE + 4)
#define MARIN_UART_TXDATA  (MARIN_UART_BASE + 6)

void console_outbyte_polled(
  int  port,
  char ch
)
{
  volatile unsigned short *pUartTxReady = 
    (volatile unsigned short *) MARIN_UART_TXRDY;
  volatile unsigned short *pUartTxData = 
    (volatile unsigned short *) MARIN_UART_TXDATA;


  while (*pUartTxReady != 1) ;

  *pUartTxData = ch;
}

/*
 *  console_inbyte_nonblocking
 *
 *  This routine polls for a character.
 */

int console_inbyte_nonblocking(
  int port
)
{
  volatile unsigned short *pUartRxReady = 
    (volatile unsigned short *) MARIN_UART_RXRDY;
  volatile unsigned short *pUartRxData = 
    (volatile unsigned short *) MARIN_UART_RXDATA;


  if (*pUartRxReady == 1) {
    return *pUartRxData;
  } else {
    return -1;
  }
}

#include <rtems/bspIo.h>

void marin_output_char(char c) { console_outbyte_polled( 0, c ); }
void marin_input_char(char c) { console_inbyte_nonblocking( 0 ); }

BSP_output_char_function_type           BSP_output_char = marin_output_char;
BSP_polling_getchar_function_type       BSP_poll_char = marin_input_char;
