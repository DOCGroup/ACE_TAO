/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/schedtypes.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

/* Keep sched types defined here as they need to be in more than one
 * file and otherwise cause circular dependencies.

 * Do NOT include this file directly!! Please include "pace/sched.h". */

#ifndef PACE_SYS_SCHEDTYPES_H_WIN32
#define PACE_SYS_SCHEDTYPES_H_WIN32

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  /* These are defined via POSIX, but win32 is not POSIX compliant!
   * So please forgive me for these (non pace prefix) sins! */

#ifndef SCHED_OTHER
#define SCHED_OTHER 0
#endif /* SCHED_RR */

#ifndef SCHED_FIFO
#define SCHED_FIFO 1
#endif /* SCHED_FIFO */

#ifndef SCHED_RR
#define SCHED_RR 2
#endif /* SCHED_RR */

#ifndef PACE_SCHED_OTHER
#define PACE_SCHED_OTHER SCHED_OTHER
#endif /* PACE_SCHED_OTHER */

#ifndef PACE_SCHED_FIFO
#define PACE_SCHED_FIFO SCHED_FIFO
#endif /* PACE_SCHED_FIFO */

#ifndef PACE_SCHED_RR
#define PACE_SCHED_RR SCHED_RR
#endif /* PACE_SCHED_RR */

#ifndef PACE_SCHED_PARAM
#define PACE_SCHED_PARAM 1
  typedef struct sched_param
  {
    int sched_priority;
  } pace_sched_param;
#endif

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SYS_SCHEDTYPES_H_WIN32 */
