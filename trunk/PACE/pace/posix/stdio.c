/* $Id$

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/stdio.c
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include "pace/stdio.h"
#include <stdarg.h>

int
pace_fprintf (FILE *fp, const char *format, ...)
{
  int result = 0;
  va_list ap;
  va_start (ap, format);
  result = vfprintf (fp, format, ap);
  va_end (ap);
  return result;
}

int
pace_printf (const char* format, ...)
{
  int result = 0;
  va_list ap;
  va_start (ap, format);
  result = vprintf (format, ap);
  va_end (ap);
  return result;
}

int
pace_sprintf (char* s, const char* format, ...)
{
  int result = 0;
  va_list ap;
  va_start (ap, format);
  result = vsprintf (s, format, ap);
  va_end (ap);
  return result;
}

int
pace_snprintf (char *s, size_t  n, const char *format, ...)
{
  int result = 0;
  va_list ap;
  va_start (ap, format);
  result = vsnprintf (s, n, format, ap);
  va_end (ap);
  return result;
}

#if !defined (PACE_HAS_INLINE)
# include "pace/posix/stdio.inl"
#endif /* ! PACE_HAS_INLINE */
