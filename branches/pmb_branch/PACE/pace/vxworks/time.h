/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/time.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_TIME_H_VXWORKS
#define PACE_TIME_H_VXWORKS

#include <time.h>
#include "pace/signal.h"

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#define PACE_CLK_TCK CLK_TCK
#define PACE_CLOCKS_PER_SEC CLOCKS_PER_SEC
#define PACE_CLOCK_REALTIME CLOCK_REALTIME
#ifndef PACE_NULL
#define PACE_NULL NULL
#endif /* PACE_NULL */
#define PACE_TIMER_ABSTIME TIMER_ABSTIME

#ifndef PACE_CLOCKID_T
#define PACE_CLOCKID_T
# if defined PACE_EMU_CLOCKID_T
  typedef pace_emu_clockid_t pace_clockid_t;
# else
  typedef clockid_t pace_clockid_t;
# endif /* PACE_LINUX */
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

#ifndef PACE_SIGEVENT
#define PACE_SIGEVENT
  typedef struct sigevent pace_sigevent;
#endif /* PACE_SIGEVENT */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_TIME_H_VXWORKS */
