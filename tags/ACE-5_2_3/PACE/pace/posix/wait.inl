/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/wait.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include <sys/wait.h>

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
pid_t
pace_wait (int * statloc)
{
  return wait (statloc);
}
#endif /* PACE_HAS_POSIX_MP_UOF */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
pid_t
pace_waitpid (pid_t pid, int * statloc, int options)
{
  return waitpid (pid, statloc, options);
}
#endif /* PACE_HAS_POSIX_MP_UOF */
