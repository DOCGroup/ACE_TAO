/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/wait.inl
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================= */

#include <sys/wait.h>

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
pid_t
pace_wait (int * statloc)
{
  PACE_TRACE("pace_wait");

  PACE_UNUSED_ARG (statloc);
  PACE_ERRNO_NO_SUPPORT_RETURN (0);
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
pid_t
pace_waitpid (pid_t pid, int * statloc, int options)
{
  PACE_TRACE("pace_waitpid");

  PACE_UNUSED_ARG (pid);
  PACE_UNUSED_ARG (statloc);
  PACE_UNUSED_ARG (options);
  PACE_ERRNO_NO_SUPPORT_RETURN (0);
}
#endif /* PACE_HAS_POSIX_MP_UOF */
