/**
 * @file
 *
 * @ingroup lpc24xx
 *
 * @brief Console configuration.
 */

/*
 * Copyright (c) 2008-2011 embedded brains GmbH.  All rights reserved.
 *
 *  embedded brains GmbH
 *  Obere Lagerstr. 30
 *  82178 Puchheim
 *  Germany
 *  <rtems@embedded-brains.de>
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#include <libchip/serial.h>
#include <libchip/ns16550.h>

#include <bsp.h>
#include <bsp/lpc24xx.h>
#include <bsp/irq.h>
#include <bsp/io.h>

static uint8_t lpc24xx_uart_get_register(uintptr_t addr, uint8_t i)
{
  volatile uint32_t *reg = (volatile uint32_t *) addr;

  return (uint8_t) reg [i];
}

static void lpc24xx_uart_set_register(uintptr_t addr, uint8_t i, uint8_t val)
{
  volatile uint32_t *reg = (volatile uint32_t *) addr;

  reg [i] = val;
}

console_tbl Console_Configuration_Ports [] = {
  #ifdef LPC24XX_CONFIG_CONSOLE
    {
      .sDeviceName = "/dev/ttyS0",
      .deviceType = SERIAL_NS16550_WITH_FDR,
      .pDeviceFns = &ns16550_fns,
      .deviceProbe = NULL,
      .pDeviceFlow = NULL,
      .ulMargin = 16,
      .ulHysteresis = 8,
      .pDeviceParams = (void *) LPC24XX_UART_BAUD,
      .ulCtrlPort1 = UART0_BASE_ADDR,
      .ulCtrlPort2 = 0,
      .ulDataPort = UART0_BASE_ADDR,
      .getRegister = lpc24xx_uart_get_register,
      .setRegister = lpc24xx_uart_set_register,
      .getData = NULL,
      .setData = NULL,
      .ulClock = LPC24XX_PCLK,
      .ulIntVector = LPC24XX_IRQ_UART_0
    },
  #endif
  #ifdef LPC24XX_CONFIG_UART_1
    {
      .sDeviceName = "/dev/ttyS1",
      .deviceType = SERIAL_NS16550_WITH_FDR,
      .pDeviceFns = &ns16550_fns,
      .deviceProbe = lpc24xx_uart_probe_1,
      .pDeviceFlow = NULL,
      .ulMargin = 16,
      .ulHysteresis = 8,
      .pDeviceParams = (void *) LPC24XX_UART_BAUD,
      .ulCtrlPort1 = UART1_BASE_ADDR,
      .ulCtrlPort2 = 0,
      .ulDataPort = UART1_BASE_ADDR,
      .getRegister = lpc24xx_uart_get_register,
      .setRegister = lpc24xx_uart_set_register,
      .getData = NULL,
      .setData = NULL,
      .ulClock = LPC24XX_PCLK,
      .ulIntVector = LPC24XX_IRQ_UART_1
    },
  #endif
  #ifdef LPC24XX_CONFIG_UART_2
    {
      .sDeviceName = "/dev/ttyS2",
      .deviceType = SERIAL_NS16550_WITH_FDR,
      .pDeviceFns = &ns16550_fns,
      .deviceProbe = lpc24xx_uart_probe_2,
      .pDeviceFlow = NULL,
      .ulMargin = 16,
      .ulHysteresis = 8,
      .pDeviceParams = (void *) LPC24XX_UART_BAUD,
      .ulCtrlPort1 = UART2_BASE_ADDR,
      .ulCtrlPort2 = 0,
      .ulDataPort = UART2_BASE_ADDR,
      .getRegister = lpc24xx_uart_get_register,
      .setRegister = lpc24xx_uart_set_register,
      .getData = NULL,
      .setData = NULL,
      .ulClock = LPC24XX_PCLK,
      .ulIntVector = LPC24XX_IRQ_UART_2
    },
  #endif
  #ifdef LPC24XX_CONFIG_UART_3
    {
      .sDeviceName = "/dev/ttyS3",
      .deviceType = SERIAL_NS16550_WITH_FDR,
      .pDeviceFns = &ns16550_fns,
      .deviceProbe = lpc24xx_uart_probe_3,
      .pDeviceFlow = NULL,
      .ulMargin = 16,
      .ulHysteresis = 8,
      .pDeviceParams = (void *) LPC24XX_UART_BAUD,
      .ulCtrlPort1 = UART3_BASE_ADDR,
      .ulCtrlPort2 = 0,
      .ulDataPort = UART3_BASE_ADDR,
      .getRegister = lpc24xx_uart_get_register,
      .setRegister = lpc24xx_uart_set_register,
      .getData = NULL,
      .setData = NULL,
      .ulClock = LPC24XX_PCLK,
      .ulIntVector = LPC24XX_IRQ_UART_3
    },
  #endif
};

#define LPC24XX_UART_COUNT \
  (sizeof(Console_Configuration_Ports) \
    / sizeof(Console_Configuration_Ports [0]))
unsigned long Console_Configuration_Count = LPC24XX_UART_COUNT;
