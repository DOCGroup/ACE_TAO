/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/sys/wait.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_WAIT_H
#define PACE_WAIT_H

#include "pace/config/defines.h"
#include "pace/sys/types.h"

#if (PACE_HAS_POSIX)
# include "pace/posix/wait.h"
#elif (PACE_VXWORKS)
# include "pace/vxworks/wait.h"
#elif (PACE_WIN32)
# include "pace/win32/wait.h"
#endif

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */
  /**
     PACE's implementation of the POSIX function wait.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.2.1.
     */
#if (PACE_HAS_POSIX_MP_UOF)
  PACE_Export PACE_INLINE pace_pid_t pace_wait (int * statloc);
#endif /* PACE_HAS_POSIX_MP_UOF */

  /**
     PACE's implementation of the POSIX function waitpid.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.2.1.
     */
#if (PACE_HAS_POSIX_MP_UOF)
  PACE_Export PACE_INLINE pace_pid_t pace_waitpid (pace_pid_t pid,
                                                   int * statloc,
                                                   int options);
#endif /* PACE_HAS_POSIX_MP_UOF */

#if defined (PACE_HAS_INLINE)
#  if (PACE_HAS_POSIX)
#    include "pace/posix/wait.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/wait.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/wait.inl"
#  endif
#endif /* PACE_HAS_INLINE */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_WAIT_H */
