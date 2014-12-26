/**
 * @file cache_.h
 *
 * @ingroup L2C-310_cache
 *
 * @brief Cache definitions and functions.
 * 
 * This file implements handling for the ARM L2C-310 cache controller
 */

/*
 * Authorship
 * ----------
 * This software was created by
 *     R. Claus <claus@slac.stanford.edu>, 2013,
 *       Stanford Linear Accelerator Center, Stanford University.
 *
 * Acknowledgement of sponsorship
 * ------------------------------
 * This software was produced by
 *     the Stanford Linear Accelerator Center, Stanford University,
 *     under Contract DE-AC03-76SFO0515 with the Department of Energy.
 *
 * Government disclaimer of liability
 * ----------------------------------
 * Neither the United States nor the United States Department of Energy,
 * nor any of their employees, makes any warranty, express or implied, or
 * assumes any legal liability or responsibility for the accuracy,
 * completeness, or usefulness of any data, apparatus, product, or process
 * disclosed, or represents that its use would not infringe privately owned
 * rights.
 *
 * Stanford disclaimer of liability
 * --------------------------------
 * Stanford University makes no representations or warranties, express or
 * implied, nor assumes any liability for the use of this software.
 *
 * Stanford disclaimer of copyright
 * --------------------------------
 * Stanford University, owner of the copyright, hereby disclaims its
 * copyright and all other rights in this software.  Hence, anyone may
 * freely use it for any purpose without restriction.
 *
 * Maintenance of notices
 * ----------------------
 * In the interest of clarity regarding the origin and status of this
 * SLAC software, this and all the preceding Stanford University notices
 * are to remain affixed to any copy or derivative of this software made
 * or distributed by the recipient and are to be affixed to any copy of
 * software made or distributed by the recipient that contains a copy or
 * derivative of this software.
 *
 * ------------------ SLAC Software Notices, Set 4 OTT.002a, 2004 FEB 03
 */

#ifndef LIBBSP_ARM_SHARED_L2C_310_CACHE_H
#define LIBBSP_ARM_SHARED_L2C_310_CACHE_H

#include <assert.h>
#include <bsp.h>
#include <libcpu/arm-cp15.h>
#include <bsp/arm-release-id.h>
#include <bsp/arm-errata.h>
#include "../include/arm-cache-l1.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* These two defines also ensure that the rtems_cache_* functions have bodies */
#define CPU_DATA_CACHE_ALIGNMENT ARM_CACHE_L1_CPU_DATA_ALIGNMENT
#define CPU_INSTRUCTION_CACHE_ALIGNMENT ARM_CACHE_L1_CPU_INSTRUCTION_ALIGNMENT
#define CPU_CACHE_SUPPORT_PROVIDES_RANGE_FUNCTIONS \
  ARM_CACHE_L1_CPU_SUPPORT_PROVIDES_RANGE_FUNCTIONS

#define CACHE_L2C_310_DATA_LINE_MASK ( CPU_DATA_CACHE_ALIGNMENT - 1 )
#define CACHE_L2C_310_INSTRUCTION_LINE_MASK \
  ( CPU_INSTRUCTION_CACHE_ALIGNMENT \
    - 1 )
#define CACHE_l2C_310_NUM_WAYS 8
#define CACHE_l2C_310_WAY_MASK ( ( 1 << CACHE_l2C_310_NUM_WAYS ) - 1 )


/* RTL release number as can be read from cache_id register */
typedef enum {
  CACHE_L2C_310_RTL_RELEASE_R0_P0 = 0x0,
  CACHE_L2C_310_RTL_RELEASE_R1_P0 = 0x2,
  CACHE_L2C_310_RTL_RELEASE_R2_P0 = 0x4,
  CACHE_L2C_310_RTL_RELEASE_R3_P0 = 0x5,
  CACHE_L2C_310_RTL_RELEASE_R3_P1 = 0x6,
  CACHE_L2C_310_RTL_RELEASE_R3_P2 = 0x8,
  CACHE_L2C_310_RTL_RELEASE_R3_P3 = 0x9
} cache_l2c_310_rtl_release;

/**
 * @defgroup L2C-310_cache Cache Support
 * @ingroup arm_shared
 * @brief Cache Functions and Defitions
 * @{
 */


/**
 * @brief L2CC Register Offsets
 */
