/**
 * @file
 *
 * @brief Deterministic Priority Affinity SMP Scheduler Implementation
 *
 * @ingroup ScoreSchedulerPriorityAffinitySMP
 */

/*
 *  COPYRIGHT (c) 2014.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
  #include "config.h"
#endif

#include <rtems/score/schedulerpriorityaffinitysmp.h>
#include <rtems/score/schedulerpriorityimpl.h>
#include <rtems/score/schedulersmpimpl.h>
#include <rtems/score/schedulerprioritysmpimpl.h>
#include <rtems/score/wkspace.h>
#include <rtems/score/cpusetimpl.h>

#include <rtems/score/priority.h>

/*
 * The following methods which initially were static in schedulerprioritysmp.c
 * are shared with this scheduler. They are now public so they can be shared.
 *
 *  + _Scheduler_priority_SMP_Get_self
 *  + _Scheduler_priority_SMP_Insert_ready_fifo
 *  + _Scheduler_priority_SMP_Insert_ready_lifo
 *  + _Scheduler_priority_SMP_Node_get
 *  + _Scheduler_priority_SMP_Move_from_scheduled_to_ready
 *  + _Scheduler_priority_SMP_Move_from_ready_to_scheduled
 *  + _Scheduler_priority_SMP_Extract_from_ready
 *  + _Scheduler_priority_SMP_Do_update
 */

static bool _Scheduler_priority_affinity_SMP_Insert_priority_lifo_order(
  const Chain_Node *to_insert,
  const Chain_Node *next
)
{
  const Thread_Control *thread_to_insert = (const Thread_Control *) to_insert;
  const Thread_Control *thread_next = (const Thread_Control *) next;

  return next != NULL
    && thread_to_insert->current_priority <= thread_next->current_priority;
}

static bool _Scheduler_priority_affinity_SMP_Insert_priority_fifo_order(
  const Chain_Node *to_insert,
  const Chain_Node *next
)
{
  const Thread_Control *thread_to_insert = (const Thread_Control *) to_insert;
  const Thread_Control *thread_next = (const Thread_Control *) next;

  return next != NULL
    && thread_to_insert->current_priority < thread_next->current_priority;
}

/*
 * This method returns the scheduler node for the specified thread
 * as a scheduler specific type.
 */
static Scheduler_priority_affinity_SMP_Node *
_Scheduler_priority_affinity_SMP_Node_get(
  Thread_Control *thread
)
{
  return (Scheduler_priority_affinity_SMP_Node *) _Scheduler_Node_get( thread );
}

/*
 * This method initializes the scheduler control information for
 * this scheduler instance.
 */
void _Scheduler_priority_affinity_SMP_Node_initialize(
  const Scheduler_Control *scheduler,
  Thread_Control          *thread
)
{
  Scheduler_SMP_Node *smp_node = _Scheduler_SMP_Node_get( thread );

  Scheduler_priority_affinity_SMP_Node *node =
    _Scheduler_priority_affinity_SMP_Node_get( thread );

  (void) scheduler;

  /*
   *  All we add is affinity information to the basic SMP node.
   */
  _Scheduler_SMP_Node_initialize( smp_node );

  node->Affinity     = *_CPU_set_Default();
  node->Affinity.set = &node->Affinity.preallocated;
}

/*
 * This method is slightly different from _Scheduler_SMP_Allocate_processor()
 * in that it does what it is asked to do. _Scheduler_SMP_Allocate_processor()
 * attempts to prevent migrations but does not take into account affinity
 */
static inline void _Scheduler_SMP_Allocate_processor_exact(
   Scheduler_SMP_Context *self,
   Thread_Control        *scheduled,
   Thread_Control        *victim
)
{
   Scheduler_SMP_Node *scheduled_node = _Scheduler_SMP_Node_get( scheduled );
   Per_CPU_Control    *cpu_of_victim = _Thread_Get_CPU( victim );
   Per_CPU_Control    *cpu_self = _Per_CPU_Get();

   _Scheduler_SMP_Node_change_state(
     scheduled_node,
     SCHEDULER_SMP_NODE_SCHEDULED
   );

   _Thread_Set_CPU( scheduled, cpu_of_victim );
   _Scheduler_SMP_Update_heir( cpu_self, cpu_of_victim, scheduled );
}

/*
 * This method is unique to this scheduler because it takes into
 * account affinity as it determines the highest ready thread.
 * Since this is used to pick a new thread to replace the victim,
 * the highest ready thread must have affinity such that it can
 * be executed on the victim's processor.
 */
