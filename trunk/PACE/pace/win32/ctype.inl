/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/ctype.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include <ctype.h>

PACE_INLINE
int
pace_isalnum (int c)
{
  return isalnum (c);
}

PACE_INLINE
int
pace_isalpha (int c)
{
  return isalpha (c);
}

PACE_INLINE
int
pace_iscntrl (int c)
{
  return iscntrl (c);
}

PACE_INLINE
int
pace_isdigit (int c)
{
  return isdigit (c);
}

PACE_INLINE
int
pace_isgraph (int c)
{
  return isgraph (c);
}

PACE_INLINE
int
pace_islower (int c)
{
  return islower (c);
}

PACE_INLINE
int
pace_isprint (int c)
{
  return isprint (c);
}

PACE_INLINE
int
pace_ispunct (int c)
{
  return ispunct (c);
}

PACE_INLINE
int
pace_isspace (int c)
{
  return isspace (c);
}

PACE_INLINE
int
pace_isupper (int c)
{
  return isupper (c);
}

PACE_INLINE
int
pace_isxdigit (int c)
{
  return isxdigit (c);
}

PACE_INLINE
int
pace_tolower (int c)
{
  return tolower (c);
}

PACE_INLINE
int
pace_toupper (int c)
{
  return toupper (c);
}
