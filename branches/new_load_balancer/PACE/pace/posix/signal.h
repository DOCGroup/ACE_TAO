/* $Id$

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/signal.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#ifndef PACE_SIGNAL_H_POSIX
#define PACE_SIGNAL_H_POSIX

#include <signal.h>
#include "pace/sys/types.h"
#include "pace/time.h"

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#ifndef PACE_SIG_ATOMIC_T
#define PACE_SIG_ATOMIC_T
  typedef sig_atomic_t pace_sig_atomic_t;
#endif /* PACE_SIG_ATOMIC_T */

#ifndef PACE_SIGSET_T
#define PACE_SIGSET_T
  typedef sigset_t pace_sigset_t;
#endif /* PACE_SIGSET_T */

#ifndef PACE_S_SIGACTION
#define PACE_S_SIGACTION
  typedef struct sigaction pace_s_sigaction;
#endif /* PACE_S_SIGACTION */

#ifndef PACE_SIGINFO_T
#define PACE_SIGINFO_T
  typedef siginfo_t pace_siginfo_t;
#endif /* PACE_SIGINFO_T */

#ifndef PACE_SIGEVENT
#define PACE_SIGEVENT
  typedef struct sigevent pace_sigevent;
#endif /* PACE_SIGEVENT */

#ifndef PACE_SIGVAL
#define PACE_SIGVAL
  typedef union sigval pace_sigval;
#endif /* PACE_SIGVAL */

#ifndef PACE_TIMESPEC
#define PACE_TIMESPEC
  typedef struct timespec pace_timespec;
#endif /* PACE_TIMESPEC */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SIGNAL_H_POSIX */
