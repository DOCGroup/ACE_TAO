/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/fnctl.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_FCNTL_H
#define PACE_FCNTL_H

#include "pace/sys/types.h"
#include "pace/unistd.h"
#include <sys/stat.h>
#include <fcntl.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  PACE_INLINE int pace_creat (const char * path, pace_mode_t mode);

  # define pace_fcntl(X) fcntl X
  /* int fcntl (int fildes, int cmd, *//* arg *//* ... );
   *
   * PLEASE PAY SPECIAL ATTENTION HERE!
   * This is a macro and requires an additional set of parenthesis
   * surrounding the arguments.
   */

  # define pace_open(X) open X
  /* int open (const char *path, int oflag, *//* pace_mode_t mode *//* ... );
   *
   * PLEASE PAY SPECIAL ATTENTION HERE!
   * This is a macro and requires an additional set of parenthesis
   * surrounding the arguments.
   */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/posix/fcntl.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_FCNTL_H */
