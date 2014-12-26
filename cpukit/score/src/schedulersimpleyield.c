/**
 * @file
 *
 * @brief Simple Schedule Yield CPU
 * @ingroup ScoreScheduler
 */

/*
 *  COPYRIGHT (c) 2011.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/score/schedulersimpleimpl.h>
#include <rtems/score/isr.h>
#include <rtems/score/threadimpl.h>

void _Scheduler_simple_Yield(
  Scheduler_Control *scheduler,
  Thread_Control    *the_thread
)
{
  ISR_Level       level;

  _ISR_Disable( level );

    _Scheduler_simple_Ready_queue_requeue( scheduler, the_thread );

    _ISR_Flash( level );

    _Scheduler_simple_Schedule_body( scheduler, the_thread, false );

  _ISR_Enable( level );
}