static Thread_Control *_Scheduler_priority_affinity_SMP_Get_highest_ready(
  Scheduler_Context *context,
  Thread_Control    *victim
)
{
  Scheduler_priority_SMP_Context *self =
    _Scheduler_priority_SMP_Get_self( context );
  Priority_Control                index;
  Thread_Control                 *highest = NULL;
  int                             victim_cpu;

  /*
   * This is done when we need to check if reevaluations are needed.
   */
  if ( victim == NULL ) {
    return _Scheduler_priority_Ready_queue_first(
        &self->Bit_map,
        &self->Ready[ 0 ]
      );
  }

  victim_cpu = _Per_CPU_Get_index( _Thread_Get_CPU( victim ) );

  /**
   * @todo The deterministic priority scheduler structure is optimized
   * for insertion, extraction, and finding the highest priority
   * thread. Scanning the list of ready threads is not a purpose
   * for which it was optimized. There are optimizations to be
   * made in this loop.
   *
   * + by checking the major bit, we could potentially skip entire
   *   groups of 16.
   *
   * When using this scheduler as implemented, the application's
   * choice of numeric priorities and their distribution can have
   * an impact on performance.
   */
  for ( index = _Priority_bit_map_Get_highest( &self->Bit_map ) ;
        index <= PRIORITY_MAXIMUM;
        index++ )
  {
    Chain_Control   *chain =  &self->Ready[index];
    Chain_Node      *chain_node;
    for ( chain_node = _Chain_First( chain );
          chain_node != _Chain_Immutable_tail( chain ) ;
          chain_node = _Chain_Next( chain_node ) )
    {
      Thread_Control                       *thread;
      Scheduler_priority_affinity_SMP_Node *node;

      thread = (Thread_Control *) chain_node;
      node = _Scheduler_priority_affinity_SMP_Node_get( thread );

      /*
       * Can this thread run on this CPU?
       */
      if ( CPU_ISSET( victim_cpu, node->Affinity.set ) ) {
        highest = thread;
        break;
      }
    }
    if ( highest )
      break;
  }

  _Assert( highest != NULL );

  return highest;
}

/*
 * This method is very similar to _Scheduler_priority_affinity_SMP_Block
 * but has the difference that is invokes this scheduler's
 * get_highest_ready() support method.
 */
void _Scheduler_priority_affinity_SMP_Block(
  const Scheduler_Control *scheduler,
  Thread_Control *thread
)
{
  Scheduler_Context *context = _Scheduler_Get_context( scheduler );

  _Scheduler_SMP_Block(
    context,
    thread,
    _Scheduler_priority_SMP_Extract_from_ready,
    _Scheduler_priority_affinity_SMP_Get_highest_ready,
    _Scheduler_priority_SMP_Move_from_ready_to_scheduled,
    _Scheduler_SMP_Allocate_processor_exact
  );

  /*
   * Since this removed a single thread from the scheduled set
   * and selected the most appropriate thread from the ready
   * set to replace it, there should be no need for thread
   * migrations.
   */
}

/*
 * This method is unique to this scheduler because it must take into
 * account affinity as it searches for the lowest priority scheduled
 * thread. It ignores those which cannot be replaced by the filter
 * thread because the potential victim thread does not have affinity
 * for that processor.
 */
static Thread_Control *_Scheduler_priority_affinity_SMP_Get_lowest_scheduled(
  Scheduler_Context *context,
  Thread_Control    *filter,
  Chain_Node_order   order
)
{
  Scheduler_SMP_Context *self = _Scheduler_SMP_Get_self( context );
  Thread_Control  *lowest_scheduled = NULL;
  Thread_Control  *thread = NULL;
  Chain_Control   *scheduled = &self->Scheduled;
  Scheduler_priority_affinity_SMP_Node *node =
    _Scheduler_priority_affinity_SMP_Node_get( filter );

  for ( thread =  (Thread_Control *) _Chain_Last( scheduled );
        (Chain_Node *) thread != _Chain_Immutable_head( scheduled ) ;
        thread = (Thread_Control *) _Chain_Previous( &thread->Object.Node ) ) {
    int   cpu_index;

    /*
     * If we didn't find a thread which is of equal or lower importance
     * than filter thread is, then we can't schedule the filter thread
     * to execute.
     */
    if ( (*order)(&thread->Object.Node, &filter->Object.Node) )
      break;

    /* cpu_index is the processor number thread is executing on */
    cpu_index = _Per_CPU_Get_index( _Thread_Get_CPU( thread ) );

    if ( CPU_ISSET( cpu_index, node->Affinity.set ) ) {
      lowest_scheduled = thread;
      break;
    }

  }

  return lowest_scheduled;
}

/*
 * This method is unique to this scheduler because it must pass
 * _Scheduler_priority_affinity_SMP_Get_lowest_scheduled into
 * _Scheduler_SMP_Enqueue_ordered.
 */
