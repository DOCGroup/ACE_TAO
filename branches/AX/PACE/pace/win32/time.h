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

#ifndef PACE_TIME_H_WIN32
#define PACE_TIME_H_WIN32

#include <time.h>
#include "pace/sys/types.h"

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

# ifndef PACE_CLK_TCK
# define PACE_CLK_TCK CLK_TCK
# endif /* PACE_CLK_TCK */

# ifndef PACE_CLOCKS_PER_SEC
# define PACE_CLOCKS_PER_SEC CLOCKS_PER_SEC
# endif /* PACE_CLOCKS_PER_SEC */

# ifndef PACE_CLOCK_REALTIME
# define PACE_CLOCK_REALTIME CLOCK_REALTIME
# endif /* PACE_CLOCK_REALTIME */

# ifndef PACE_NULL
# define PACE_NULL NULL
# endif /* PACE_NULL */

# ifndef PACE_TIMER_ABSTIME
# define PACE_TIMER_ABSTIME TIMER_ABSTIME
# endif /* PACE_TIMER_ABSTIME */

#ifndef PACE_CLOCKID_T
#define PACE_CLOCKID_T 1
  typedef int clockid_t;
  typedef clockid_t pace_clockid_t;
#endif /* PACE_CLOCKID_T */

#ifndef PACE_CLOCK_T
#define PACE_CLOCK_T 1
  typedef clock_t pace_clock_t;
#endif /* PACE_CLOCK_T */

#ifndef PACE_TIMER_T
#define PACE_TIMER_T 1
  typedef long timer_t;
  typedef timer_t pace_timer_t;
#endif /* PACE_TIMER_T */

#ifndef PACE_TIME_T
#define PACE_TIME_T 1
  typedef time_t pace_time_t;
#endif /* PACE_TIME_T */

#ifndef PACE_TM
#define PACE_TM 1
  typedef struct tm pace_tm;
#endif /* PACE_TM */

#ifndef PACE_TIMESPEC
#define PACE_TIMESPEC 1
  typedef struct timespec pace_timespec;
#endif /* PACE_TIMESPEC */

#ifndef PACE_ITIMERSPEC
#define PACE_ITIMERSPEC 1
  typedef struct itimerspec pace_itimerspec;
#endif /* PACE_ITIMERSPEC */

#ifndef PACE_SIGVAL
#define PACE_SIGVAL 1
  typedef union sigval {
    int sival_int;                 /* integer value */
    void * sival_ptr;              /* pointer value */
} pace_sigval;
#endif  /* PACE_SIGVAL */

#ifndef PACE_SIGEVENT
#define PACE_SIGEVENT 1
  typedef struct sigevent {
    int sigev_notify;             /* notification mode */
    int sigev_signo;              /* signal number */
    union sigval sigev_value;     /* signal value */
    void (*sigev_notify_function)(union sigval);
    pace_pthread_attr_t * sigev_notify_attributes;
    int __sigev_pad2;
  } pace_sigevent;
#endif /* PACE_SIGEVENT */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_TIME_H_WIN32 */