typedef struct {
  /** @brief Cache ID */
  uint32_t cache_id;
#define CACHE_L2C_310_L2CC_ID_RTL_MASK 0x3f
#define CACHE_L2C_310_L2CC_ID_PART_MASK ( 0xf << 6 )
#define CACHE_L2C_310_L2CC_ID_PART_L210 ( 1 << 6 )
#define CACHE_L2C_310_L2CC_ID_PART_L310 ( 3 << 6 )
  /** @brief Cache type */
  uint32_t cache_type;
/** @brief 1 if data banking implemented, 0 if not */
#define CACHE_L2C_310_L2CC_TYPE_DATA_BANKING_MASK 0x80000000
/** @brief 11xy, where: x=1 if pl310_LOCKDOWN_BY_MASTER is defined, otherwise 0 */
#define CACHE_L2C_310_L2CC_TYPE_CTYPE_MASK 0x1E000000
/** @brief y=1 if pl310_LOCKDOWN_BY_LINE is defined, otherwise 0. */
#define CACHE_L2C_310_L2CC_TYPE_CTYPE_SHIFT 25
/** @brief 1 for Harvard architecture, 0 for unified architecture */
#define CACHE_L2C_310_L2CC_TYPE_HARVARD_MASK 0x01000000
/** @brief Data cache way size = 2 Exp(value + 2) KB */
#define CACHE_L2C_310_L2CC_TYPE_SIZE_D_WAYS_MASK 0x00700000
#define CACHE_L2C_310_L2CC_TYPE_SIZE_D_WAYS_SHIFT 20
/** @brief Assoziativity aka number of data ways = (value * 8) + 8 */
#define CACHE_L2C_310_L2CC_TYPE_NUM_D_WAYS_MASK 0x00040000
#define CACHE_L2C_310_L2CC_TYPE_NUM_D_WAYS_SHIFT 18
/** @brief Data cache line length 00 - 32 */
#define CACHE_L2C_310_L2CC_TYPE_LENGTH_D_LINE_MASK 0x00003000
#define CACHE_L2C_310_L2CC_TYPE_LENGTH_D_LINE_SHIFT 12
#define CACHE_L2C_310_L2CC_TYPE_LENGTH_D_LINE_VAL_32 0x0
/** @brief Instruction cache way size = 2 Exp(value + 2) KB */
#define CACHE_L2C_310_L2CC_TYPE_SIZE_I_WAYS_MASK 0x00000700
#define CACHE_L2C_310_L2CC_TYPE_SIZE_I_WAYS_SHIFT 8
/** @brief Assoziativity aka number of instruction ways = (value * 8) + 8 */
#define CACHE_L2C_310_L2CC_TYPE_NUM_I_WAYS_MASK 0x00000040
#define CACHE_L2C_310_L2CC_TYPE_NUM_I_WAYS_SHIFT 6
/** @brief Instruction cache line length 00 - 32 */
#define CACHE_L2C_310_L2CC_TYPE_LENGTH_I_LINE_MASK 0x00000003
#define CACHE_L2C_310_L2CC_TYPE_LENGTH_I_LINE_SHIFT 0
#define CACHE_L2C_310_L2CC_TYPE_LENGTH_I_LINE_VAL_32 0x0

  uint8_t reserved_8[0x100 - 8];
  uint32_t ctrl; /* Control */
/** @brief Enables the L2CC */
#define CACHE_L2C_310_L2CC_ENABLE_MASK 0x00000001

  /** @brief Auxiliary control */
  uint32_t aux_ctrl;

/** @brief Early BRESP Enable */
#define CACHE_L2C_310_L2CC_AUX_EBRESPE_MASK 0x40000000

/** @brief Instruction Prefetch Enable */
#define CACHE_L2C_310_L2CC_AUX_IPFE_MASK 0x20000000

/** @brief Data Prefetch Enable */
#define CACHE_L2C_310_L2CC_AUX_DPFE_MASK 0x10000000

/** @brief Non-secure interrupt access control */
#define CACHE_L2C_310_L2CC_AUX_NSIC_MASK 0x08000000

/** @brief Non-secure lockdown enable */
#define CACHE_L2C_310_L2CC_AUX_NSLE_MASK 0x04000000

/** @brief Cache replacement policy */
#define CACHE_L2C_310_L2CC_AUX_CRP_MASK 0x02000000

/** @brief Force write allocate */
#define CACHE_L2C_310_L2CC_AUX_FWE_MASK 0x01800000

/** @brief Shared attribute override enable */
#define CACHE_L2C_310_L2CC_AUX_SAOE_MASK 0x00400000

/** @brief Parity enable */
#define CACHE_L2C_310_L2CC_AUX_PE_MASK 0x00200000

/** @brief Event monitor bus enable */
#define CACHE_L2C_310_L2CC_AUX_EMBE_MASK 0x00100000

/** @brief Way-size */
#define CACHE_L2C_310_L2CC_AUX_WAY_SIZE_MASK 0x000E0000
#define CACHE_L2C_310_L2CC_AUX_WAY_SIZE_SHIFT 17

/** @brief Way-size */
#define CACHE_L2C_310_L2CC_AUX_ASSOC_MASK 0x00010000

/** @brief Shared attribute invalidate enable */
#define CACHE_L2C_310_L2CC_AUX_SAIE_MASK 0x00002000

/** @brief Exclusive cache configuration */
#define CACHE_L2C_310_L2CC_AUX_EXCL_CACHE_MASK 0x00001000

/** @brief Store buffer device limitation Enable */
#define CACHE_L2C_310_L2CC_AUX_SBDLE_MASK 0x00000800

/** @brief High Priority for SO and Dev Reads Enable */
#define CACHE_L2C_310_L2CC_AUX_HPSODRE_MASK 0x00000400

/** @brief Full line of zero enable */
#define CACHE_L2C_310_L2CC_AUX_FLZE_MASK 0x00000001

/** @brief Enable all prefetching, */
#define CACHE_L2C_310_L2CC_AUX_REG_DEFAULT_MASK \
  ( CACHE_L2C_310_L2CC_AUX_WAY_SIZE_MASK & ( 0x3 << CACHE_L2C_310_L2CC_AUX_WAY_SIZE_SHIFT ) ) \
  | CACHE_L2C_310_L2CC_AUX_PE_MASK      /* Prefetch enable */ \
  | CACHE_L2C_310_L2CC_AUX_SAOE_MASK    /* Shared attribute override enable */ \
  | CACHE_L2C_310_L2CC_AUX_CRP_MASK     /* Cache replacement policy */ \
  | CACHE_L2C_310_L2CC_AUX_DPFE_MASK    /* Data prefetch enable */ \
  | CACHE_L2C_310_L2CC_AUX_IPFE_MASK    /* Instruction prefetch enable */ \
  | CACHE_L2C_310_L2CC_AUX_EBRESPE_MASK /* Early BRESP enable */

#define CACHE_L2C_310_L2CC_AUX_REG_ZERO_MASK 0xFFF1FFFF

/** @brief 1 cycle of latency, there is no additional latency fot tag RAM */
#define CACHE_L2C_310_L2CC_RAM_1_CYCLE_LAT_VAL 0x00000000
/** @brief 2 cycles of latency for tag RAM */
#define CACHE_L2C_310_L2CC_RAM_2_CYCLE_LAT_VAL 0x00000001
/** @brief 3 cycles of latency for tag RAM */
#define CACHE_L2C_310_L2CC_RAM_3_CYCLE_LAT_VAL 0x00000002
/** @brief 4 cycles of latency for tag RAM */
#define CACHE_L2C_310_L2CC_RAM_4_CYCLE_LAT_VAL 0x00000003
/** @brief 5 cycles of latency for tag RAM */
#define CACHE_L2C_310_L2CC_RAM_5_CYCLE_LAT_VAL 0x00000004
/** @brief 6 cycles of latency for tag RAM */
#define CACHE_L2C_310_L2CC_RAM_6_CYCLE_LAT_VAL 0x00000005
/** @brief 7 cycles of latency for tag RAM */
#define CACHE_L2C_310_L2CC_RAM_7_CYCLE_LAT_VAL 0x00000006
/** @brief 8 cycles of latency for tag RAM */
#define CACHE_L2C_310_L2CC_RAM_8_CYCLE_LAT_VAL 0x00000007
/** @brief Shift left setup latency values by this value */
#define CACHE_L2C_310_L2CC_RAM_SETUP_SHIFT 0x00000000
/** @brief Shift left read latency values by this value */
#define CACHE_L2C_310_L2CC_RAM_READ_SHIFT 0x00000004
/** @brief Shift left write latency values by this value */
#define CACHE_L2C_310_L2CC_RAM_WRITE_SHIFT 0x00000008
/** @brief Mask for RAM setup latency */
#define CACHE_L2C_310_L2CC_RAM_SETUP_LAT_MASK 0x00000007
/** @brief Mask for RAM read latency */
#define CACHE_L2C_310_L2CC_RAM_READ_LAT_MASK 0x00000070
/** @brief Mask for RAM read latency */
#define CACHE_L2C_310_L2CC_RAM_WRITE_LAT_MASK 0x00000700
  /** @brief Latency for tag RAM */
  uint32_t tag_ram_ctrl;
/* @brief Latency for tag RAM */
#define CACHE_L2C_310_L2CC_TAG_RAM_DEFAULT_LAT \
  ( ( CACHE_L2C_310_L2CC_RAM_2_CYCLE_LAT_VAL << CACHE_L2C_310_L2CC_RAM_SETUP_SHIFT ) \
    | ( CACHE_L2C_310_L2CC_RAM_2_CYCLE_LAT_VAL << CACHE_L2C_310_L2CC_RAM_READ_SHIFT ) \
    | ( CACHE_L2C_310_L2CC_RAM_2_CYCLE_LAT_VAL << CACHE_L2C_310_L2CC_RAM_WRITE_SHIFT ) )
  /** @brief Latency for data RAM */
  uint32_t data_ram_ctrl;
/** @brief Latency for data RAM */
#define CACHE_L2C_310_L2CC_DATA_RAM_DEFAULT_MASK \
  ( ( CACHE_L2C_310_L2CC_RAM_2_CYCLE_LAT_VAL << CACHE_L2C_310_L2CC_RAM_SETUP_SHIFT ) \
    | ( CACHE_L2C_310_L2CC_RAM_3_CYCLE_LAT_VAL << CACHE_L2C_310_L2CC_RAM_READ_SHIFT ) \
    | ( CACHE_L2C_310_L2CC_RAM_2_CYCLE_LAT_VAL << CACHE_L2C_310_L2CC_RAM_WRITE_SHIFT ) )

  uint8_t reserved_110[0x200 - 0x110];

  /** @brief Event counter control */
  uint32_t ev_ctrl;

  /** @brief Event counter 1 configuration */
  uint32_t ev_cnt1_cfg;

  /** @brief Event counter 0 configuration */
  uint32_t ev_cnt0_cfg;

  /** @brief Event counter 1 value */
  uint32_t ev_cnt1;

  /** @brief Event counter 0 value */
  uint32_t ev_cnt0;

  /** @brief Interrupt enable mask */
  uint32_t int_mask;

  /** @brief Masked   interrupt status (read-only)*/
  uint32_t int_mask_status;

  /** @brief Unmasked interrupt status */
  uint32_t int_raw_status;

  /** @brief Interrupt clear */
  uint32_t int_clr;

/**
 * @name Interrupt bit masks
 *
 * @{
 */

/** @brief DECERR from L3 */
#define CACHE_L2C_310_L2CC_INT_DECERR_MASK 0x00000100

/** @brief SLVERR from L3 */
#define CACHE_L2C_310_L2CC_INT_SLVERR_MASK 0x00000080

/** @brief Error on L2 data RAM (Read) */
#define CACHE_L2C_310_L2CC_INT_ERRRD_MASK 0x00000040

/** @brief Error on L2 tag RAM (Read) */
#define CACHE_L2C_310_L2CC_INT_ERRRT_MASK 0x00000020

/** @brief Error on L2 data RAM (Write) */
#define CACHE_L2C_310_L2CC_INT_ERRWD_MASK 0x00000010

/** @brief Error on L2 tag RAM (Write) */
#define CACHE_L2C_310_L2CC_INT_ERRWT_MASK 0x00000008

/** @brief Parity Error on L2 data RAM (Read) */
#define CACHE_L2C_310_L2CC_INT_PARRD_MASK 0x00000004

/** @brief Parity Error on L2 tag RAM (Read) */
#define CACHE_L2C_310_L2CC_INT_PARRT_MASK 0x00000002

/** @brief Event Counter1/0 Overflow Increment */
#define CACHE_L2C_310_L2CC_INT_ECNTR_MASK 0x00000001

/** @} */

  uint8_t reserved_224[0x730 - 0x224];

  /** @brief Drain the STB */
  uint32_t cache_sync;
  uint8_t reserved_734[0x740 - 0x734];
  /** @brief ARM Errata 753970 for pl310-r3p0 */
  uint32_t dummy_cache_sync_reg;
  uint8_t reserved_744[0x770 - 0x744];

  /** @brief Invalidate line by PA */
  uint32_t inv_pa;
  uint8_t reserved_774[0x77c - 0x774];

  /** @brief Invalidate by Way */
  uint32_t inv_way;
  uint8_t reserved_780[0x7b0 - 0x780];

  /** @brief Clean Line by PA */
  uint32_t clean_pa;
  uint8_t reserved_7b4[0x7b8 - 0x7b4];

  /** @brief Clean Line by Set/Way */
  uint32_t clean_index;

  /** @brief Clean by Way */
  uint32_t clean_way;
  uint8_t reserved_7c0[0x7f0 - 0x7c0];

  /** @brief Clean and Invalidate Line by PA */
  uint32_t clean_inv_pa;
  uint8_t reserved_7f4[0x7f8 - 0x7f4];

  /** @brief Clean and Invalidate Line by Set/Way */
  uint32_t clean_inv_indx;

  /** @brief Clean and Invalidate by Way */
  uint32_t clean_inv_way;

  /** @brief Data        lock down 0 */
  uint32_t d_lockdown_0;

  /** @brief Instruction lock down 0 */
  uint32_t i_lockdown_0;

  /** @brief Data        lock down 1 */
  uint32_t d_lockdown_1;

  /** @brief Instruction lock down 1 */
  uint32_t i_lockdown_1;

  /** @brief Data        lock down 2 */
  uint32_t d_lockdown_2;

  /** @brief Instruction lock down 2 */
  uint32_t i_lockdown_2;

  /** @brief Data        lock down 3 */
  uint32_t d_lockdown_3;

  /** @brief Instruction lock down 3 */
  uint32_t i_lockdown_3;

  /** @brief Data        lock down 4 */
  uint32_t d_lockdown_4;

  /** @brief Instruction lock down 4 */
  uint32_t i_lockdown_4;

  /** @brief Data        lock down 5 */
  uint32_t d_lockdown_5;

  /** @brief Instruction lock down 5 */
  uint32_t i_lockdown_5;

  /** @brief Data        lock down 6 */
  uint32_t d_lockdown_6;

  /** @brief Instruction lock down 6 */
  uint32_t i_lockdown_6;

  /** @brief Data        lock down 7 */
  uint32_t d_lockdown_7;

  /** @brief Instruction lock down 7 */
  uint32_t i_lockdown_7;

  uint8_t reserved_940[0x950 - 0x940];

  /** @brief Lockdown by Line Enable */
  uint32_t lock_line_en;

  /** @brief Cache lockdown by way */
  uint32_t unlock_way;

  uint8_t reserved_958[0xc00 - 0x958];

  /** @brief Address range redirect, part 1 */
  uint32_t addr_filtering_start;

  /** @brief Address range redirect, part 2 */
  uint32_t addr_filtering_end;

/** @brief Address filtering valid bits*/
#define CACHE_L2C_310_L2CC_ADDR_FILTER_VALID_MASK 0xFFF00000

/** @brief Address filtering enable bit*/
#define CACHE_L2C_310_L2CC_ADDR_FILTER_ENABLE_MASK 0x00000001

  uint8_t reserved_c08[0xf40 - 0xc08];

  /** @brief Debug control */
  uint32_t debug_ctrl;

/** @brief Debug SPIDEN bit */
#define CACHE_L2C_310_L2CC_DEBUG_SPIDEN_MASK 0x00000004

/** @brief Debug DWB bit, forces write through */
#define CACHE_L2C_310_L2CC_DEBUG_DWB_MASK 0x00000002

/** @brief Debug DCL bit, disables cache line fill */
#define CACHE_L2C_310_L2CC_DEBUG_DCL_MASK 0x00000002

  uint8_t reserved_f44[0xf60 - 0xf44];

  /** @brief Purpose prefetch enables */
  uint32_t prefetch_ctrl;
/** @brief Prefetch offset */
#define CACHE_L2C_310_L2CC_PREFETCH_OFFSET_MASK 0x0000001F
  uint8_t reserved_f64[0xf80 - 0xf64];

  /** @brief Purpose power controls */
  uint32_t power_ctrl;
} L2CC;

