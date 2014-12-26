/**
 * @file
 *
 * @brief Link Operations Table for the IMFS
 * @ingroup IMFS
 */

/*
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
  #include "config.h"
#endif

#include "imfs.h"

#include <stdlib.h>
#include <string.h>

static int IMFS_stat_link(
  const rtems_filesystem_location_info_t *loc,
  struct stat *buf
)
{
  const IMFS_jnode_t *node = loc->node_access;

  if ( IMFS_type( node ) != IMFS_HARD_LINK ) {
    buf->st_size = strlen( node->info.sym_link.name );

    return IMFS_stat( loc, buf );
  } else {
    rtems_filesystem_location_info_t targetloc = *loc;

    targetloc.node_access = node->info.hard_link.link_node;
    IMFS_Set_handlers( &targetloc );

    return (targetloc.handlers->fstat_h)( &targetloc, buf );
  }
}

static const rtems_filesystem_file_handlers_r IMFS_link_handlers = {
  .open_h = rtems_filesystem_default_open,
  .close_h = rtems_filesystem_default_close,
  .read_h = rtems_filesystem_default_read,
  .write_h = rtems_filesystem_default_write,
  .ioctl_h = rtems_filesystem_default_ioctl,
  .lseek_h = rtems_filesystem_default_lseek,
  .fstat_h = IMFS_stat_link,
  .ftruncate_h = rtems_filesystem_default_ftruncate,
  .fsync_h = rtems_filesystem_default_fsync_or_fdatasync,
  .fdatasync_h = rtems_filesystem_default_fsync_or_fdatasync,
  .fcntl_h = rtems_filesystem_default_fcntl,
  .kqfilter_h = rtems_filesystem_default_kqfilter,
  .poll_h = rtems_filesystem_default_poll,
  .readv_h = rtems_filesystem_default_readv,
  .writev_h = rtems_filesystem_default_writev
};

static IMFS_jnode_t *IMFS_node_initialize_hard_link(
  IMFS_jnode_t *node,
  const IMFS_types_union *info
)
{
  node->info.hard_link.link_node = info->hard_link.link_node;

  return node;
}

static IMFS_jnode_t *IMFS_node_remove_hard_link(
  IMFS_jnode_t *node
)
{
  IMFS_jnode_t *target = node->info.hard_link.link_node;

  if ( target->st_nlink == 1) {
    target = (*target->control->node_remove)( target );
    if ( target == NULL ) {
      node = NULL;
    }
  } else {
    --target->st_nlink;
    IMFS_update_ctime( target );
  }

  return node;
}

const IMFS_node_control IMFS_node_control_hard_link = {
  .imfs_type = IMFS_HARD_LINK,
  .handlers = &IMFS_link_handlers,
  .node_initialize = IMFS_node_initialize_hard_link,
  .node_remove = IMFS_node_remove_hard_link,
  .node_destroy = IMFS_node_destroy_default
};

static IMFS_jnode_t *IMFS_node_initialize_sym_link(
  IMFS_jnode_t *node,
  const IMFS_types_union *info
)
{
  node->info.sym_link.name = info->sym_link.name;

  return node;
}

static IMFS_jnode_t *IMFS_node_destroy_sym_link( IMFS_jnode_t *node )
{
  free( node->info.sym_link.name );

  return node;
}

const IMFS_node_control IMFS_node_control_sym_link = {
  .imfs_type = IMFS_SYM_LINK,
  .handlers = &IMFS_link_handlers,
  .node_initialize = IMFS_node_initialize_sym_link,
  .node_remove = IMFS_node_remove_default,
  .node_destroy = IMFS_node_destroy_sym_link
};
