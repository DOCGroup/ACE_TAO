/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/string.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_STRING_H
#define PACE_STRING_H

#include "pace/config/defines.h"
#include "pace/unistd.h"

#if (PACE_HAS_POSIX)
# include "pace/posix/string.h"
#elif (PACE_VXWORKS)
# include "pace/vxworks/string.h"
#elif (PACE_WIN32)
# include "pace/win32/string.h"
#endif

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  /**
  pace_memchr function, C std ref. 7.21.5.1
  */
  PACE_INLINE void * pace_memchr(const void *s, int c, pace_size_t n);

  /**
  pace_memcmp function, C std ref. 7.21.4.1
  */
  PACE_INLINE int pace_memcmp(const void *s1, const void *s2, pace_size_t n);

  /**
  pace_memcpy function, C std ref. 7.21.2.1
  */
  PACE_INLINE void *pace_memcpy(void * s1, const void * s2, pace_size_t n);

  /**
  pace_memmove function, C std ref.7.21.2.2
  */
  PACE_INLINE void *pace_memmove(void *s1, const void *s2, pace_size_t n);

  /**
  pace_memset function, C std ref. 7.21.6.1
  */
  PACE_INLINE void *pace_memset(void *s, int c, pace_size_t n);

  /**
     PACE's implementation of the POSIX function strcat.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE char * pace_strcat (char * s1, const char * s2);

  /**
     PACE's implementation of the POSIX function strncat.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE char * pace_strncat (char * s1, const char * s2, size_t n);

  /**
     PACE's implementation of the POSIX function strchr.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE const char * pace_strchr (const char * s, int c);

  /**
     PACE's implementation of the POSIX function strrchr.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE const char * pace_strrchr (const char * s, int c);

  /**
     PACE's implementation of the POSIX function strcmp.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE int pace_strcmp (const char * s1, const char * s2);

  /**
     PACE's implementation of the POSIX function strncmp.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE int pace_strncmp (const char * s1, const char * s2, size_t n);

  /**
     PACE's implementation of the POSIX function strcpy.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE char * pace_strcpy (char * s1, const char * s2);

  /**
     PACE's implementation of the POSIX function strncpy.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE char * pace_strncpy (char * s1, const char * s2, size_t n);

  /**
     PACE's implementation of the POSIX function strcspn.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE size_t pace_strcspn (const char * s1, const char * s2);

  /**
     PACE's implementation of the POSIX function strspn.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE size_t pace_strspn (const char * s1, const char * s2);

  /**
     PACE's implementation of the POSIX function strlen.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE size_t pace_strlen (const char * s);

  /**
     PACE's implementation of the POSIX function strpbrk.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE const char * pace_strpbrk (const char * s1, const char * s2);

  /**
     PACE's implementation of the POSIX function strstr.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE const char * pace_strstr (const char * s1, const char * s2);

  /**
     PACE's implementation of the POSIX function strtok.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE char * pace_strtok (char * s1, const char * s2);

  /**
     PACE's implementation of the POSIX function strtok_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.3.3.
     */
  PACE_INLINE char * pace_strtok_r (char * s,
                                    const char * sep,
                                    char ** lasts);
  /* Requires PACE_HAS_REENTRANT. */


#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
#  if (PACE_HAS_POSIX)
#    include "pace/posix/string.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/string.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/string.inl"
#  endif
#endif /* PACE_HAS_INLINE */

#endif /* PACE_STRING_H */
