/* $Id$

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/unistd.c
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */
#include <stdarg.h>
#include "pace/unistd.h"

int
execl (const char* path, const char* arg, ...)
{
  int result = 0;
  va_list ap;
  va_start (ap, arg);
  result = pace_execv (path, ap);
  va_end (ap);
  return result;
}

int
execle (const char* path, const char* arg, ...)
{
  int result = 0;
  va_list ap;
  va_start (ap, arg);
  result = pace_execve (path, ap, 0);
  va_end (ap);
  return result;
}

int
execlp (const char* file, const char* arg,  ...)
{
  int result = 0;
  va_list ap;
  va_start (ap, arg);
  result = pace_execvp (file, ap);
  va_end (ap);
  return result;
}

#if !defined (PACE_HAS_INLINE)
# include "pace/posix/unistd.inl"
#endif /* ! PACE_HAS_INLINE */
