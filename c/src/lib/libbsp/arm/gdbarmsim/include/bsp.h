/**
 * @file
 *
 * @ingroup arm_gdbarmsim
 *
 * @brief Global BSP definitions.
 */

/*
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
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

/**
 * @defgroup arm_gdbarmsim GDBARMSIM Support
 *
 * @ingroup bsp_arm
 *
 * @brief GDBARMSIM support package.
 *
 * @{
 */

/**
 * @brief Support for simulated clock tick
 */
Thread clock_driver_sim_idle_body(uintptr_t);
#define BSP_IDLE_TASK_BODY clock_driver_sim_idle_body

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _BSP_H */

