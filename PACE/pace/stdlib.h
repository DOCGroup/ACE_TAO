/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/stdlib.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_STDLIB_H
#define PACE_STDLIB_H

#include "pace/config/defines.h"
#include "pace/unistd.h"

#if (PACE_HAS_POSIX)
# include "pace/posix/stdlib.h"
#elif (PACE_VXWORKS)
# include "pace/vxworks/stdlib.h"
#elif (PACE_WIN32)
# include "pace/win32/stdlib.h"
#endif

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  /**
    C std 7.20.4.1
    */
  PACE_INLINE void pace_abort (void);

  /**
     PACE's implementation of the POSIX function abs.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE int pace_abs (int val);

  /**
     PACE's implementation of the POSIX function atof.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE double pace_atof (const char * str);

  /**
     PACE's implementation of the POSIX function atoi.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE int pace_atoi (const char * str);

  /**
     PACE's implementation of the POSIX function atol.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE long pace_atol (const char * str);

  /**
     PACE's implementation of the POSIX function bsearch.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE void * pace_bsearch (const void *key, const void *base,
                                   size_t nel, size_t size,
                                   int (*compar)(const void *,const void *));

  /**
     PACE's implementation of the POSIX function exit.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE void pace_exit (int status);

  /**
     PACE's implementation of the POSIX function getenv.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.6.1.
     */
  PACE_INLINE char * pace_getenv (const char * name);

  /**
     PACE's implementation of the POSIX function qsort.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE void pace_qsort (void * base, size_t nel, size_t width,
                               int (*compar)(const void *, const void *));

  /**
     PACE's implementation of the POSIX function rand.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE int pace_rand ();

  /**
     PACE's implementation of the POSIX function srand.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE void pace_srand (unsigned int seed);

  /**
     PACE's implementation of the POSIX function rand_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.3.8.
     */
  PACE_INLINE int pace_rand_r (unsigned int * seed);
  /* Requires PACE_HAS_REENTRANT. */

  /* Memory Management */

  /**
     PACE's implementation of the POSIX function malloc.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE void * pace_malloc (size_t size);

  /**
     PACE's implementation of the POSIX function calloc.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE void * pace_calloc (size_t nelem, size_t elsize);

  /**
     PACE's implementation of the POSIX function free.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE void pace_free (void * ptr);

  /**
     PACE's implementation of the POSIX function realloc.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
  PACE_INLINE void * pace_realloc (void * ptr, size_t size);

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
#  if (PACE_HAS_POSIX)
#    include "pace/posix/stdlib.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/stdlib.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/stdlib.inl"
#  endif
#endif /* PACE_HAS_INLINE */

#endif /* PACE_STDLIB_H */
