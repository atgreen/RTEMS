/**
 * @file
 *
 * @brief Changes the Priority of a Thread
 *
 * @ingroup ScoreThread
 */

/*
 *  COPYRIGHT (c) 1989-2011.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/score/threadimpl.h>
#include <rtems/score/schedulerimpl.h>
#include <rtems/score/threadqimpl.h>

void _Thread_Change_priority(
  Thread_Control   *the_thread,
  Priority_Control  new_priority,
  bool              prepend_it
)
{
  /*
   *  Do not bother recomputing all the priority related information if
   *  we are not REALLY changing priority.
   */
  if ( the_thread->current_priority != new_priority ) {
    ISR_Level                level;
    const Scheduler_Control *scheduler;

    _ISR_Disable( level );

    scheduler = _Scheduler_Get( the_thread );
    the_thread->current_priority = new_priority;

    if ( _States_Is_ready( the_thread->current_state ) ) {
      _Scheduler_Change_priority(
        scheduler,
        the_thread,
        new_priority,
        prepend_it
      );

      _ISR_Flash( level );

      /*
       *  We altered the set of thread priorities.  So let's figure out
       *  who is the heir and if we need to switch to them.
       */
      scheduler = _Scheduler_Get( the_thread );
      _Scheduler_Schedule( scheduler, the_thread );
    } else {
      _Scheduler_Update( scheduler, the_thread );
    }
    _ISR_Enable( level );

    _Thread_queue_Requeue( the_thread->Wait.queue, the_thread );
  }
}
