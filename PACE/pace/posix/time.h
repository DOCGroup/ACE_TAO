/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    time.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */


#ifndef PACE_TIME_H
#define PACE_TIME_H


#include "pace/defines.h"

/* Linux doesn't support this file at all.
 * Need these typedefs to just allow compilation.
 */
# if defined (linux)
typedef unsigned int clockid_t;
typedef unsigned int timerid_t;
typedef unsigned int timer_t;
struct tm { };
struct sigevent { };
struct itimerspec { };
#include "pace/unistd.h"
# else /* !linux */
#include <time.h>
#endif /* ! linux */


# if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
# endif /* PACE_HAS_CPLUSPLUS */

  PACE_INLINE char * pace_asctime_r (const struct tm * tm, char * buf);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  PACE_INLINE int pace_clock_getres (clockid_t clock_id,
                                     struct timespec * res);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  PACE_INLINE int pace_clock_gettime (clockid_t clock_id,
                                      struct timespec * tp);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  PACE_INLINE int pace_clock_settime (clockid_t clock_id,
                                      const struct timespec * tp);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  PACE_INLINE char * pace_ctime_r (const time_t * clock, char * buf);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  PACE_INLINE struct tm * pace_gmtime_r (const time_t * clock,
                                         struct tm * result);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  PACE_INLINE struct tm * pace_localtime_r (const time_t * clock,
                                            struct tm * result);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  PACE_INLINE int pace_nanosleep (const struct timespec * rqtp,
                                  struct timespec * rmtp);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  PACE_INLINE int pace_time (time_t *tloc);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  PACE_INLINE int pace_timer_create (clockid_t clock_id,
                                     struct sigevent * evp,
                                     timer_t * timerid);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  PACE_INLINE int pace_timer_delete (timer_t timerid);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  PACE_INLINE int pace_timer_getoverrun (timer_t timerid);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  PACE_INLINE int pace_timer_gettime (timer_t timerid,
                                      struct itimerspec * value);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  PACE_INLINE int pace_timer_settime (timer_t timerid,
                                      int flags,
                                      const struct itimerspec * value,
                                      struct itimerspec * ovalue);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  PACE_INLINE void pace_tzset ();
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

# if defined (PACE_HAS_CPLUSPLUS)
}
# endif /* PACE_HAS_CPLUSPLUS */

# if defined (PACE_HAS_INLINE)
# include "time.inl"
# endif /* PACE_HAS_INLINE */


#endif /* PACE_TIME_H */
