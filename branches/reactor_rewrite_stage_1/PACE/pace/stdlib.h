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
#if (PACE_HAS_POSIX_SIG_UOF)
  PACE_Export PACE_INLINE void pace_abort (void);
#endif /* PACE_HAS_POSIX_SIG_UOF */

  /**
     PACE's implementation of the POSIX function abs.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE int pace_abs (int val);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
    C std ref. 7.20.4.2
    */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_atexit (void (*func)(void));
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function atof.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE double pace_atof (const char * str);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function atoi.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE int pace_atoi (const char * str);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function atol.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE long pace_atol (const char * str);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function bsearch.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE void * pace_bsearch (const void *key,
                                               const void *base,
                                               size_t nel, size_t size,
                                               int (*compar)(const void *,
                                                             const void *));
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
    C std ref. 7.20.6.2
    */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE pace_div_t pace_div (int numer, int denom);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function exit.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_MP_UOF)
  PACE_Export PACE_INLINE void pace_exit (int status);
#endif /* PACE_HAS_POSIX_MP_UOF */

  /**
     PACE's implementation of the POSIX function getenv.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.6.1.
     */
#if (PACE_HAS_POSIX_MP_UOF)
  PACE_Export PACE_INLINE char * pace_getenv (const char * name);
#endif /* PACE_HAS_POSIX_MP_UOF */

  /**
    C std ref. 7.20.6.1
    */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE long int pace_labs (long int j);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
    C std ref. 7.20.6.2
    */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE pace_ldiv_t pace_ldiv (long int numer,
                                                 long int denom);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
    C std ref. 7.20.7.1
    */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_mblen (const char* s, pace_size_t n);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
    C std ref. 7.20.8.1
    */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE pace_size_t pace_mbstowcs (pace_wchar_t* pwcs,
                                                     const char* s,
                                                     pace_size_t n);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
    C std ref. 7.20.7.2
    */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_mbtowc (pace_wchar_t* pwc,
                                           const char* s,
                                           pace_size_t n);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function qsort.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE void pace_qsort (void * base,
                                           size_t nel,
                                           size_t width,
                                           int (*compar)(const void *,
                                                         const void *));
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function rand.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE int pace_rand ();
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function srand.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE void pace_srand (unsigned int seed);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function rand_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.3.8.
     */
#if (PACE_HAS_POSIX_CLSR_UOF)
  PACE_Export PACE_INLINE int pace_rand_r (unsigned int * seed);
#endif /* PACE_HAS_POSIX_CLSR_UOF */
  /* Requires PACE_HAS_REENTRANT. */

  /**
    C std ref. 7.20.1.3
    */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE double pace_strtod (const char* nptr,
                                              char** endptr);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
    C std ref. 7.20.1.4
    */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_strtol (const char* nptr,
                                           char** endptr,
                                           int base);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
    C std ref. 7.20.7.2
    */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE unsigned long int pace_strtoul (const char* nptr,
                                                          char** endptr,
                                                          int base);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
    C std ref. 7.20.4.5
    */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_system (const char* string);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
    C std ref. 7.20.8.2
    */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE pace_size_t pace_wcstombs (char* s,
                                                     const pace_wchar_t* pwcs,
                                                     pace_size_t n);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
    C std ref. 7.20.7.3
    */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_wctomb (char* s, pace_wchar_t wchar);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */


                        /* Memory Management */

  /**
     PACE's implementation of the POSIX function malloc.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE void * pace_malloc (size_t size);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function calloc.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE void * pace_calloc (size_t nelem, size_t elsize);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function free.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE void pace_free (void * ptr);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function realloc.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE void * pace_realloc (void * ptr, size_t size);
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if defined (PACE_HAS_INLINE)
#  if (PACE_HAS_POSIX)
#    include "pace/posix/stdlib.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/stdlib.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/stdlib.inl"
#  endif
#endif /* PACE_HAS_INLINE */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_STDLIB_H */