/* Errata table for the LC2 310 Level 2 cache from ARM.
* Information taken from ARMs
* "CoreLink controllers and peripherals
* - System controllers
* - L2C-310 Level 2 Cache Controller
* - Revision r3p3
* - Software Developer Errata Notice
* - ARM CoreLink Level 2 Cache Controller (L2C-310 or PL310), 
*   r3 releases Software Developers Errata Notice"
* The corresponding link is: 
* http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0360f/BABJFIBA.html
* Please see this document for more information on these erratas */
static bool l2c_310_cache_errata_is_applicable_753970(
  void
)
{
  volatile L2CC                  *l2cc          = 
    (volatile L2CC *) BSP_ARM_L2CC_BASE;
  const cache_l2c_310_rtl_release RTL_RELEASE   = 
    l2cc->cache_id & CACHE_L2C_310_L2CC_ID_RTL_MASK;
  bool                            is_applicable = false;
  
  switch( RTL_RELEASE ) {
    case CACHE_L2C_310_RTL_RELEASE_R3_P3:
    case CACHE_L2C_310_RTL_RELEASE_R3_P2:
    case CACHE_L2C_310_RTL_RELEASE_R3_P1:
    case CACHE_L2C_310_RTL_RELEASE_R2_P0:
    case CACHE_L2C_310_RTL_RELEASE_R1_P0:
    case CACHE_L2C_310_RTL_RELEASE_R0_P0:
      is_applicable = false;
    break;
    case CACHE_L2C_310_RTL_RELEASE_R3_P0:
      is_applicable = true;
    break;
    default:
       assert(   RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P3
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P2
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P1
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R2_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R1_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R0_P0 );
     break;
  }
  
  return is_applicable;
}



