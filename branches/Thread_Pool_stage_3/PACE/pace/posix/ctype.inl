/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/ctype.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include <ctype.h>

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_isalnum (int c)
{
  return isalnum (c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_isalpha (int c)
{
  return isalpha (c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_iscntrl (int c)
{
  return iscntrl (c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_isdigit (int c)
{
  return isdigit (c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_isgraph (int c)
{
  return isgraph (c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_islower (int c)
{
  return islower (c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_isprint (int c)
{
  return isprint (c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_ispunct (int c)
{
  return ispunct (c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_isspace (int c)
{
  return isspace (c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_isupper (int c)
{
  return isupper (c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_isxdigit (int c)
{
  return isxdigit (c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_tolower (int c)
{
  return tolower (c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_toupper (int c)
{
  return toupper (c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */
