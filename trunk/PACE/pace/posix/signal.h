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

#include "pace/sys/types.h"
#include <signal.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  /** 
     PACE's implementation of the POSIX function kill.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.2.
   */
  PACE_INLINE int pace_kill (pid_t pid, int sig);

  /** 
     PACE's implementation of the POSIX function sigaction.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.4.
   */
  PACE_INLINE int pace_sigaction (int sig,
                                  const struct sigaction * act,
                                  struct sigaction * oact);

  /** 
     PACE's implementation of the POSIX function sigaddset.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.3.
   */
  PACE_INLINE int pace_sigaddset (sigset_t * set, int signo);

  /** 
     PACE's implementation of the POSIX function sigemptyset.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.3.
   */
  PACE_INLINE int pace_sigemptyset (sigset_t * set);

  /** 
     PACE's implementation of the POSIX function sigdelset.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.3.
   */
  PACE_INLINE int pace_sigdelset (sigset_t * set, int signo);

  /** 
     PACE's implementation of the POSIX function sigfillset.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.3.
   */
  PACE_INLINE int pace_sigfillset (sigset_t * set);

  /** 
     PACE's implementation of the POSIX function sigismember.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.3.
   */
  PACE_INLINE int pace_sigismember (const sigset_t * set, int signo);

  /** 
     PACE's implementation of the POSIX function sigpending.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.6.
   */
  PACE_INLINE int pace_sigpending (sigset_t * set);

  /** 
     PACE's implementation of the POSIX function sigprocmask.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.5.
   */
  PACE_INLINE int pace_sigprocmask (int how,
                                    const sigset_t * set,
                                    sigset_t * oset);

  /** 
     PACE's implementation of the POSIX function sigqueue.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.9.
   */
  PACE_INLINE int pace_sigqueue (pid_t pid,
                                 int signo,
                                 const union sigval value);

  /** 
     PACE's implementation of the POSIX function sigsuspend.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.7.
   */
  PACE_INLINE int pace_sigsuspend (const sigset_t * sigmask);

  /** 
     PACE's implementation of the POSIX function sigtimedwait.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.8.
   */
  PACE_INLINE int pace_sigtimedwait (const sigset_t * set,
                                     siginfo_t * info,
                                     const struct timespec * timeout);

  /** 
     PACE's implementation of the POSIX function sigwait.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.8.
   */
  PACE_INLINE int pace_sigwait (const sigset_t * set, int * sig);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  /** 
     PACE's implementation of the POSIX function sigwaitinfo.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.8.
   */
  PACE_INLINE int pace_sigwaitinfo (const sigset_t * set, siginfo_t * info);

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/posix/signal.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_SIGNAL_H */