static bool l2c_310_cache_errata_is_applicable_727913(
  void
)
{
  volatile L2CC                  *l2cc          = 
    (volatile L2CC *) BSP_ARM_L2CC_BASE;
  const cache_l2c_310_rtl_release RTL_RELEASE   = 
    l2cc->cache_id & CACHE_L2C_310_L2CC_ID_RTL_MASK;
  bool                            is_applicable = false;
  
  switch( RTL_RELEASE ) {
    case CACHE_L2C_310_RTL_RELEASE_R3_P3:
    case CACHE_L2C_310_RTL_RELEASE_R3_P2:
    case CACHE_L2C_310_RTL_RELEASE_R3_P1:
    case CACHE_L2C_310_RTL_RELEASE_R2_P0:
    case CACHE_L2C_310_RTL_RELEASE_R1_P0:
    case CACHE_L2C_310_RTL_RELEASE_R0_P0:
      is_applicable = false;
    break;
    case CACHE_L2C_310_RTL_RELEASE_R3_P0:
      is_applicable = true;
    break;
    default:
       assert(   RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P3
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P2
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P1
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R2_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R1_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R0_P0 );
     break;
  }
  
  return is_applicable;
}

static bool l2c_310_cache_errata_is_applicable_727914(
  void
)
{
  volatile L2CC                  *l2cc          = 
    (volatile L2CC *) BSP_ARM_L2CC_BASE;
  const cache_l2c_310_rtl_release RTL_RELEASE   = 
    l2cc->cache_id & CACHE_L2C_310_L2CC_ID_RTL_MASK;
  bool                            is_applicable = false;
  
  switch( RTL_RELEASE ) {
    case CACHE_L2C_310_RTL_RELEASE_R3_P3:
    case CACHE_L2C_310_RTL_RELEASE_R3_P2:
    case CACHE_L2C_310_RTL_RELEASE_R3_P1:
    case CACHE_L2C_310_RTL_RELEASE_R2_P0:
    case CACHE_L2C_310_RTL_RELEASE_R1_P0:
    case CACHE_L2C_310_RTL_RELEASE_R0_P0:
      is_applicable = false;
    break;
    case CACHE_L2C_310_RTL_RELEASE_R3_P0:
      is_applicable = true;
    break;
    default:
       assert(   RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P3
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P2
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P1
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R2_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R1_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R0_P0 );
     break;
  }
  
  return is_applicable;
}

static bool l2c_310_cache_errata_is_applicable_727915(
  void
)
{
  volatile L2CC                  *l2cc          = 
    (volatile L2CC *) BSP_ARM_L2CC_BASE;
  const cache_l2c_310_rtl_release RTL_RELEASE   = 
    l2cc->cache_id & CACHE_L2C_310_L2CC_ID_RTL_MASK;
  bool                            is_applicable = false;
  
  switch( RTL_RELEASE ) {
    case CACHE_L2C_310_RTL_RELEASE_R3_P3:
    case CACHE_L2C_310_RTL_RELEASE_R3_P2:
    case CACHE_L2C_310_RTL_RELEASE_R3_P1:
    case CACHE_L2C_310_RTL_RELEASE_R1_P0:
    case CACHE_L2C_310_RTL_RELEASE_R0_P0:
      is_applicable = false;
    break;
    case CACHE_L2C_310_RTL_RELEASE_R3_P0:
    case CACHE_L2C_310_RTL_RELEASE_R2_P0:
      is_applicable = true;
    break;
    default:
       assert(   RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P3
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P2
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P1
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R2_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R1_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R0_P0 );
     break;
  }
  
  return is_applicable;
}

static bool l2c_310_cache_errata_is_applicable_729806(
  void
)
{
  volatile L2CC                  *l2cc          = 
    (volatile L2CC *) BSP_ARM_L2CC_BASE;
  const cache_l2c_310_rtl_release RTL_RELEASE   = 
    l2cc->cache_id & CACHE_L2C_310_L2CC_ID_RTL_MASK;
  bool                            is_applicable = false;
  
  switch( RTL_RELEASE ) {
    case CACHE_L2C_310_RTL_RELEASE_R3_P3:
    case CACHE_L2C_310_RTL_RELEASE_R3_P2:
    case CACHE_L2C_310_RTL_RELEASE_R2_P0:
    case CACHE_L2C_310_RTL_RELEASE_R1_P0:
    case CACHE_L2C_310_RTL_RELEASE_R0_P0:
      is_applicable = false;
    break;
    case CACHE_L2C_310_RTL_RELEASE_R3_P1:
    case CACHE_L2C_310_RTL_RELEASE_R3_P0:
      is_applicable = true;
    break;
    default:
       assert(   RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P3
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P2
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P1
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R2_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R1_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R0_P0 );
     break;
  }
  
  return is_applicable;
}

