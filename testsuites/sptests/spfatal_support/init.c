/*
 *  COPYRIGHT (c) 1989-2011.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define CONFIGURE_INIT
#include "system.h"

static void print_test_begin_message(void)
{
  static bool done = false;

  if (!done) {
    done = true;
    printk( "\n\n\n*** TEST FATAL " FATAL_ERROR_TEST_NAME " ***\n" );
  }
}

rtems_task Init(
  rtems_task_argument argument
)
{
  print_test_begin_message();
  force_error();
  printk( "Fatal error (%s) NOT hit\n", FATAL_ERROR_DESCRIPTION );
  rtems_test_exit(0);
}

char *Errors_Rtems[] = {
  "RTEMS_SUCCESSFUL",               /* successful completion */
  "RTEMS_TASK_EXITTED",             /* returned from a task */
  "RTEMS_MP_NOT_CONFIGURED",        /* multiprocessing not configured */
  "RTEMS_INVALID_NAME",             /* invalid object name */
  "RTEMS_INVALID_ID",               /* invalid object id */
  "RTEMS_TOO_MANY",                 /* too many */
  "RTEMS_TIMEOUT",                  /* timed out waiting */
  "RTEMS_OBJECT_WAS_DELETED",       /* object was deleted while waiting */
  "RTEMS_INVALID_SIZE",             /* specified size was invalid */
  "RTEMS_INVALID_ADDRESS",          /* address specified is invalid */
  "RTEMS_INVALID_NUMBER",           /* number was invalid */
  "RTEMS_NOT_DEFINED",              /* item has not been initialized */
  "RTEMS_RESOURCE_IN_USE",          /* resources still outstanding */
  "RTEMS_UNSATISFIED",              /* request not satisfied */
  "RTEMS_INCORRECT_STATE",          /* task is in wrong state */
  "RTEMS_ALREADY_SUSPENDED",        /* task already in state */
  "RTEMS_ILLEGAL_ON_SELF",          /* illegal operation on calling task */
  "RTEMS_ILLEGAL_ON_REMOTE_OBJECT", /* illegal operation for remote object */
  "RTEMS_CALLED_FROM_ISR",          /* called from ISR */
  "RTEMS_INVALID_PRIORITY",         /* invalid task priority */
  "RTEMS_INVALID_CLOCK",            /* invalid date/time */
  "RTEMS_INVALID_NODE",             /* invalid node id */
  "RTEMS_NOT_OWNER_OF_RESOURCE",    /* not owner of resource */
  "RTEMS_NOT_CONFIGURED",           /* directive not configured */
  "RTEMS_NOT_IMPLEMENTED"           /* directive not implemented */
};

void Put_Error( uint32_t source, uint32_t error )
{
  if ( source == INTERNAL_ERROR_CORE ) {
    printk( rtems_internal_error_text( error ) );
  }
  else if (source == INTERNAL_ERROR_RTEMS_API ){
    if (error >  RTEMS_NOT_IMPLEMENTED )
      printk("Unknown Internal Rtems Error (0x%08x)", error);
    else
      printk( Errors_Rtems[ error ] );
  }
}

void Put_Source( rtems_fatal_source source )
{
  printk( "%s", rtems_fatal_source_text( source ) );
}

static bool is_expected_error( rtems_fatal_code error )
{
#ifdef FATAL_ERROR_EXPECTED_ERROR
  return error == FATAL_ERROR_EXPECTED_ERROR;
#else /* FATAL_ERROR_EXPECTED_ERROR */
  return FATAL_ERROR_EXPECTED_ERROR_CHECK( error );
#endif /* FATAL_ERROR_EXPECTED_ERROR */
}

void Fatal_extension(
  rtems_fatal_source source,
  bool               is_internal,
  rtems_fatal_code   error
)
{
  print_test_begin_message();
  printk( "Fatal error (%s) hit\n", FATAL_ERROR_DESCRIPTION );

  if ( source != FATAL_ERROR_EXPECTED_SOURCE ){
    printk( "ERROR==> Fatal Extension source Expected (");
    Put_Source( FATAL_ERROR_EXPECTED_SOURCE );
    printk( ") received (");
    Put_Source( source );
    printk( ")\n" );
  }

  if ( is_internal !=  FATAL_ERROR_EXPECTED_IS_INTERNAL )
  {
    if ( is_internal == TRUE )
      printk(
        "ERROR==> Fatal Extension is internal set to TRUE expected FALSE\n"
      );
    else
      printk(
        "ERROR==> Fatal Extension is internal set to FALSE expected TRUE\n"
      );
  }

#ifdef FATAL_ERROR_EXPECTED_ERROR
  if ( error !=  FATAL_ERROR_EXPECTED_ERROR ) {
    printk( "ERROR==> Fatal Error Expected (");
    Put_Error( source, FATAL_ERROR_EXPECTED_ERROR );
    printk( ") received (");
    Put_Error( source, error );
    printk( ")\n" );
  }
#endif /* FATAL_ERROR_EXPECTED_ERROR */

  if (
    source == FATAL_ERROR_EXPECTED_SOURCE
      && is_internal == FATAL_ERROR_EXPECTED_IS_INTERNAL
      && is_expected_error( error )
  ) {
    printk( "*** END OF TEST FATAL " FATAL_ERROR_TEST_NAME " ***\n" );
  }
}

