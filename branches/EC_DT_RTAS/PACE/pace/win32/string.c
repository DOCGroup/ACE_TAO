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
  int l_org;
  int l_sub;

  if (s == 0)
    s = *lasts;
  else
    *lasts = s;
  if (*s == 0)                  // We have reached the end
    return 0;
  l_org = pace_strlen (s);
  l_sub = pace_strlen (s = pace_strtok (s, sep));
  *lasts = s + l_sub;
  if (l_sub != l_org)
    *lasts += 1;
  return s ;
}
#endif /* PACE_HAS_POSIX_CLSR_UOF */
