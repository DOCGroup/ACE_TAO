/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    wait.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */


#include <sys/wait.h>


PACE_INLINE
pid_t
pace_wait (int * statloc)
{
  return wait (statloc);
}

PACE_INLINE
pid_t
pace_waitpid (pid_t pid, int * statloc, int options)
{
  return waitpid (pid, statloc, options);
}
