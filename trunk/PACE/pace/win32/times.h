/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/times.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SYS_TIMES_H
#define PACE_SYS_TIMES_H

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

  PACE_INLINE pace_clock_t pace_times (struct tms * buffer);

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/win32/times.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_SYS_TIMES_H */
