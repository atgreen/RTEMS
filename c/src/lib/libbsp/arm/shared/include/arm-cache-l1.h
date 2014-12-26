/**
 * @file arm-cache-l1.h
 *
 * @ingroup arm_shared
 *
 * @brief Level 1 Cache definitions and functions.
 * 
 * This file implements handling for the ARM Level 1 cache controller
 */

/*
 * Copyright (c) 2014 embedded brains GmbH.  All rights reserved.
 *
 *  embedded brains GmbH
 *  Dornierstr. 4
 *  82178 Puchheim
 *  Germany
 *  <rtems@embedded-brains.de>
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifndef LIBBSP_ARM_SHARED_CACHE_L1_H
#define LIBBSP_ARM_SHARED_CACHE_L1_H

#include <assert.h>
#include <bsp.h>
#include <libcpu/arm-cp15.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* These two defines also ensure that the rtems_cache_* functions have bodies */
#define ARM_CACHE_L1_CPU_DATA_ALIGNMENT 32
#define ARM_CACHE_L1_CPU_INSTRUCTION_ALIGNMENT 32
#define ARM_CACHE_L1_CPU_SUPPORT_PROVIDES_RANGE_FUNCTIONS

#define ARM_CACHE_L1_CSS_ID_DATA 0
#define ARM_CACHE_L1_CSS_ID_INSTRUCTION 1
#define ARM_CACHE_L1_DATA_LINE_MASK ( ARM_CACHE_L1_CPU_DATA_ALIGNMENT - 1 )
#define ARM_CACHE_L1_INSTRUCTION_LINE_MASK \
  ( ARM_CACHE_L1_CPU_INSTRUCTION_ALIGNMENT \
    - 1 )

/* Errata Handlers */
#if ( defined( RTEMS_SMP ) )
  #define ARM_CACHE_L1_ERRATA_764369_HANDLER()                 \
    if( arm_errata_is_applicable_processor_errata_764369() ) { \
      _ARM_Data_synchronization_barrier();                     \
    }                                           
#else /* #if ( defined( RTEMS_SMP ) ) */
  #define ARM_CACHE_L1_ERRATA_764369_HANDLER()
#endif /* #if ( defined( RTEMS_SMP ) ) */

    
static void arm_cache_l1_select( const uint32_t selection )
{
  /* select current cache level in cssr */
  arm_cp15_set_cache_size_selection( selection );

  /* isb to sych the new cssr&csidr */
  _ARM_Instruction_synchronization_barrier();
}

/*
 * @param l1LineSize      Number of bytes in cache line expressed as power of 
 *                        2 value
 * @param l1Associativity Associativity of cache. The associativity does not 
 *                        have to be a power of 2.
 * qparam liNumSets       Number of sets in cache
 * */

static inline void arm_cache_l1_properties( 
  uint32_t *l1LineSize,
  uint32_t *l1Associativity,
  uint32_t *l1NumSets )
{
  uint32_t id;

  _ARM_Instruction_synchronization_barrier();
  id               = arm_cp15_get_cache_size_id();

  /* Cache line size in words + 2 -> bytes) */
  *l1LineSize      = ( id & 0x0007U ) + 2 + 2;
  /* Number of Ways */
  *l1Associativity = ( ( id >> 3 ) & 0x03ffU ) + 1; 
  /* Number of Sets */
  *l1NumSets       = ( ( id >> 13 ) & 0x7fffU ) + 1;
}

/*
 * @param log_2_line_bytes The number of bytes per cache line expressed in log2
 * @param associativity    The associativity of the cache beeing operated
 * @param cache_level_idx  The level of the cache beeing operated minus 1 e.g 0
 *                         for cache level 1
 * @param set              Number of the set to operate on
 * @param way              Number of the way to operate on
 * */

