/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    strings.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */


#include <strings.h>


PACE_INLINE
int
pace_strcasecmp (const char * s1,
                 const char * s2)
{
  return strcasecmp (s1, s2);
}

PACE_INLINE
int
pace_strncasecmp (const char * s1,
                  const char * s2,
                  size_t n)
{
  return strncasecmp (s1, s2, n);
}
