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

  /** 
     PACE's implementation of the POSIX function creat.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.3.2.
   */
  PACE_INLINE int pace_creat (const char * path, pace_mode_t mode);

  /** 
     PACE's implementation of the POSIX function fcntl.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 6.5.2.
   */
  # define pace_fcntl(X) fcntl X
  /* int fcntl (int fildes, int cmd, *//* arg *//* ... );
   *
   * PLEASE PAY SPECIAL ATTENTION HERE!
   * This is a macro and requires an additional set of parenthesis
   * surrounding the arguments.
   */

  /** 
     PACE's implementation of the POSIX function open.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 5.3.1.
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
