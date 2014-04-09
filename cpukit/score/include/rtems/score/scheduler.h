/**
 *  @file  rtems/score/scheduler.h
 *
 *  @brief Constants and Structures Associated with the Scheduler
 *
 *  This include file contains all the constants and structures associated
 *  with the scheduler.
 */

/*
 *  Copyright (C) 2010 Gedare Bloom.
 *  Copyright (C) 2011 On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifndef _RTEMS_SCORE_SCHEDULER_H
#define _RTEMS_SCORE_SCHEDULER_H

#include <rtems/score/percpu.h>
#include <rtems/score/priority.h>
#include <rtems/score/thread.h>
#if defined(__RTEMS_HAVE_SYS_CPUSET_H__) && defined(RTEMS_SMP)
  #include <sys/cpuset.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @defgroup ScoreScheduler Scheduler Handler
 *
 *  @ingroup Score
 *
 *  This handler encapsulates functionality related to managing sets of threads
 *  that are ready for execution.
 */
/**@{*/

typedef struct Scheduler_Control Scheduler_Control;

/**
 * function jump table that holds pointers to the functions that
 * implement specific schedulers.
 */
typedef struct {
  /** Implements the scheduling decision logic (policy). */
  void ( *initialize )( const Scheduler_Control * );

  /** Implements the scheduling decision logic (policy). */
  void ( *schedule )( const Scheduler_Control *, Thread_Control *);

  /**
   * @brief Voluntarily yields the processor per the scheduling policy.
   *
   * @see _Scheduler_Yield().
   */
  void ( *yield )( const Scheduler_Control *, Thread_Control *);

  /** Removes the given thread from scheduling decisions. */
  void ( *block )( const Scheduler_Control *, Thread_Control * );

  /** Adds the given thread to scheduling decisions. */
  void ( *unblock )( const Scheduler_Control *, Thread_Control * );

  /** allocates the scheduler field of the given thread */
  bool ( *allocate )( const Scheduler_Control *, Thread_Control * );

  /** frees the scheduler field of the given thread */
  void ( *free )( const Scheduler_Control *, Thread_Control * );

  /** updates the scheduler field of the given thread -- primarily used
   * when changing the thread's priority. */
  void ( *update )( const Scheduler_Control *, Thread_Control * );

  /** enqueue a thread as the last of its priority group */
  void ( *enqueue )( const Scheduler_Control *, Thread_Control * );

  /** enqueue a thread as the first of its priority group */
  void ( *enqueue_first )( const Scheduler_Control *, Thread_Control * );

  /** extract a thread from the ready set */
  void ( *extract )( const Scheduler_Control *, Thread_Control * );

  /**
   * Compares two priorities (returns >0 for higher priority, 0 for equal
   * and <0 for lower priority).
   */
  int ( *priority_compare )(
    Priority_Control,
    Priority_Control
  );

  /** This routine is called upon release of a new job. */
  void ( *release_job ) (
    const Scheduler_Control *,
    Thread_Control *,
    uint32_t
  );

  /** perform scheduler update actions required at each clock tick */
  void ( *tick )( const Scheduler_Control * );

  /**
   * @brief Starts the idle thread for a particular processor.
   *
   * @see _Scheduler_Start_idle().
   */
  void ( *start_idle )(
    const Scheduler_Control *,
    Thread_Control *,
    Per_CPU_Control *
  );

#if defined(__RTEMS_HAVE_SYS_CPUSET_H__) && defined(RTEMS_SMP)
  /**
   * @brief Obtain the processor affinity for a thread.
   *
   * @see _Scheduler_Get_affinity().
   */
  bool ( *get_affinity )(
    const Scheduler_Control *,
    Thread_Control *,
    size_t,
    cpu_set_t *
  );
  
  /**
   * @brief Set the processor affinity for a thread.
   *
   * @see _Scheduler_Set_affinity().
   */
  bool ( *set_affinity )(
    const Scheduler_Control *,
    Thread_Control *,
    size_t,
    const cpu_set_t *
  );
#endif
} Scheduler_Operations;

/**
 * @brief Scheduler context.
 *
 * The scheduler context of a particular scheduler implementation must place
 * this structure at the begin of its context structure.
 */
