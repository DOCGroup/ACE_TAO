/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/sched.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SCHED_H_POSIX
#define PACE_SCHED_H_POSIX

#include <sched.h>
#include "pace/time.h"

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#define PACE_SCHED_FIFO SCHED_FIFO
#define PACE_SCHED_OTHER SCHED_RR

#ifndef PACE_SCHED_PARAM
#define PACE_SCHED_PARAM
  typedef struct sched_param pace_sched_param;
#endif /* PACE_SCHED_PARAM */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SCHED_H_POSIX */
