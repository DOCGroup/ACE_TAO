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

#ifndef PACE_SIG_PF
# define PACE_SIG_PF
# if (PACE_LYNXOS)
#  if defined (PACE_HAS_CPLUSPLUS)
  typedef void (*pace_sig_pf)(...);
#  else
  typedef void (*pace_sig_pf)();
#  endif /* PACE_HAS_CPLUSPLUS */
# else
  typedef void (*pace_sig_pf)(int);
# endif /* PACE_LYNXOS */
#endif /* PACE_SIG_PF */

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

#ifndef PACE_SIGACTION_S
#define PACE_SIGACTION_S
  typedef struct sigaction pace_sigaction_s;
#endif /* PACE_SIGACTION_S */

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
