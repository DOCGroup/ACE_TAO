/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/time.h
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

  PACE_INLINE char * pace_asctime (const struct tm * time);

  PACE_INLINE char * pace_asctime_r (const struct tm * tm, char * buf);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  PACE_INLINE int pace_clock_getres (clockid_t clock_id,
                                     struct timespec * res);

  PACE_INLINE int pace_clock_gettime (clockid_t clock_id,
                                      struct timespec * tp);

  PACE_INLINE int pace_clock_settime (clockid_t clock_id,
                                      const struct timespec * tp);

  PACE_INLINE char * pace_ctime (const time_t * clock);

  PACE_INLINE char * pace_ctime_r (const time_t * clock, char * buf);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  PACE_INLINE struct tm * pace_gmtime (const time_t * clock);

  PACE_INLINE struct tm * pace_gmtime_r (const time_t * clock,
                                         struct tm * result);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  PACE_INLINE struct tm * pace_localtime (const time_t * clock);

  PACE_INLINE struct tm * pace_localtime_r (const time_t * clock,
                                            struct tm * result);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  PACE_INLINE time_t pace_mktime (struct tm * timeptr);

  PACE_INLINE int pace_nanosleep (const struct timespec * rqtp,
                                  struct timespec * rmtp);

  PACE_INLINE size_t pace_strftime (char *s, size_t maxsize,
                                    const char *format,
                                    const struct tm *timeptr);

  PACE_INLINE time_t pace_time (time_t *tloc);

  PACE_INLINE int pace_timer_create (clockid_t clock_id,
                                     struct sigevent * evp,
                                     timer_t * timerid);

  PACE_INLINE int pace_timer_delete (timer_t timerid);

  PACE_INLINE int pace_timer_getoverrun (timer_t timerid);

  PACE_INLINE int pace_timer_gettime (timer_t timerid,
                                      struct itimerspec * value);

  PACE_INLINE int pace_timer_settime (timer_t timerid,
                                      int flags,
                                      const struct itimerspec * value,
                                      struct itimerspec * ovalue);

  PACE_INLINE void pace_tzset ();
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/win32/time.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_TIME_H */