static void _Scheduler_priority_affinity_SMP_Enqueue_fifo(
  Scheduler_Context *context,
  Thread_Control *thread
)
{
  _Scheduler_SMP_Enqueue_ordered(
    context,
    thread,
    _Scheduler_priority_affinity_SMP_Insert_priority_fifo_order,
    _Scheduler_priority_SMP_Insert_ready_fifo,
    _Scheduler_SMP_Insert_scheduled_fifo,
    _Scheduler_priority_SMP_Move_from_scheduled_to_ready,
    _Scheduler_priority_affinity_SMP_Get_lowest_scheduled,
    _Scheduler_SMP_Allocate_processor_exact
  );
}

/*
 * This method is invoked at the end of certain scheduling operations
 * to ensure that the highest priority ready thread cannot be scheduled
 * to execute. When we schedule with affinity, there is the possibility
 * that we need to migrate a thread to another core to ensure that the
 * highest priority ready threads are in fact scheduled.
 */
static void _Scheduler_priority_affinity_SMP_Check_for_migrations(
  Scheduler_Context *context
)
{
  Thread_Control        *lowest_scheduled;
  Thread_Control        *highest_ready;
  Scheduler_SMP_Node    *lowest_scheduled_node;
  Scheduler_SMP_Context *self = _Scheduler_SMP_Get_self( context );

  while (1) {
    highest_ready =
      _Scheduler_priority_affinity_SMP_Get_highest_ready( context, NULL );
    lowest_scheduled = _Scheduler_priority_affinity_SMP_Get_lowest_scheduled(
      context,
      highest_ready,
      _Scheduler_simple_Insert_priority_lifo_order
    );

    /*
     * If we can't find a thread to displace from the scheduled set,
     * then we have placed all the highest priority threads possible
     * in the scheduled set.
     *
     * We found the absolute highest priority thread without
     * considering affinity. But now we have to consider that thread's
     * affinity as we look to place it.
     */
    if ( lowest_scheduled == NULL )
      break;

    /*
     * But if we found a thread which is lower priority than one
     * in the ready set, then we need to swap them out.
     */
    lowest_scheduled_node = _Scheduler_SMP_Node_get( lowest_scheduled );

    _Scheduler_SMP_Node_change_state(
      lowest_scheduled_node,
      SCHEDULER_SMP_NODE_READY
    );

    _Scheduler_SMP_Allocate_processor_exact(
      self,
      highest_ready,
      lowest_scheduled
    );

    _Scheduler_priority_SMP_Move_from_ready_to_scheduled(
      context,
      highest_ready
    );

    _Scheduler_priority_SMP_Move_from_scheduled_to_ready(
      &self->Base,
      lowest_scheduled
    );
  }
}

/*
 * This is the public scheduler specific Unblock operation.
 */
void _Scheduler_priority_affinity_SMP_Unblock(
  const Scheduler_Control *scheduler,
  Thread_Control *thread
)
{
  Scheduler_Context *context = _Scheduler_Get_context( scheduler );

  _Scheduler_SMP_Unblock(
    context,
    thread,
    _Scheduler_priority_affinity_SMP_Enqueue_fifo
  );

  /*
   * Perform any thread migrations that are needed due to these changes.
   */
  _Scheduler_priority_affinity_SMP_Check_for_migrations( context );
}

/*
 *  This is unique to this scheduler because it passes scheduler specific
 *  get_lowest_scheduled helper to _Scheduler_SMP_Enqueue_ordered.
 */
static void _Scheduler_priority_affinity_SMP_Enqueue_ordered(
  Scheduler_Context     *context,
  Thread_Control        *thread,
  Chain_Node_order       order,
  Scheduler_SMP_Insert   insert_ready,
  Scheduler_SMP_Insert   insert_scheduled
)
{
  _Scheduler_SMP_Enqueue_ordered(
    context,
    thread,
    order,
    insert_ready,
    insert_scheduled,
    _Scheduler_priority_SMP_Move_from_scheduled_to_ready,
    _Scheduler_priority_affinity_SMP_Get_lowest_scheduled,
    _Scheduler_SMP_Allocate_processor_exact
  );
}

/*
 *  This is unique to this scheduler because it is on the path
 *  to _Scheduler_priority_affinity_SMP_Enqueue_ordered() which
 *  invokes a scheduler unique get_lowest_scheduled helper.
 */
static void _Scheduler_priority_affinity_SMP_Enqueue_lifo(
  Scheduler_Context *context,
  Thread_Control *thread
)
{
  _Scheduler_priority_affinity_SMP_Enqueue_ordered(
    context,
    thread,
    _Scheduler_priority_affinity_SMP_Insert_priority_lifo_order,
    _Scheduler_priority_SMP_Insert_ready_lifo,
    _Scheduler_SMP_Insert_scheduled_lifo
  );
}

/*
 * This method is unique to this scheduler because it must
 * invoke _Scheduler_SMP_Enqueue_scheduled_ordered() with
 * this scheduler's get_highest_ready() helper.
 */