static inline uint32_t arm_cache_l1_get_set_way_param(
  const uint32_t log_2_line_bytes,
  const uint32_t associativity,
  const uint32_t cache_level_idx,
  const uint32_t set,
  const uint32_t way )
{
  uint32_t way_shift = __builtin_clz( associativity - 1 );


  return ( 0
           | ( way
    << way_shift ) | ( set << log_2_line_bytes ) | ( cache_level_idx << 1 ) );
}

static inline void arm_cache_l1_flush_1_data_line( const void *d_addr )
{
  /* Flush the Data cache */
  arm_cp15_data_cache_clean_and_invalidate_line( d_addr );

  /* Wait for L1 flush to complete */
  _ARM_Data_synchronization_barrier();
}

static inline void arm_cache_l1_flush_entire_data( void )
{
  uint32_t              l1LineSize, l1Associativity, l1NumSets;
  uint32_t              s, w;
  uint32_t              set_way_param;
  rtems_interrupt_level level;


  /* ensure ordering with previous memory accesses */
  _ARM_Data_memory_barrier();

  /* make cssr&csidr read atomic */
  rtems_interrupt_disable( level );

  /* Get the L1 cache properties */
  arm_cache_l1_properties( &l1LineSize, &l1Associativity,
                                     &l1NumSets );
  rtems_interrupt_enable( level );

  for ( w = 0; w < l1Associativity; ++w ) {
    for ( s = 0; s < l1NumSets; ++s ) {
      set_way_param = arm_cache_l1_get_set_way_param(
        l1LineSize,
        l1Associativity,
        0,
        s,
        w
        );
      arm_cp15_data_cache_clean_line_by_set_and_way( set_way_param );
    }
  }

  /* Wait for L1 flush to complete */
  _ARM_Data_synchronization_barrier();
}

static inline void arm_cache_l1_invalidate_entire_data( void )
{
  uint32_t              l1LineSize, l1Associativity, l1NumSets;
  uint32_t              s, w;
  uint32_t              set_way_param;
  rtems_interrupt_level level;


  /* ensure ordering with previous memory accesses */
  _ARM_Data_memory_barrier();

  /* make cssr&csidr read atomic */
  rtems_interrupt_disable( level );

  /* Get the L1 cache properties */
  arm_cache_l1_properties( &l1LineSize, &l1Associativity,
                                     &l1NumSets );
  rtems_interrupt_enable( level );

  for ( w = 0; w < l1Associativity; ++w ) {
    for ( s = 0; s < l1NumSets; ++s ) {
      set_way_param = arm_cache_l1_get_set_way_param(
        l1LineSize,
        l1Associativity,
        0,
        s,
        w
        );
      arm_cp15_data_cache_invalidate_line_by_set_and_way( set_way_param );
    }
  }

  /* Wait for L1 invalidate to complete */
  _ARM_Data_synchronization_barrier();
}

static inline void arm_cache_l1_clean_and_invalidate_entire_data( void )
{
  uint32_t              l1LineSize, l1Associativity, l1NumSets;
  uint32_t              s, w;
  uint32_t              set_way_param;
  rtems_interrupt_level level;


  /* ensure ordering with previous memory accesses */
  _ARM_Data_memory_barrier();

  /* make cssr&csidr read atomic */
  rtems_interrupt_disable( level );

  /* Get the L1 cache properties */
  arm_cache_l1_properties( &l1LineSize, &l1Associativity,
                                     &l1NumSets );
  rtems_interrupt_enable( level );

  for ( w = 0; w < l1Associativity; ++w ) {
    for ( s = 0; s < l1NumSets; ++s ) {
      set_way_param = arm_cache_l1_get_set_way_param(
        l1LineSize,
        l1Associativity,
        0,
        s,
        w
        );
      arm_cp15_data_cache_clean_and_invalidate_line_by_set_and_way(
        set_way_param );
    }
  }

  /* Wait for L1 invalidate to complete */
  _ARM_Data_synchronization_barrier();
}

static inline void arm_cache_l1_store_data( const void *d_addr )
{
  /* Store the Data cache line */
  arm_cp15_data_cache_clean_line( d_addr );

  /* Wait for L1 store to complete */
  _ARM_Data_synchronization_barrier();
}

