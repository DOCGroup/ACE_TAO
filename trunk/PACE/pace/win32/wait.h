/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/wait.h
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

#define PACE_WEXITSTATUS WEXITSTATUS
#define PACE_WIFEXITED WIFEXITED
#define PACE_WIFSIGNALED WISIGNALED
#define PACE_WIFSTOPPED WIFSTOPPED
#define PACE_WNOHANG WNOHANG
#define PACE_WSTOPSIG WSTOPSIG
#define PACE_WTERMSIG WTERMSIG
#define PACE_WUNTRACED WUNTRACED

  /**
     PACE's implementation of the POSIX function wait.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.2.1.
   */
  PACE_INLINE pid_t pace_wait (int * statloc);

  /**
     PACE's implementation of the POSIX function waitpid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.2.1.
   */
  PACE_INLINE pid_t pace_waitpid (pid_t pid,
                                  int * statloc,
                                  int options);

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/win32/wait.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_WAIT_H */
