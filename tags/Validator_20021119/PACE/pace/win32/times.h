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

#ifndef PACE_SYS_TIMES_H_WIN32
#define PACE_SYS_TIMES_H_WIN32

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#ifndef PACE_CLOCK_T
#define PACE_CLOCK_T
  /* Temporary until we emulate this correctly. */
  typedef int clock_t;
  typedef clock_t pace_clock_t;
#endif /* PACE_CLOCK_T */

#ifndef PACE_TMS
#define PACE_TMS
  /* Temporary until we emulate this correctly. */
  typedef struct tms { int a_; } pace_tms;
#endif /* PACE_TMS */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SYS_TIMES_H_WIN32 */