static inline void arm_cache_l1_flush_data_range( 
  const void *d_addr,
  size_t      n_bytes
)
{
  if ( n_bytes != 0 ) {
    uint32_t       adx       = (uint32_t) d_addr
                               & ~ARM_CACHE_L1_DATA_LINE_MASK;
    const uint32_t ADDR_LAST =
      ( (uint32_t) d_addr + n_bytes - 1 ) & ~ARM_CACHE_L1_DATA_LINE_MASK;
      
    ARM_CACHE_L1_ERRATA_764369_HANDLER();

    for (; adx <= ADDR_LAST; adx += ARM_CACHE_L1_CPU_DATA_ALIGNMENT ) {
      /* Store the Data cache line */
      arm_cp15_data_cache_clean_line( (void*)adx );
    }
    /* Wait for L1 store to complete */
    _ARM_Data_synchronization_barrier();
  }
}


static inline void arm_cache_l1_invalidate_1_data_line(
  const void *d_addr )
{
  /* Invalidate the data cache line */
  arm_cp15_data_cache_invalidate_line( d_addr );

  /* Wait for L1 invalidate to complete */
  _ARM_Data_synchronization_barrier();
}

static inline void arm_cache_l1_freeze_data( void )
{
  /* To be implemented as needed, if supported by hardware at all */
}

static inline void arm_cache_l1_unfreeze_data( void )
{
  /* To be implemented as needed, if supported by hardware at all */
}

static inline void arm_cache_l1_invalidate_1_instruction_line(
  const void *i_addr )
{
  /* Invalidate the Instruction cache line */
  arm_cp15_instruction_cache_invalidate_line( i_addr );

  /* Wait for L1 invalidate to complete */
  _ARM_Data_synchronization_barrier();
}

static inline void arm_cache_l1_invalidate_data_range(
  const void *d_addr,
  size_t      n_bytes
)
{
  if ( n_bytes != 0 ) {
    uint32_t       adx = (uint32_t) d_addr
                         & ~ARM_CACHE_L1_DATA_LINE_MASK;
    const uint32_t end =
      ( adx + n_bytes ) & ~ARM_CACHE_L1_DATA_LINE_MASK;

    ARM_CACHE_L1_ERRATA_764369_HANDLER();
    
    /* Back starting address up to start of a line and invalidate until end */
    for (;
         adx < end;
         adx += ARM_CACHE_L1_CPU_DATA_ALIGNMENT ) {
        /* Invalidate the Instruction cache line */
        arm_cp15_data_cache_invalidate_line( (void*)adx );
    }
    /* Wait for L1 invalidate to complete */
    _ARM_Data_synchronization_barrier();
  }
}

static inline void arm_cache_l1_invalidate_instruction_range(
  const void *i_addr,
  size_t      n_bytes
)
{
  if ( n_bytes != 0 ) {
    uint32_t       adx = (uint32_t) i_addr
                         & ~ARM_CACHE_L1_INSTRUCTION_LINE_MASK;
    const uint32_t end =
      ( adx + n_bytes ) & ~ARM_CACHE_L1_INSTRUCTION_LINE_MASK;

    arm_cache_l1_select( ARM_CACHE_L1_CSS_ID_INSTRUCTION );
    
    ARM_CACHE_L1_ERRATA_764369_HANDLER();
    
    /* Back starting address up to start of a line and invalidate until end */
    for (;
         adx < end;
         adx += ARM_CACHE_L1_CPU_INSTRUCTION_ALIGNMENT ) {
        /* Invalidate the Instruction cache line */
        arm_cp15_instruction_cache_invalidate_line( (void*)adx );
    }
    /* Wait for L1 invalidate to complete */
    _ARM_Data_synchronization_barrier();

    arm_cache_l1_select( ARM_CACHE_L1_CSS_ID_DATA );
  }
}

