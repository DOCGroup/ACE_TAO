/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/time.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_TIME_H
#define PACE_TIME_H

#include "pace/config/defines.h"

#if (PACE_HAS_POSIX)
# include "pace/posix/time.h"
#elif (PACE_VXWORKS)
# include "pace/vxworks/time.h"
#elif (PACE_WIN32)
# include "pace/win32/time.h"
#endif

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  /**
     PACE's implementation of the POSIX function asctime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE char * pace_asctime (const pace_tm * time);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function asctime_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.1.
     */
#if (PACE_HAS_POSIX_CLSR_UOF)
  PACE_Export PACE_INLINE char * pace_asctime_r (const pace_tm * tm,
                                                 char * buf);
#endif /* PACE_HAS_POSIX_CLSR_UOF */
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */


  /**
     C std 7.23.2.1
     */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE pace_clock_t pace_clock (void);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     C std 7.23.2.2
     */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE double pace_difftime (pace_time_t time1,
                                                pace_time_t time2);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function clock_getres.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.1.
     */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_clock_getres (pace_clockid_t clock_id,
                                                 pace_timespec * res);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function clock_gettime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.1.
     */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_clock_gettime (pace_clockid_t clock_id,
                                                  pace_timespec * tp);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function clock_settime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.1.
     */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_clock_settime (pace_clockid_t clock_id,
                                                  const pace_timespec * tp);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function ctime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE char * pace_ctime (const pace_time_t * clock);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function ctime_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.3.5.
     */
#if (PACE_HAS_POSIX_CLSR_UOF)
  PACE_Export PACE_INLINE char * pace_ctime_r (const pace_time_t * clock,
                                               char * buf);
#endif /* PACE_HAS_POSIX_CLSR_UOF */
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  /**
     PACE's implementation of the POSIX function gmtime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE pace_tm * pace_gmtime (const pace_time_t * clock);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function gmtime_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.3.6.
     */
#if (PACE_HAS_POSIX_CLSR_UOF)
  PACE_Export PACE_INLINE pace_tm * pace_gmtime_r (const pace_time_t * clock,
                                                   pace_tm * result);
#endif /* PACE_HAS_POSIX_CLSR_UOF */
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  /**
     PACE's implementation of the POSIX function localtime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE pace_tm * pace_localtime (const time_t * clock);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function localtime_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.3.7.
     */
#if (PACE_HAS_POSIX_CLSR_UOF)
  PACE_Export PACE_INLINE pace_tm * pace_localtime_r (const pace_time_t * clock,
                                                      pace_tm * result);
#endif /* PACE_HAS_POSIX_CLSR_UOF */
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  /**
     PACE's implementation of the POSIX function mktime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE pace_time_t pace_mktime (pace_tm * timeptr);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function nanosleep.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.5.
     */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_nanosleep (const pace_timespec * rqtp,
                                              pace_timespec * rmtp);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function strftime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
     */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE pace_size_t pace_strftime (char *s,
                                                     pace_size_t maxsize,
                                                     const char *format,
                                                     const pace_tm *timeptr);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function time.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.5.1.
     */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE pace_time_t pace_time (pace_time_t *tloc);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function timer_create.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.2.
     */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_timer_create (pace_clockid_t clock_id,
                                                 pace_sigevent * evp,
                                                 pace_timer_t * timerid);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function timer_delete.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.3.
     */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_timer_delete (pace_timer_t timerid);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function timer_getoverrun.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.4.
     */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_timer_getoverrun (pace_timer_t timerid);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function timer_gettime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.4.
     */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_timer_gettime (pace_timer_t timerid,
                                                  pace_itimerspec * value);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function timer_settime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.4.
     */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_timer_settime (pace_timer_t timerid,
                                                  int flags,
                                                  const pace_itimerspec * value,
                                                  pace_itimerspec * ovalue);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function tzset.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.3.2.
     */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE void pace_tzset ();
#endif /* PACE_HAS_POSIX_CLS_UOF */
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

#if defined (PACE_HAS_INLINE)
#  if (PACE_HAS_POSIX)
#    include "pace/posix/time.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/time.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/time.inl"
#  endif
#endif /* PACE_HAS_INLINE */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_TIME_H */
