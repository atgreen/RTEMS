/**
 * @file
 *
 * @ingroup lpc24xx
 *
 * @brief UART 1 probe.
 */

/*
 * Copyright (c) 2011-2013 embedded brains GmbH.  All rights reserved.
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

#include <bsp.h>
#include <bsp/io.h>

bool lpc24xx_uart_probe_1(int minor)
{
  static const lpc24xx_pin_range pins [] = {
    LPC24XX_PIN_UART_1_TXD_P0_15,
    LPC24XX_PIN_UART_1_RXD_P0_16,
    LPC24XX_PIN_TERMINAL
  };

  lpc24xx_module_enable(LPC24XX_MODULE_UART_1, LPC24XX_MODULE_PCLK_DEFAULT);
  lpc24xx_pin_config(&pins [0], LPC24XX_PIN_SET_FUNCTION);

  return true;
}