static inline void arm_cache_l1_invalidate_entire_instruction( void )
{
  uint32_t ctrl = arm_cp15_get_control();


  #ifdef RTEMS_SMP

  /* invalidate I-cache inner shareable */
  arm_cp15_instruction_cache_inner_shareable_invalidate_all();

  /* I+BTB cache invalidate */
  arm_cp15_instruction_cache_invalidate();
  #else /* RTEMS_SMP */
  /* I+BTB cache invalidate */
  arm_cp15_instruction_cache_invalidate();
  #endif /* RTEMS_SMP */

  if ( ( ctrl & ARM_CP15_CTRL_Z ) == 0 ) {
    arm_cp15_branch_predictor_inner_shareable_invalidate_all();
    arm_cp15_branch_predictor_invalidate_all();
  }
}

static inline void arm_cache_l1_freeze_instruction( void )
{
  /* To be implemented as needed, if supported by hardware at all */
}

static inline void arm_cache_l1_unfreeze_instruction( void )
{
  /* To be implemented as needed, if supported by hardware at all */
}

static inline void arm_cache_l1_enable_data( void )
{
  rtems_interrupt_level level;
  uint32_t              ctrl;


  arm_cache_l1_select( ARM_CACHE_L1_CSS_ID_DATA );

  assert( ARM_CACHE_L1_CPU_DATA_ALIGNMENT == arm_cp15_get_data_cache_line_size() );

  rtems_interrupt_disable( level );
  ctrl = arm_cp15_get_control();
  rtems_interrupt_enable( level );

  /* Only enable the cache if it is disabled */
  if ( !( ctrl & ARM_CP15_CTRL_C ) ) {
    /* Clean and invalidate the Data cache */
    arm_cache_l1_invalidate_entire_data();

    /* Enable the Data cache */
    ctrl |= ARM_CP15_CTRL_C;

    rtems_interrupt_disable( level );
    arm_cp15_set_control( ctrl );
    rtems_interrupt_enable( level );
  }
}

static inline void arm_cache_l1_disable_data( void )
{
  rtems_interrupt_level level;


  /* Clean and invalidate the Data cache */
  arm_cache_l1_flush_entire_data();

  rtems_interrupt_disable( level );

  /* Disable the Data cache */
  arm_cp15_set_control( arm_cp15_get_control() & ~ARM_CP15_CTRL_C );

  rtems_interrupt_enable( level );
}

static inline void arm_cache_l1_disable_instruction( void )
{
  rtems_interrupt_level level;


  rtems_interrupt_disable( level );

  /* Synchronize the processor */
  _ARM_Data_synchronization_barrier();

  /* Invalidate the Instruction cache */
  arm_cache_l1_invalidate_entire_instruction();

  /* Disable the Instruction cache */
  arm_cp15_set_control( arm_cp15_get_control() & ~ARM_CP15_CTRL_I );

  rtems_interrupt_enable( level );
}

static inline void arm_cache_l1_enable_instruction( void )
{
  rtems_interrupt_level level;
  uint32_t              ctrl;


  arm_cache_l1_select( ARM_CACHE_L1_CSS_ID_INSTRUCTION );

  assert( ARM_CACHE_L1_CPU_INSTRUCTION_ALIGNMENT
          == arm_cp15_get_data_cache_line_size() );

  rtems_interrupt_disable( level );

  /* Enable Instruction cache only if it is disabled */
  ctrl = arm_cp15_get_control();

  if ( !( ctrl & ARM_CP15_CTRL_I ) ) {
    /* Invalidate the Instruction cache */
    arm_cache_l1_invalidate_entire_instruction();

    /* Enable the Instruction cache */
    ctrl |= ARM_CP15_CTRL_I;

    arm_cp15_set_control( ctrl );
  }

  rtems_interrupt_enable( level );

  arm_cache_l1_select( ARM_CACHE_L1_CSS_ID_DATA );
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LIBBSP_ARM_SHARED_CACHE_L1_H */