/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/sched.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SCHED_H_WIN32
#define PACE_SCHED_H_WIN32

#include "pace/time.h"

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#define PACE_SCHED_FIFO SCHED_FIFO
#define PACE_SCHED_OTHER SCHED_RR

#ifndef PACE_SCHED_PARAM
#define PACE_SCHED_PARAM
  typedef struct sched_param { int a_; } pace_sched_param;
#endif

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SCHED_H_WIN32 */