typedef struct {
  /* No fields yet */
} Scheduler_Context;

/**
 * @brief Scheduler control.
 */
struct Scheduler_Control {
  /**
   * @brief Reference to a statically allocated scheduler context.
   */
  Scheduler_Context *context;

  /**
   * @brief The scheduler operations.
   */
  Scheduler_Operations Operations;

  /**
   * @brief The scheduler name.
   */
  uint32_t name;
};

/**
 * @brief Registered schedulers.
 *
 * Application provided via <rtems/confdefs.h>.
 *
 * @see _Scheduler_Count.
 */
extern const Scheduler_Control _Scheduler_Table[];

/**
 * @brief Count of registered schedulers.
 *
 * Application provided via <rtems/confdefs.h> on SMP configurations.
 *
 * It is very important that this is a compile-time constant on uni-processor
 * configurations (in this case RTEMS_SMP is not defined) so that the compiler
 * can optimize the some loops away
 *
 * @see _Scheduler_Table.
 */
#if defined(RTEMS_SMP)
  extern const size_t _Scheduler_Count;
#else
  #define _Scheduler_Count ( (size_t) 1 )
#endif

/**
 * @brief Returns an arbitrary non-NULL value.
 *
 * @param[in] scheduler Unused.
 * @param[in] the_thread Unused.
 *
 * @retval true Always.
 */
bool _Scheduler_default_Allocate(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread
);

/**
 * @brief Does nothing.
 *
 * @param[in] scheduler Unused.
 * @param[in] the_thread Unused.
 */
void _Scheduler_default_Free(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread
);

/**
 * @brief Does nothing.
 *
 * @param[in] scheduler Unused.
 * @param[in] the_thread Unused.
 */
void _Scheduler_default_Update(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread
);

/**
 * @brief Does nothing.
 *
 * @param[in] scheduler Unused.
 * @param[in] the_thread Unused.
 * @param[in] deadline Unused.
 */
void _Scheduler_default_Release_job(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread,
  uint32_t                 deadline
);

/**
 * @brief Performs tick operations depending on the CPU budget algorithm for
 * each executing thread.
 *
 * This routine is invoked as part of processing each clock tick.
 *
 * @param[in] scheduler The scheduler.
 */
void _Scheduler_default_Tick( const Scheduler_Control *scheduler );

/**
 * @brief Starts an idle thread.
 *
 * @param[in] scheduler The scheduler.
 * @param[in] the_thread An idle thread.
 * @param[in] cpu This parameter is unused.
 */
void _Scheduler_default_Start_idle(
  const Scheduler_Control *scheduler,
  Thread_Control          *the_thread,
  Per_CPU_Control         *cpu
);

#if defined(__RTEMS_HAVE_SYS_CPUSET_H__) && defined(RTEMS_SMP)
  /**
   * @brief Get affinity for the default scheduler.
   *
   * @param[in] thread The associated thread.
   * @param[in] cpusetsize The size of the cpuset.
   * @param[out] cpuset Affinity set containing all CPUs.
   *
   * @retval 0 Successfully got cpuset
   * @retval -1 The cpusetsize is invalid for the system
   */
  bool _Scheduler_default_Get_affinity(
    const Scheduler_Control *scheduler,
    Thread_Control          *thread,
    size_t                   cpusetsize,
    cpu_set_t               *cpuset
  );

  /** 
   * @brief Set affinity for the default scheduler.
   *
   * @param[in] thread The associated thread.
   * @param[in] cpusetsize The size of the cpuset.
   * @param[in] cpuset Affinity new affinity set.
   *
   * @retval 0 Successful
   *
   *  This method always returns successful and does not save
   *  the cpuset.
   */
  bool _Scheduler_default_Set_affinity(
    const Scheduler_Control *scheduler,
    Thread_Control          *thread,
    size_t                   cpusetsize,
    const cpu_set_t         *cpuset
  );
#endif

/*
 * See also PR2174: Memory corruption with EDF scheduler and thread priority
 * queues.
 */
extern const bool _Scheduler_FIXME_thread_priority_queues_are_broken;

/**@}*/

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
