/* $Id$ -*- C -*-

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/locale.inl
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * =========================================================================== */

#include <locale.h>

PACE_INLINE
char *
pace_setlocale (int category, const char * locale)
{
  return setlocale (category, locale);
}
