/* $Id$

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/string.c
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include "pace/string.h"

#if !defined (PACE_HAS_INLINE)
# include "pace/win32/string.inl"
#endif /* ! PACE_HAS_INLINE */

#if (PACE_HAS_POSIX_CLSR_UOF)
char *
strtok_r (char * s, const char * sep, char ** lasts)
{
  if (s == 0)
    s = *lasts;
  else
    *lasts = s;
  if (*s == 0)                  // We have reached the end
    return 0;
  int l_org = ACE_OS_String::strlen (s);
  int l_sub = ACE_OS_String::strlen (s = ::strtok (s, sep));
  *lasts = s + l_sub;
  if (l_sub != l_org)
    *lasts += 1;
  return s ;
}
#endif /* PACE_HAS_POSIX_CLSR_UOF */
