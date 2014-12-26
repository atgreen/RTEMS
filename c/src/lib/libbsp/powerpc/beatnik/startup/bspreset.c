#include <rtems.h>
#include <bsp.h>
#include <bsp/bootcard.h>
#include <rtems/bspIo.h>
#include <libcpu/io.h>
#include <libcpu/stackTrace.h>

void bsp_reset()
{

  printk("Printing a stack trace for your convenience :-)\n");
  CPU_print_stack();

  printk("RTEMS terminated; Rebooting ...\n");
  /* Mvme5500 board reset : 2004 S. Kate Feng <feng1@bnl.gov>  */
  out_8((volatile unsigned char*) (BSP_MV64x60_DEV1_BASE +2), 0x80);
}