static bool l2c_310_cache_errata_is_applicable_729815(
  void
)
{
  volatile L2CC                  *l2cc          = 
    (volatile L2CC *) BSP_ARM_L2CC_BASE;
  const cache_l2c_310_rtl_release RTL_RELEASE   = 
    l2cc->cache_id & CACHE_L2C_310_L2CC_ID_RTL_MASK;
  bool                            is_applicable = false;
  
  switch( RTL_RELEASE ) {
    case CACHE_L2C_310_RTL_RELEASE_R3_P3:
    case CACHE_L2C_310_RTL_RELEASE_R1_P0:
    case CACHE_L2C_310_RTL_RELEASE_R0_P0:
      is_applicable = false;
    break;
    case CACHE_L2C_310_RTL_RELEASE_R3_P2:
    case CACHE_L2C_310_RTL_RELEASE_R3_P1:
    case CACHE_L2C_310_RTL_RELEASE_R3_P0:
    case CACHE_L2C_310_RTL_RELEASE_R2_P0:
      is_applicable = true;
    break;
    default:
       assert(   RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P3
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P2
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P1
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R2_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R1_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R0_P0 );
     break;
  }
  
  return is_applicable;
}

static bool l2c_310_cache_errata_is_applicable_742884(
  void
)
{
  volatile L2CC                  *l2cc          = 
    (volatile L2CC *) BSP_ARM_L2CC_BASE;
  const cache_l2c_310_rtl_release RTL_RELEASE   = 
    l2cc->cache_id & CACHE_L2C_310_L2CC_ID_RTL_MASK;
  bool                            is_applicable = false;
  
  switch( RTL_RELEASE ) {
    case CACHE_L2C_310_RTL_RELEASE_R3_P3:
    case CACHE_L2C_310_RTL_RELEASE_R3_P2:
    case CACHE_L2C_310_RTL_RELEASE_R3_P0:
    case CACHE_L2C_310_RTL_RELEASE_R2_P0:
    case CACHE_L2C_310_RTL_RELEASE_R1_P0:
    case CACHE_L2C_310_RTL_RELEASE_R0_P0:
      is_applicable = false;
    break;
    case CACHE_L2C_310_RTL_RELEASE_R3_P1:
      is_applicable = true;
    break;
    default:
       assert(   RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P3
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P2
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P1
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R2_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R1_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R0_P0 );
     break;
  }
  
  return is_applicable;
}

static bool l2c_310_cache_errata_is_applicable_752271(
  void
)
{
  volatile L2CC                  *l2cc          = 
    (volatile L2CC *) BSP_ARM_L2CC_BASE;
  const cache_l2c_310_rtl_release RTL_RELEASE   = 
    l2cc->cache_id & CACHE_L2C_310_L2CC_ID_RTL_MASK;
  bool                            is_applicable = false;
  
  switch( RTL_RELEASE ) {
    case CACHE_L2C_310_RTL_RELEASE_R3_P3:
    case CACHE_L2C_310_RTL_RELEASE_R3_P2:
    case CACHE_L2C_310_RTL_RELEASE_R2_P0:
    case CACHE_L2C_310_RTL_RELEASE_R1_P0:
    case CACHE_L2C_310_RTL_RELEASE_R0_P0:
      is_applicable = false;
    break;
    case CACHE_L2C_310_RTL_RELEASE_R3_P1:
    case CACHE_L2C_310_RTL_RELEASE_R3_P0:
      is_applicable = true;
    break;
    default:
       assert(   RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P3
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P2
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P1
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R2_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R1_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R0_P0 );
     break;
  }
  
  return is_applicable;
}

static bool l2c_310_cache_errata_is_applicable_765569(
  void
)
{
  volatile L2CC                  *l2cc          = 
    (volatile L2CC *) BSP_ARM_L2CC_BASE;
  const cache_l2c_310_rtl_release RTL_RELEASE   = 
    l2cc->cache_id & CACHE_L2C_310_L2CC_ID_RTL_MASK;
  bool                            is_applicable = false;
  
  switch( RTL_RELEASE ) {
    case CACHE_L2C_310_RTL_RELEASE_R3_P3:
    case CACHE_L2C_310_RTL_RELEASE_R3_P2:
    case CACHE_L2C_310_RTL_RELEASE_R3_P1:
    case CACHE_L2C_310_RTL_RELEASE_R3_P0:
    case CACHE_L2C_310_RTL_RELEASE_R2_P0:
    case CACHE_L2C_310_RTL_RELEASE_R1_P0:
    case CACHE_L2C_310_RTL_RELEASE_R0_P0:
      is_applicable = true;
    break;
    default:
       assert(   RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P3
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P2
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P1
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R2_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R1_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R0_P0 );
     break;
  }
  
  return is_applicable;
}

static bool l2c_310_cache_errata_is_applicable_769419(
  void
)
{
  volatile L2CC                  *l2cc          = 
    (volatile L2CC *) BSP_ARM_L2CC_BASE;
  const cache_l2c_310_rtl_release RTL_RELEASE   = 
    l2cc->cache_id & CACHE_L2C_310_L2CC_ID_RTL_MASK;
  bool                            is_applicable = false;
  
  switch( RTL_RELEASE ) {
    case CACHE_L2C_310_RTL_RELEASE_R3_P3:
    case CACHE_L2C_310_RTL_RELEASE_R3_P2:
      is_applicable = false;
    break;
    case CACHE_L2C_310_RTL_RELEASE_R3_P1:
    case CACHE_L2C_310_RTL_RELEASE_R3_P0:
    case CACHE_L2C_310_RTL_RELEASE_R2_P0:
    case CACHE_L2C_310_RTL_RELEASE_R1_P0:
    case CACHE_L2C_310_RTL_RELEASE_R0_P0:
      is_applicable = true;
    break;
    default:
       assert(   RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P3
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P2
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P1
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R2_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R1_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R0_P0 );
     break;
  }
  
  return is_applicable;
}

static bool l2c_310_cache_errata_is_applicable_588369(
  void
)
{
  volatile L2CC                  *l2cc          = 
    (volatile L2CC *) BSP_ARM_L2CC_BASE;
  const cache_l2c_310_rtl_release RTL_RELEASE   = 
    l2cc->cache_id & CACHE_L2C_310_L2CC_ID_RTL_MASK;
  bool                            is_applicable = false;
  
  switch( RTL_RELEASE ) {
    case CACHE_L2C_310_RTL_RELEASE_R3_P3:
    case CACHE_L2C_310_RTL_RELEASE_R3_P2:
    case CACHE_L2C_310_RTL_RELEASE_R3_P1:
    case CACHE_L2C_310_RTL_RELEASE_R3_P0:
    case CACHE_L2C_310_RTL_RELEASE_R2_P0:
      is_applicable = false;
    break;
    case CACHE_L2C_310_RTL_RELEASE_R1_P0:
    case CACHE_L2C_310_RTL_RELEASE_R0_P0:
      is_applicable = true;
    break;
    default:
       assert(   RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P3
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P2
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P1
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R2_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R1_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R0_P0 );
     break;
  }
  
  return is_applicable;
}

