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

#define PACE_CLK_TCK CLK_TCK
#define PACE_CLOCKS_PER_SEC CLOCKS_PER_SEC
#define PACE_CLOCK_REALTIME CLOCK_REALTIME
#define PACE_NULL NULL
#define PACE_TIMER_ABSTIME TIMER_ABSTIME

#ifndef PACE_CLOCKID_T
#define PACE_CLOCKID_T
  typedef clockid_t pace_clockid_t;
#endif /* PACE_CLOCKID_T */

#ifndef PACE_CLOCK_T
#define PACE_CLOCK_T
  typedef clock_t pace_clock_t;
#endif /* PACE_CLOCK_T */

#ifndef PACE_SIZE_T
#define PACE_SIZE_T
  typedef size_t pace_size_t;
#endif /* PACE_SIZE_T */

#ifndef PACE_TIMER_T
#define PACE_TIMER_T
  typedef timer_t pace_timer_t;
#endif /* PACE_TIMER_T */

#ifndef PACE_TIME_T
#define PACE_TIME_T
  typedef time_t pace_time_t;
#endif /* PACE_TIME_T */

#ifndef PACE_TM
#define PACE_TM
  typedef struct tm pace_tm;
#endif /* PACE_TM */

#ifndef PACE_TIMESPEC
#define PACE_TIMESPEC
  typedef struct timespec pace_timespec;
#endif /* PACE_TIMESPEC */

#ifndef PACE_ITIMERSPEC
#define PACE_ITIMERSPEC
  typedef struct itimerspec pace_itimerspec;
#endif /* PACE_ITIMERSPEC */

  /**
     PACE's implementation of the POSIX function asctime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE char * pace_asctime (const pace_tm * time);

  /**
     PACE's implementation of the POSIX function asctime_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.1.
   */
  PACE_INLINE char * pace_asctime_r (const pace_tm * tm, char * buf);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */


  /**
     C std 7.23.2.1
     */
  PACE_INLINE pace_clock_t pace_clock (void);

  /**
     C std 7.23.2.2
     */
  PACE_INLINE double pace_difftime (pace_time_t time1, pace_time_t time2);

  /**
     PACE's implementation of the POSIX function clock_getres.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.1.
   */
  PACE_INLINE int pace_clock_getres (pace_clockid_t clock_id,
                                     pace_timespec * res);

  /**
     PACE's implementation of the POSIX function clock_gettime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.1.
   */
  PACE_INLINE int pace_clock_gettime (pace_clockid_t clock_id,
                                      pace_timespec * tp);

  /**
     PACE's implementation of the POSIX function clock_settime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.1.
   */
  PACE_INLINE int pace_clock_settime (pace_clockid_t clock_id,
                                      const pace_timespec * tp);

  /**
     PACE's implementation of the POSIX function ctime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE char * pace_ctime (const pace_time_t * clock);

  /**
     PACE's implementation of the POSIX function ctime_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.3.5.
   */
  PACE_INLINE char * pace_ctime_r (const pace_time_t * clock, char * buf);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  /**
     PACE's implementation of the POSIX function gmtime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE pace_tm * pace_gmtime (const pace_time_t * clock);

  /**
     PACE's implementation of the POSIX function gmtime_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.3.6.
   */
  PACE_INLINE pace_tm * pace_gmtime_r (const pace_time_t * clock,
                                       pace_tm * result);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  /**
     PACE's implementation of the POSIX function localtime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE pace_tm * pace_localtime (const time_t * clock);

  /**
     PACE's implementation of the POSIX function localtime_r.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.3.7.
   */
  PACE_INLINE pace_tm * pace_localtime_r (const pace_time_t * clock,
                                          pace_tm * result);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  /**
     PACE's implementation of the POSIX function mktime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE pace_time_t pace_mktime (pace_tm * timeptr);

  /**
     PACE's implementation of the POSIX function nanosleep.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.5.
   */
  PACE_INLINE int pace_nanosleep (const pace_timespec * rqtp,
                                  pace_timespec * rmtp);

  /**
     PACE's implementation of the POSIX function strftime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
  PACE_INLINE pace_size_t pace_strftime (char *s, pace_size_t maxsize,
                                         const char *format,
                                         const pace_tm *timeptr);

  /**
     PACE's implementation of the POSIX function time.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 4.5.1.
   */
  PACE_INLINE pace_time_t pace_time (pace_time_t *tloc);

  /**
     PACE's implementation of the POSIX function timer_create.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.2.
   */
  PACE_INLINE int pace_timer_create (pace_clockid_t clock_id,
                                     pace_sigevent * evp,
                                     pace_timer_t * timerid);

  /**
     PACE's implementation of the POSIX function timer_delete.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.3.
   */
  PACE_INLINE int pace_timer_delete (pace_timer_t timerid);

  /**
     PACE's implementation of the POSIX function timer_getoverrun.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.4.
   */
  PACE_INLINE int pace_timer_getoverrun (pace_timer_t timerid);

  /**
     PACE's implementation of the POSIX function timer_gettime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.4.
   */
  PACE_INLINE int pace_timer_gettime (pace_timer_t timerid,
                                      pace_itimerspec * value);

  /**
     PACE's implementation of the POSIX function timer_settime.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 14.2.4.
   */
  PACE_INLINE int pace_timer_settime (pace_timer_t timerid,
                                      int flags,
                                      const pace_itimerspec * value,
                                      pace_itimerspec * ovalue);

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
