/* $Id$

 * ============================================================================
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
 * ============================================================================ */

#ifndef PACE_CTYPE_H
#define PACE_CTYPE_H

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  PACE_INLINE int pace_isalnum (int c);

  PACE_INLINE int pace_isalpha (int c);

  PACE_INLINE int pace_iscntrl (int c);

  PACE_INLINE int pace_isdigit (int c);

  PACE_INLINE int pace_isgraph (int c);

  PACE_INLINE int pace_islower (int c);

  PACE_INLINE int pace_isprint (int c);

  PACE_INLINE int pace_ispunct (int c);

  PACE_INLINE int pace_isspace (int c);

  PACE_INLINE int pace_isupper (int c);

  PACE_INLINE int pace_isxdigit (int c);

  PACE_INLINE int pace_tolower (int c);

  PACE_INLINE int pace_toupper (int c);
  
#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/posix/ctype.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_CTYPE_H */