#ifdef CACHE_ERRATA_CHECKS_FOR_IMPLEMENTED_ERRATAS
static bool l2c_310_cache_errata_is_applicable_754670(
  void
)
{
  volatile L2CC                  *l2cc          = 
    (volatile L2CC *) BSP_ARM_L2CC_BASE;
  const cache_l2c_310_rtl_release RTL_RELEASE   = 
    l2cc->cache_id & CACHE_L2C_310_L2CC_ID_RTL_MASK;
  bool                            is_applicable = false;
  
  switch( RTL_RELEASE ) {
    case CACHE_L2C_310_RTL_RELEASE_R3_P3:
    case CACHE_L2C_310_RTL_RELEASE_R3_P2:
    case CACHE_L2C_310_RTL_RELEASE_R3_P1:
    case CACHE_L2C_310_RTL_RELEASE_R3_P0:
    case CACHE_L2C_310_RTL_RELEASE_R2_P0:
    case CACHE_L2C_310_RTL_RELEASE_R1_P0:
    case CACHE_L2C_310_RTL_RELEASE_R0_P0:
      is_applicable = true;
    break;
    default:
       assert(   RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P3
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P2
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P1
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R2_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R1_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R0_P0 );
     break;
  }
  
  return is_applicable;
}
#endif /* CACHE_ERRATA_CHECKS_FOR_IMPLEMENTED_ERRATAS */

/* Errata Handlers */
#if ( defined( RTEMS_SMP ) )
  #define CACHE_ARM_ERRATA_764369_HANDLER()                    \
    if( arm_errata_is_applicable_processor_errata_764369() ) { \
      _ARM_Data_synchronization_barrier();                     \
    }                                           
#else /* #if ( defined( RTEMS_SMP ) ) */
  #define CACHE_ARM_ERRATA_764369_HANDLER()
#endif /* #if ( defined( RTEMS_SMP ) ) */

/* The common workaround for this erratum would be to add a 
 * data synchronization barrier to the beginning of the abort handler.
 * But for RTEMS a call of the abort handler means a fatal condition anyway. 
 * So there is no need to handle this erratum */
#define CACHE_ARM_ERRATA_775420_HANDLER()                   \
  if( arm_errata_is_applicable_processor_errata_775420 ) {  \
  }                                                         \

static void l2c_310_cache_check_errata( void )
{
  /* This erratum gets handled within the sources */
  /* Unhandled erratum present: 588369 Errata 588369 says that clean + inv may 
   * keep the cache line if it was clean. See ARMs documentation on the erratum
   * for a workaround */
  /* assert( ! l2c_310_cache_errata_is_applicable_588369() ); */

  /* Unhandled erratum present: 727913 Prefetch dropping feature can cause 
   * incorrect behavior when PL310 handles reads that cross cache line 
   * boundary */
  assert( ! l2c_310_cache_errata_is_applicable_727913() );

  /* Unhandled erratum present: 727914 Double linefill feature can cause 
   * deadlock */
  assert( ! l2c_310_cache_errata_is_applicable_727914() );
  
  /* Unhandled erratum present: 727915 Background Clean and Invalidate by Way 
   * operation can cause data corruption */
  assert( ! l2c_310_cache_errata_is_applicable_727915() );

  /* Unhandled erratum present: 729806 Speculative reads from the Cortex-A9 
   * MPCore processor can cause deadlock */
  assert( ! l2c_310_cache_errata_is_applicable_729806() );

  if( l2c_310_cache_errata_is_applicable_729815() )
  {
    volatile L2CC *l2cc = (volatile L2CC *) BSP_ARM_L2CC_BASE;

    assert( 0 == ( l2cc->aux_ctrl & CACHE_L2C_310_L2CC_AUX_HPSODRE_MASK ) );

    /* Erratum: 729815 The “High Priority for SO and Dev reads” feature can 
     * cause Quality of Service issues to cacheable read transactions*/

    /* Conditions
       This problem occurs when the following conditions are met:
       1. Bit[10] “High Priority for SO and Dev reads enable” of the PL310 
          Auxiliary Control Register is set to 1.
       2. PL310 receives a cacheable read that misses in the L2 cache.
       3. PL310 receives a continuous flow of Strongly Ordered or Device 
          reads that take all address slots in the master interface.
       Workaround
       A workaround is only necessary in systems that are able to issue a 
       continuous flow of Strongly Ordered or Device reads. In such a case, 
       the workaround is to disable the “High Priority for SO and Dev reads” 
       feature. This is the default behavior.*/
  }
  
  /* Unhandled erratum present: 742884 Double linefill feature might introduce 
   * circular dependency and deadlock */
  assert( ! l2c_310_cache_errata_is_applicable_742884() );

  /* Unhandled erratum present: 752271 Double linefill feature can cause data 
   * corruption */
  assert( ! l2c_310_cache_errata_is_applicable_752271() );

  /* This erratum gets handled with a workaround: 753970 The Cache Sync 
   * operation prevents further bufferable writes from merging in the store.
     Search for 753970 in cache_.h for detailed information */

  /* Conditions
     This problem occurs when the following conditions are met:
     1. PL310 receives a Cache Sync operation.
     Workaround
     The proposed workaround to avoid this erratum is to replace the normal 
     offset of Cache Sync operation (0x730) by another offset targeting an 
     unmapped PL310 register: 0x740.
     More specifically, find below a pseudo code sequence illustrating the 
     workaround:
     Replace
     // PL310 Cache Sync operation
     LDR r1,=PL310_BASE
     STR r2,[r1,#0x730]
     by
     // Workaround for PL310 Cache Sync operation
     LDR r1,=PL310_BASE
     STR r2,[r1,#0x740] ; write to an unmapped register
     This write has the same effect as the Cache Sync operation: store buffer 
     drained and waiting for all buffers empty.*/
  /* assert( ! l2c_310_cache_errata_is_applicable_753970() ); */

  /* This erratum can not be worked around: 754670 A continuous write flow can 
   * stall a read targeting the same memory area
   * But this erratum does not lead to any data corruption */
  /* assert( ! l2c_310_cache_errata_is_applicable_754670() ); */

  if( l2c_310_cache_errata_is_applicable_765569() )
  {
    volatile L2CC *l2cc = (volatile L2CC *) BSP_ARM_L2CC_BASE;

    assert( !( ( l2cc->aux_ctrl & CACHE_L2C_310_L2CC_AUX_IPFE_MASK
                 || l2cc->aux_ctrl & CACHE_L2C_310_L2CC_AUX_DPFE_MASK )
               && ( ( l2cc->prefetch_ctrl & CACHE_L2C_310_L2CC_PREFETCH_OFFSET_MASK )
                    == 23 ) ) );

    /* Unhandled erratum present: 765569 Prefetcher can cross 4KB boundary if 
     * offset is programmed with value 23 */

    /* Conditions
       This problem occurs when the following conditions are met:
       1. One of the Prefetch Enable bits (bits [29:28] of the Auxiliary or 
          Prefetch Control Register) is set HIGH.
       2. The prefetch offset bits are programmed with value 23 (5'b10111).
       Workaround
       A workaround for this erratum is to program the prefetch offset with any
       value except 23.*/
  }

  /* Unhandled erratum present: 769419 No automatic Store Buffer drain, 
   * visibility of written data requires an explicit Cache */
  assert( ! l2c_310_cache_errata_is_applicable_769419() );
}

static inline void
cache_l2c_310_sync( void )
{
  volatile L2CC *l2cc = (volatile L2CC *) BSP_ARM_L2CC_BASE;

  if( l2c_310_cache_errata_is_applicable_753970() ) {
    l2cc->dummy_cache_sync_reg = 0;
  } else {
    l2cc->cache_sync           = 0;
  }
}

