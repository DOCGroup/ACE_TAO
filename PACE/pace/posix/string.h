/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/string.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_STRING_H
#define PACE_STRING_H

#include "pace/defines.h"
#include "pace/unistd.h"

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  PACE_INLINE void * pace_memccpy (void * s1, const void * s2, int c, size_t n);

  PACE_INLINE const void * pace_memchr (const void * s, int c, size_t n);

  PACE_INLINE int pace_memcmp (const void * s1, const void * s2, size_t n);

  PACE_INLINE void * pace_memcpy (void * s1, const void * s2, size_t n);

  PACE_INLINE void * pace_memmove (void * s1, const void * s2, size_t n);

  PACE_INLINE void * pace_memset (void * s, int c, size_t n);

  PACE_INLINE char * pace_strcat (char * s1, const char * s2);

  PACE_INLINE char * pace_strncat (char * s1, const char * s2, size_t n);

  PACE_INLINE const char * pace_strchr (const char * s, int c);

  PACE_INLINE const char * pace_strrchr (const char * s, int c);

  PACE_INLINE int pace_strcmp (const char * s1, const char * s2);

  PACE_INLINE int pace_strncmp (const char * s1, const char * s2, size_t n);

  PACE_INLINE char * pace_strcpy (char * s1, const char * s2);

  PACE_INLINE char * pace_strncpy (char * s1, const char * s2, size_t n);

  PACE_INLINE size_t pace_strcspn (const char * s1, const char * s2);

  PACE_INLINE size_t pace_strspn (const char * s1, const char * s2);

  PACE_INLINE char * pace_strdup (const char * s1);

  PACE_INLINE size_t pace_strlen (const char * s);

  PACE_INLINE const char * pace_strpbrk (const char * s1, const char * s2);

  PACE_INLINE const char * pace_strstr (const char * s1, const char * s2);

  PACE_INLINE char * pace_strtok (char * s1, const char * s2);

  PACE_INLINE char * pace_strtok_r (char * s,
                                    const char * sep,
                                    char ** lasts);
  /* Requires PACE_HAS_REENTRANT. */



#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/posix/string.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_STRING_H */
