/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/time.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_TIME_H
#define PACE_TIME_H

#include <time.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  /** 
     PACE's implementation of the POSIX function asctime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE char * pace_asctime (const struct tm * time);

  /** 
     PACE's implementation of the POSIX function asctime_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.1.
   */
  PACE_INLINE char * pace_asctime_r (const struct tm * tm, char * buf);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  /** 
     PACE's implementation of the POSIX function clock_getres.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.1.
   */
  PACE_INLINE int pace_clock_getres (clockid_t clock_id,
                                     struct timespec * res);

  /** 
     PACE's implementation of the POSIX function clock_gettime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.1.
   */
  PACE_INLINE int pace_clock_gettime (clockid_t clock_id,
                                      struct timespec * tp);

  /** 
     PACE's implementation of the POSIX function clock_settime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.1.
   */
  PACE_INLINE int pace_clock_settime (clockid_t clock_id,
                                      const struct timespec * tp);

  /** 
     PACE's implementation of the POSIX function ctime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE char * pace_ctime (const time_t * clock);

  /** 
     PACE's implementation of the POSIX function ctime_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.3.5.
   */
  PACE_INLINE char * pace_ctime_r (const time_t * clock, char * buf);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  /** 
     PACE's implementation of the POSIX function gmtime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE struct tm * pace_gmtime (const time_t * clock);

  /** 
     PACE's implementation of the POSIX function gmtime_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.3.6.
   */
  PACE_INLINE struct tm * pace_gmtime_r (const time_t * clock,
                                         struct tm * result);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  /** 
     PACE's implementation of the POSIX function localtime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE struct tm * pace_localtime (const time_t * clock);

  /** 
     PACE's implementation of the POSIX function localtime_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.3.7.
   */
  PACE_INLINE struct tm * pace_localtime_r (const time_t * clock,
                                            struct tm * result);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  /** 
     PACE's implementation of the POSIX function mktime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE time_t pace_mktime (const tm * timeptr);

  /** 
     PACE's implementation of the POSIX function nanosleep.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.5.
   */
  PACE_INLINE int pace_nanosleep (const struct timespec * rqtp,
                                  struct timespec * rmtp);

  /** 
     PACE's implementation of the POSIX function strftime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE size_t pace_strftime (const char *s, size_t maxsize,
                                    const char *format,
                                    const struct tm *timeptr);

  /** 
     PACE's implementation of the POSIX function time.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.5.1.
   */
  PACE_INLINE time_t pace_time (time_t *tloc);

  /** 
     PACE's implementation of the POSIX function timer_create.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.2.
   */
  PACE_INLINE int pace_timer_create (clockid_t clock_id,
                                     struct sigevent * evp,
                                     timer_t * timerid);

  /** 
     PACE's implementation of the POSIX function timer_delete.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.3.
   */
  PACE_INLINE int pace_timer_delete (timer_t timerid);

  /** 
     PACE's implementation of the POSIX function timer_getoverrun.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.4.
   */
  PACE_INLINE int pace_timer_getoverrun (timer_t timerid);

  /** 
     PACE's implementation of the POSIX function timer_gettime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.4.
   */
  PACE_INLINE int pace_timer_gettime (timer_t timerid,
                                      struct itimerspec * value);

  /** 
     PACE's implementation of the POSIX function timer_settime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.4.
   */
  PACE_INLINE int pace_timer_settime (timer_t timerid,
                                      int flags,
                                      const struct itimerspec * value,
                                      struct itimerspec * ovalue);

  /** 
     PACE's implementation of the POSIX function tzset.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.3.2.
   */
  PACE_INLINE void pace_tzset ();
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/posix/time.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_TIME_H */
