/**
 * @file
 *
 * @ingroup ScoreSMPImpl
 *
 * @brief SuperCore SMP Implementation
 */

/*
 *  COPYRIGHT (c) 1989-2011.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifndef _RTEMS_SCORE_SMPIMPL_H
#define _RTEMS_SCORE_SMPIMPL_H

#include <rtems/score/smp.h>
#include <rtems/score/percpu.h>
#include <rtems/fatal.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup ScoreSMP SMP Support
 *
 * @ingroup Score
 *
 * This defines the interface of the SuperCore SMP support.
 *
 * @{
 */

/**
 * @brief SMP message to request a processor shutdown.
 *
 * @see _SMP_Send_message().
 */
#define SMP_MESSAGE_SHUTDOWN UINT32_C(0x1)

/**
 * @brief SMP fatal codes.
 */
typedef enum {
  SMP_FATAL_SHUTDOWN
} SMP_Fatal_code;

/**
 *  @brief Initialize SMP Handler
 *
 *  This method initialize the SMP Handler.
 */
#if defined( RTEMS_SMP )
  void _SMP_Handler_initialize( void );
#else
  #define _SMP_Handler_initialize() \
    do { } while ( 0 )
#endif

#if defined( RTEMS_SMP )

/**
 * @brief Performs high-level initialization of a secondary processor and runs
 * the application threads.
 *
 * The low-level initialization code must call this function to hand over the
 * control of this processor to RTEMS.  Interrupts must be disabled.  It must
 * be possible to send inter-processor interrupts to this processor.  Since
 * interrupts are disabled the inter-processor interrupt delivery is postponed
 * until interrupts are enabled the first time.  Interrupts are enabled during
 * the execution begin of threads in case they have interrupt level zero (this
 * is the default).
 *
 * The pre-requisites for the call to this function are
 * - disabled interrupts,
 * - delivery of inter-processor interrupts is possible,
 * - a valid stack pointer and enough stack space,
 * - a valid code memory, and
 * - a valid BSS section.
 *
 * This function must not be called by the main processor.  The main processor
 * uses _Thread_Start_multitasking() instead.
 *
 * This function does not return to the caller.
 */
void _SMP_Start_multitasking_on_secondary_processor( void )
  RTEMS_COMPILER_NO_RETURN_ATTRIBUTE;

/**
 * @brief Interrupt handler for inter-processor interrupts.
 */
static inline void _SMP_Inter_processor_interrupt_handler( void )
{
  Per_CPU_Control *self_cpu = _Per_CPU_Get();

  if ( self_cpu->message != 0 ) {
    uint32_t  message;
    ISR_Level level;

    _Per_CPU_ISR_disable_and_acquire( self_cpu, level );
    message = self_cpu->message;
    self_cpu->message = 0;
    _Per_CPU_Release_and_ISR_enable( self_cpu, level );

    if ( ( message & SMP_MESSAGE_SHUTDOWN ) != 0 ) {
      rtems_fatal( RTEMS_FATAL_SOURCE_SMP, SMP_FATAL_SHUTDOWN );
      /* does not continue past here */
    }
  }
}

/**
 *  @brief Sends a SMP message to a processor.
 *
 *  The target processor may be the sending processor.
 *
 *  @param[in] cpu The target processor of the message.
 *  @param[in] message The message.
 */
void _SMP_Send_message( uint32_t cpu, uint32_t message );

/**
 *  @brief Request of others CPUs.
 *
 *  This method is invoked by RTEMS when it needs to make a request
 *  of the other CPUs.  It should be implemented using some type of
 *  interprocessor interrupt. CPUs not including the originating
 *  CPU should receive the message.
 *
 *  @param [in] message is message to send
 */
void _SMP_Broadcast_message(
  uint32_t  message
);

#endif /* defined( RTEMS_SMP ) */

/**
 * @brief Requests a multitasking start on all configured and available
 * processors.
 */
#if defined( RTEMS_SMP )
  void _SMP_Request_start_multitasking( void );
#else
  #define _SMP_Request_start_multitasking() \
    do { } while ( 0 )
#endif

/**
 * @brief Requests a shutdown of all processors.
 *
 * This function is a part of the system termination procedure.
 *
 * @see _Terminate().
 */
#if defined( RTEMS_SMP )
  void _SMP_Request_shutdown( void );
#else
  #define _SMP_Request_shutdown() \
    do { } while ( 0 )
#endif

/** @} */

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
