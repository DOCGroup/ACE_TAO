/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/times.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SYS_TIMES_H_POSIX
#define PACE_SYS_TIMES_H_POSIX

#include <sys/times.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#ifndef PACE_CLOCK_T
#define PACE_CLOCK_T
typedef clock_t pace_clock_t;
#endif /* PACE_CLOCK_T */

#ifndef PACE_TMS
#define PACE_TMS
typedef struct tms pace_tms;
#endif /* PACE_TMS */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SYS_TIMES_H_POSIX */
