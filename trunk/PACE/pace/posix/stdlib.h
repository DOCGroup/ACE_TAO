/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/stdlib.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_STDLIB_H
#define PACE_STDLIB_H

#include "pace/unistd.h"

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

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
     PACE's implementation of the POSIX function rand_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.3.8.
   */
  PACE_INLINE int pace_rand_r (unsigned int * seed);

  /* Memory Management. */

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
# include "pace/posix/stdlib.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_STDLIB_H */
