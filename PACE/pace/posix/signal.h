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

#ifndef PACE_SIGNAL_H
#define PACE_SIGNAL_H

#include "pace/defines.h"
#include "pace/sys/types.h"
#include <signal.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  PACE_INLINE int pace_kill (pid_t pid, int sig);

  PACE_INLINE int pace_sigaction (int sig,
                                  const struct sigaction * act,
                                  struct sigaction * oact);

  PACE_INLINE int pace_sigaddset (sigset_t * set, int signo);

  PACE_INLINE int pace_sigemptyset (sigset_t * set);

  PACE_INLINE int pace_sigdelset (sigset_t * set, int signo);

  PACE_INLINE int pace_sigfillset (sigset_t * set);

  PACE_INLINE int pace_sigismember (const sigset_t * set, int signo);

  PACE_INLINE int pace_sigpending (sigset_t * set);

  PACE_INLINE int pace_sigprocmask (int how,
                                    const sigset_t * set,
                                    sigset_t * oset);

  PACE_INLINE int pace_sigqueue (pid_t pid,
                                 int signo,
                                 const union sigval value);

  PACE_INLINE int pace_sigsuspend (const sigset_t * sigmask);

  PACE_INLINE int pace_sigtimedwait (const sigset_t * set,
                                     siginfo_t * info,
                                     const struct timespec * timeout);

  PACE_INLINE int pace_sigwait (const sigset_t * set, int * sig);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  PACE_INLINE int pace_sigwaitinfo (const sigset_t * set, siginfo_t * info);

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/posix/signal.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_SIGNAL_H */