static inline void
cache_l2c_310_flush_1_line( const void *d_addr )
{
  volatile L2CC *l2cc = (volatile L2CC *) BSP_ARM_L2CC_BASE;


  if( l2c_310_cache_errata_is_applicable_588369() ) {
    /*
    * Errata 588369 says that clean + inv may keep the
    * cache line if it was clean, the recommanded
    * workaround is to clean then invalidate the cache
    * line, with write-back and cache linefill disabled.
    */
    l2cc->clean_pa     = (uint32_t) d_addr;
    cache_l2c_310_sync();
    l2cc->inv_pa       = (uint32_t) d_addr;
  } else {
    l2cc->clean_inv_pa = (uint32_t) d_addr;
  }

  cache_l2c_310_sync();
}

static inline void
cache_l2c_310_flush_range( const void *addr, size_t n_bytes )
{
  if ( n_bytes != 0 ) {
    uint32_t       adx       = (uint32_t) addr
                               & ~CACHE_L2C_310_DATA_LINE_MASK;
    const uint32_t ADDR_LAST =
      ( (uint32_t) addr + n_bytes - 1 ) & ~CACHE_L2C_310_DATA_LINE_MASK;
    volatile L2CC *l2cc      = (volatile L2CC *) BSP_ARM_L2CC_BASE;

    CACHE_ARM_ERRATA_764369_HANDLER();

    for (; adx <= ADDR_LAST; adx += CPU_DATA_CACHE_ALIGNMENT ) {
      l2cc->clean_pa = adx;
    }
    cache_l2c_310_sync();
  }
}

static inline void
cache_l2c_310_flush_entire( void )
{
  volatile L2CC *l2cc = (volatile L2CC *) BSP_ARM_L2CC_BASE;

  /* Only flush if level 2 cache is active */
  if( ( l2cc->ctrl & CACHE_L2C_310_L2CC_ENABLE_MASK ) != 0 ) {

    /* ensure ordering with previous memory accesses */
    _ARM_Data_memory_barrier();

    l2cc->clean_inv_way = CACHE_l2C_310_WAY_MASK;

    while ( l2cc->clean_inv_way & CACHE_l2C_310_WAY_MASK ) {
    }

    ;

    /* Wait for the flush to complete */
    cache_l2c_310_sync();
  }
}

static inline void
cache_l2c_310_invalidate_1_line( const void *d_addr )
{
  volatile L2CC *l2cc = (volatile L2CC *) BSP_ARM_L2CC_BASE;


  l2cc->inv_pa = (uint32_t) d_addr;

  cache_l2c_310_sync();
}

static inline void
cache_l2c_310_invalidate_range( const void *addr, size_t n_bytes )
{
  if ( n_bytes != 0 ) {
    uint32_t       adx  = (uint32_t) addr
                         & ~CACHE_L2C_310_INSTRUCTION_LINE_MASK;
    const uint32_t end  =
      ( adx + n_bytes ) & ~CACHE_L2C_310_INSTRUCTION_LINE_MASK;
    volatile L2CC *l2cc = (volatile L2CC *) BSP_ARM_L2CC_BASE;

    /* Back starting address up to start of a line and invalidate until end */
    for (;
         adx < end;
         adx += CPU_INSTRUCTION_CACHE_ALIGNMENT ) {
      /* Invalidate L2 cache line */
      l2cc->inv_pa = adx;
    }
    cache_l2c_310_sync();
  }
}

static inline void
cache_l2c_310_invalidate_entire( void )
{
  volatile L2CC *l2cc = (volatile L2CC *) BSP_ARM_L2CC_BASE;

  /* Invalidate the caches */

  /* ensure ordering with previous memory accesses */
  _ARM_Data_memory_barrier();

  l2cc->inv_way = CACHE_l2C_310_WAY_MASK;

  while ( l2cc->inv_way & CACHE_l2C_310_WAY_MASK ) ;

  /* Wait for the invalidate to complete */
  cache_l2c_310_sync();
}

static inline void
cache_l2c_310_clean_and_invalidate_entire( void )
{
  volatile L2CC *l2cc = (volatile L2CC *) BSP_ARM_L2CC_BASE;


  if( ( l2cc->ctrl & CACHE_L2C_310_L2CC_ENABLE_MASK ) != 0 ) {
    /* Invalidate the caches */

    /* ensure ordering with previous memory accesses */
    _ARM_Data_memory_barrier();

    l2cc->clean_inv_way = CACHE_l2C_310_WAY_MASK;

    while ( l2cc->clean_inv_way & CACHE_l2C_310_WAY_MASK ) ;

    /* Wait for the invalidate to complete */
    cache_l2c_310_sync();
  }
}

static inline void
cache_l2c_310_store( const void *d_addr )
{
  volatile L2CC *l2cc = (volatile L2CC *) BSP_ARM_L2CC_BASE;


  l2cc->clean_pa = (uint32_t) d_addr;

  cache_l2c_310_sync();
}

static inline void
cache_l2c_310_freeze( void )
{
  /* To be implemented as needed, if supported
   by hardware at all */
}

static inline void
cache_l2c_310_unfreeze( void )
{
  /* To be implemented as needed, if supported
   by hardware at all */
}

static void cache_l2c_310_unlock( void )
{
  volatile L2CC *l2cc = (volatile L2CC *) BSP_ARM_L2CC_BASE;


  l2cc->d_lockdown_0 = 0;
  l2cc->i_lockdown_0 = 0;
  l2cc->d_lockdown_1 = 0;
  l2cc->i_lockdown_1 = 0;
  l2cc->d_lockdown_2 = 0;
  l2cc->i_lockdown_2 = 0;
  l2cc->d_lockdown_3 = 0;
  l2cc->i_lockdown_3 = 0;
  l2cc->d_lockdown_4 = 0;
  l2cc->i_lockdown_4 = 0;
  l2cc->d_lockdown_5 = 0;
  l2cc->i_lockdown_5 = 0;
  l2cc->d_lockdown_6 = 0;
  l2cc->i_lockdown_6 = 0;
  l2cc->d_lockdown_7 = 0;
  l2cc->i_lockdown_7 = 0;
}

