/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/wait.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_WAIT_H
#define PACE_WAIT_H

#include "pace/sys/types.h"

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  PACE_INLINE pid_t pace_wait (int * statloc);

  PACE_INLINE pid_t pace_waitpid (pid_t pid,
                                  int * statloc,
                                  int options);

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/posix/wait.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_WAIT_H */
