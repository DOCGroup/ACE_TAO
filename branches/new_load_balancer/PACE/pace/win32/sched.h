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

#include <sched.h>
#include "pace/time.h"

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#define PACE_SCHED_FIFO SCHED_FIFO
#define PACE_SCHED_OTHER SCHED_RR

typedef struct sched_param pace_sched_param;

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SCHED_H_WIN32 */