static void _Scheduler_priority_affinity_SMP_Enqueue_scheduled_ordered(
  Scheduler_Context *context,
  Thread_Control *thread,
  Chain_Node_order order,
  Scheduler_SMP_Insert insert_ready,
  Scheduler_SMP_Insert insert_scheduled
)
{
  _Scheduler_SMP_Enqueue_scheduled_ordered(
    context,
    thread,
    order,
    _Scheduler_priority_affinity_SMP_Get_highest_ready,
    insert_ready,
    insert_scheduled,
    _Scheduler_priority_SMP_Move_from_ready_to_scheduled,
    _Scheduler_SMP_Allocate_processor_exact
  );
}

/*
 *  This is unique to this scheduler because it is on the path
 *  to _Scheduler_priority_affinity_SMP_Enqueue_scheduled__ordered() which
 *  invokes a scheduler unique get_lowest_scheduled helper.
 */
static void _Scheduler_priority_affinity_SMP_Enqueue_scheduled_lifo(
  Scheduler_Context *context,
  Thread_Control *thread
)
{
  _Scheduler_priority_affinity_SMP_Enqueue_scheduled_ordered(
    context,
    thread,
    _Scheduler_simple_Insert_priority_lifo_order,
    _Scheduler_priority_SMP_Insert_ready_lifo,
    _Scheduler_SMP_Insert_scheduled_lifo
  );
}

/*
 *  This is unique to this scheduler because it is on the path
 *  to _Scheduler_priority_affinity_SMP_Enqueue_scheduled__ordered() which
 *  invokes a scheduler unique get_lowest_scheduled helper.
 */
static void _Scheduler_priority_affinity_SMP_Enqueue_scheduled_fifo(
  Scheduler_Context *context,
  Thread_Control *thread
)
{
  _Scheduler_priority_affinity_SMP_Enqueue_scheduled_ordered(
    context,
    thread,
    _Scheduler_simple_Insert_priority_fifo_order,
    _Scheduler_priority_SMP_Insert_ready_fifo,
    _Scheduler_SMP_Insert_scheduled_fifo
  );
}

/*
 * This is the public scheduler specific Change Priority operation.
 */
void _Scheduler_priority_affinity_SMP_Change_priority(
  const Scheduler_Control *scheduler,
  Thread_Control          *thread,
  Priority_Control         new_priority,
  bool                     prepend_it
)
{
  Scheduler_Context *context = _Scheduler_Get_context( scheduler );

  _Scheduler_SMP_Change_priority(
    context,
    thread,
    new_priority,
    prepend_it,
    _Scheduler_priority_SMP_Extract_from_ready,
    _Scheduler_priority_SMP_Do_update,
    _Scheduler_priority_affinity_SMP_Enqueue_fifo,
    _Scheduler_priority_affinity_SMP_Enqueue_lifo,
    _Scheduler_priority_affinity_SMP_Enqueue_scheduled_fifo,
    _Scheduler_priority_affinity_SMP_Enqueue_scheduled_lifo
  );

  /*
   * Perform any thread migrations that are needed due to these changes.
   */
  _Scheduler_priority_affinity_SMP_Check_for_migrations( context );
}

/*
 * This is the public scheduler specific Change Priority operation.
 */
bool _Scheduler_priority_affinity_SMP_Get_affinity(
  const Scheduler_Control *scheduler,
  Thread_Control          *thread,
  size_t                   cpusetsize,
  cpu_set_t               *cpuset
)
{
  Scheduler_priority_affinity_SMP_Node *node =
    _Scheduler_priority_affinity_SMP_Node_get(thread);

  (void) scheduler;

  if ( node->Affinity.setsize != cpusetsize ) {
    return false;
  }

  CPU_COPY( cpuset, node->Affinity.set );
  return true;
}

bool _Scheduler_priority_affinity_SMP_Set_affinity(
  const Scheduler_Control *scheduler,
  Thread_Control          *thread,
  size_t                   cpusetsize,
  const cpu_set_t         *cpuset
)
{
  Scheduler_priority_affinity_SMP_Node *node =
    _Scheduler_priority_affinity_SMP_Node_get(thread);

  (void) scheduler;

  /*
   * Validate that the cpset meets basic requirements.
   */
  if ( !_CPU_set_Is_valid( cpuset, cpusetsize ) ) {
    return false;
  }

  /*
   * The old and new set are the same, there is no point in
   * doing anything.
   */
  if ( CPU_EQUAL_S( cpusetsize, cpuset, node->Affinity.set ) )
    return true;

  _Thread_Set_state( thread, STATES_MIGRATING );
    CPU_COPY( node->Affinity.set, cpuset );
  _Thread_Clear_state( thread, STATES_MIGRATING );

  return true;
}
