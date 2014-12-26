/**
 * @file
 * @ingroup arm_edb7312
 * @brief Global BSP definitions.
 */

/*
 * Cirrus EP7312 BSP header file
 *
 * Copyright (c) 2002 by Jay Monkman <jtm@smoothsmoothie.com>
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
*/
#ifndef _BSP_H
#define _BSP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bspopts.h>
#include <bsp/default-initial-extension.h>

#include <rtems.h>
#include <rtems/iosupp.h>
#include <rtems/console.h>
#include <rtems/clockdrv.h>

#define BSP_FEATURE_IRQ_EXTENSION

/**
 * @defgroup arm_edb7312 EDB7312 Support
 * @ingroup bsp_arm
 * @brief EDB7312 Support Package
 * @{
 */

/**
 * @brief Define the interrupt mechanism for Time Test 27
 *
 * NOTE: Following are not defined and are board independent
 *
 */
struct rtems_bsdnet_ifconfig;
int cs8900_driver_attach (struct rtems_bsdnet_ifconfig *config,
                          int                          attaching);

/**
 * @name Network driver configuration
 * @{
 */

#define RTEMS_BSP_NETWORK_DRIVER_NAME	"eth0"
#define RTEMS_BSP_NETWORK_DRIVER_ATTACH	cs8900_driver_attach

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _BSP_H */
