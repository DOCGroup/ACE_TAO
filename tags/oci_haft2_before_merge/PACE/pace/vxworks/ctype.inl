/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/ctype.inl
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================= */

#include <ctype.h>

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_isalnum (int c)
{
  PACE_TRACE("pace_isalnum");

  return isalnum (c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_isalpha (int c)
{
  PACE_TRACE("pace_isalpha");

  return isalpha (c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_iscntrl (int c)
{
  PACE_TRACE("pace_iscntrl");

  return iscntrl (c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_isdigit (int c)
{
  PACE_TRACE("pace_isdigit");

  return isdigit (c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_isgraph (int c)
{
  PACE_TRACE("pace_isgraph");

  return isgraph (c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_islower (int c)
{
  PACE_TRACE("pace_islower");

  return islower (c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_isprint (int c)
{
  PACE_TRACE("pace_isprint");

  return isprint (c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_ispunct (int c)
{
  PACE_TRACE("pace_ispunct");

  return ispunct (c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_isspace (int c)
{
  PACE_TRACE("pace_isspace");

  return isspace (c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_isupper (int c)
{
  PACE_TRACE("pace_isupper");

  return isupper (c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_isxdigit (int c)
{
  PACE_TRACE("pace_isxdigit");

  return isxdigit (c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_tolower (int c)
{
  PACE_TRACE("pace_tolower");

  return tolower (c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if (PACE_HAS_POSIX_CLS_UOF)
PACE_INLINE
int
pace_toupper (int c)
{
  PACE_TRACE("pace_toupper");

  return toupper (c);
}
#endif /* PACE_HAS_POSIX_CLS_UOF */