static inline void
cache_l2c_310_enable( void )
{
  volatile L2CC *l2cc     = (volatile L2CC *) BSP_ARM_L2CC_BASE;
  uint32_t       cache_id = l2cc->cache_id & CACHE_L2C_310_L2CC_ID_PART_MASK;
  int            ways     = 0;


  /* Do we actually have an L2C-310 cache controller?
   * Has BSP_ARM_L2CC_BASE been configured correctly? */
  switch ( cache_id ) {
    case CACHE_L2C_310_L2CC_ID_PART_L310:
    {
      const cache_l2c_310_rtl_release RTL_RELEASE = 
        l2cc->cache_id & CACHE_L2C_310_L2CC_ID_RTL_MASK;
      /* If this assertion fails, you have a release of the
       * L2C-310 cache for which the l2c_310_cache_errata_is_applicable_ ...
       * methods are not yet implemented. This means you will get incorrect
       * errata handling */
      assert(    RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P3
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P2
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P1
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R3_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R2_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R1_P0
              || RTL_RELEASE == CACHE_L2C_310_RTL_RELEASE_R0_P0 );
      if ( l2cc->aux_ctrl & ( 1 << 16 ) ) {
        ways = 16;
      } else {
        ways = 8;
      }

      assert( ways == CACHE_l2C_310_NUM_WAYS );
    }
    break;
    case CACHE_L2C_310_L2CC_ID_PART_L210:

      /* Invalid case */

      /* Support for this type is not implemented in this driver.
       * Either support needs to get added or a seperate driver needs to get
       * implemented */
      assert( cache_id != CACHE_L2C_310_L2CC_ID_PART_L210 );
      break;
    default:

      /* Unknown case */
      assert( cache_id == CACHE_L2C_310_L2CC_ID_PART_L310 );
      break;
  }

  if ( ways > 0 ) {
    /* Only enable if L2CC is currently disabled */    
    if ( ways != 0
         && ( l2cc->ctrl & CACHE_L2C_310_L2CC_ENABLE_MASK ) == 0 ) {
      rtems_interrupt_level level;
      uint32_t              aux;

      rtems_interrupt_disable( level );

      /* Set up the way size */
      aux  = l2cc->aux_ctrl;
      aux &= CACHE_L2C_310_L2CC_AUX_REG_ZERO_MASK; /* Set way_size to 0 */
      aux |= CACHE_L2C_310_L2CC_AUX_REG_DEFAULT_MASK;

      /* Make sure that I&D is not locked down when starting */
      cache_l2c_310_unlock();

      /* Level 2 configuration and control registers must not get written while
       * background operations are pending */
      while ( l2cc->inv_way & CACHE_l2C_310_WAY_MASK ) ;

      while ( l2cc->clean_way & CACHE_l2C_310_WAY_MASK ) ;

      while ( l2cc->clean_inv_way & CACHE_l2C_310_WAY_MASK ) ;

      l2cc->aux_ctrl = aux;

      /* Set up the latencies */
      l2cc->tag_ram_ctrl  = CACHE_L2C_310_L2CC_TAG_RAM_DEFAULT_LAT;
      l2cc->data_ram_ctrl = CACHE_L2C_310_L2CC_DATA_RAM_DEFAULT_MASK;

      cache_l2c_310_invalidate_entire();

      /* Clear the pending interrupts */
      l2cc->int_clr = l2cc->int_raw_status;

      l2c_310_cache_check_errata();

      /* Enable the L2CC */
      l2cc->ctrl |= CACHE_L2C_310_L2CC_ENABLE_MASK;

      rtems_interrupt_enable( level );
    }
  }
}

static inline void 
cache_l2c_310_disable( void )
{
  volatile L2CC *l2cc = (volatile L2CC *) BSP_ARM_L2CC_BASE;


  if ( l2cc->ctrl & CACHE_L2C_310_L2CC_ENABLE_MASK ) {
    /* Clean and Invalidate L2 Cache */
    cache_l2c_310_flush_entire();

    /* Level 2 configuration and control registers must not get written while
     * background operations are pending */
    while ( l2cc->inv_way & CACHE_l2C_310_WAY_MASK ) ;

    while ( l2cc->clean_way & CACHE_l2C_310_WAY_MASK ) ;

    while ( l2cc->clean_inv_way & CACHE_l2C_310_WAY_MASK ) ;

    /* Disable the L2 cache */
    l2cc->ctrl &= ~CACHE_L2C_310_L2CC_ENABLE_MASK;
  }
}

static inline void 
_CPU_cache_enable_data( void )
{
  cache_l2c_310_enable();
  arm_cache_l1_enable_data();
}

static inline void 
_CPU_cache_disable_data( void )
{
  arm_cache_l1_disable_data();
  cache_l2c_310_disable();
}

static inline void 
_CPU_cache_enable_instruction( void )
{
  cache_l2c_310_enable();
  arm_cache_l1_enable_instruction();
}

static inline void 
_CPU_cache_disable_instruction( void )
{
  arm_cache_l1_disable_instruction();
  cache_l2c_310_disable();
}

static inline void 
_CPU_cache_flush_data_range(
  const void *d_addr,
  size_t      n_bytes
)
{
  if ( n_bytes != 0 ) {
    arm_cache_l1_flush_data_range( 
      d_addr,
      n_bytes
    );
    cache_l2c_310_flush_range(
      d_addr,
      n_bytes
    );
  }
}

static inline void 
_CPU_cache_flush_entire_data( void )
{
  arm_cache_l1_flush_entire_data();
  cache_l2c_310_flush_entire();
}

static inline void 
_CPU_cache_invalidate_data_range( 
  const void *addr_first,
  size_t     n_bytes 
)
{
  if ( n_bytes > 0 ) {
    CACHE_ARM_ERRATA_764369_HANDLER();
    
    cache_l2c_310_invalidate_range(
      addr_first,
      n_bytes
    );
    arm_cache_l1_invalidate_data_range(
      addr_first,
      n_bytes
    );
    cache_l2c_310_invalidate_range(
      addr_first,
      n_bytes
    );
    arm_cache_l1_invalidate_data_range(
      addr_first,
      n_bytes
    );
  }
}

static inline void 
_CPU_cache_invalidate_entire_data( void )
{
  /* This is broadcast within the cluster */
  arm_cache_l1_flush_entire_data();

  /* forces the address out past level 2 */
  cache_l2c_310_clean_and_invalidate_entire();

  /*This is broadcast within the cluster */
  arm_cache_l1_clean_and_invalidate_entire_data();
}

static inline void 
_CPU_cache_store_data_line( const void *d_addr )
{
  const void *ADX =
    (const void *) ( (uint32_t) d_addr & ~CACHE_L2C_310_DATA_LINE_MASK );

  arm_cache_l1_store_data( ADX );
  cache_l2c_310_store( ADX );
}

static inline void 
_CPU_cache_freeze_data( void )
{
  arm_cache_l1_freeze_data();
  cache_l2c_310_freeze();
}

static inline void 
_CPU_cache_unfreeze_data( void )
{
  arm_cache_l1_unfreeze_data();
  cache_l2c_310_unfreeze();
}

static inline void 
_CPU_cache_invalidate_instruction_range(
  const void *i_addr,
  size_t      n_bytes
)
{
  if ( n_bytes != 0 ) {
   CACHE_ARM_ERRATA_764369_HANDLER();
    
    /* Invalidate L2 cache lines */
    cache_l2c_310_invalidate_range(
      i_addr,
      n_bytes
    );
    
    arm_cache_l1_invalidate_instruction_range(
      i_addr,
      n_bytes
    );
  }
}

static inline void 
_CPU_cache_invalidate_entire_instruction( void )
{
  cache_l2c_310_invalidate_entire();
  arm_cache_l1_invalidate_entire_instruction();
}

static inline void 
_CPU_cache_freeze_instruction( void )
{
  arm_cache_l1_freeze_instruction();
  cache_l2c_310_freeze();
}

static inline void 
_CPU_cache_unfreeze_instruction( void )
{
  arm_cache_l1_unfreeze_instruction();
  cache_l2c_310_unfreeze();
}

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LIBBSP_ARM_SHARED_L2C_310_CACHE_H */