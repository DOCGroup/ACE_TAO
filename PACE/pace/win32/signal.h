/* $Id$

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/signal.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#ifndef PACE_SIGNAL_H
#define PACE_SIGNAL_H

#include "pace/sys/types.h"
#include "pace/time.h"
#include <signal.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  typedef sig_atomic_t pace_sig_atomic_t;
  typedef sigset_t pace_sigset_t;
  typedef struct sigaction pace_s_sigaction;
  typedef siginfo_t pace_siginfo_t;
  typedef struct sigevent pace_sigevent;
  typedef union sigval pace_sigval;

  /**
     PACE's implementation of the POSIX function kill.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.2.
   */
  PACE_INLINE int pace_kill (pace_pid_t pid, int sig);

  /**
     C std 7.14.2.1
   */
  PACE_INLINE int pace_raise (int sig);

  /**
     PACE's implementation of the POSIX function sigaction.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.4.
   */
  PACE_INLINE int pace_sigaction (int sig,
                                  const pace_s_sigaction * act,
                                  pace_s_sigaction * oact);

  /**
     PACE's implementation of the POSIX function sigaddset.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.3.
   */
  PACE_INLINE int pace_sigaddset (pace_sigset_t * set, int signo);

  /**
     PACE's implementation of the POSIX function sigemptyset.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.3.
   */
  PACE_INLINE int pace_sigemptyset (pace_sigset_t * set);

  /**
     PACE's implementation of the POSIX function sigdelset.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.3.
   */
  PACE_INLINE int pace_sigdelset (pace_sigset_t * set, int signo);

  /**
     PACE's implementation of the POSIX function sigfillset.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.3.
   */
  PACE_INLINE int pace_sigfillset (pace_sigset_t * set);

  /**
     PACE's implementation of the POSIX function sigismember.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.3.
   */
  PACE_INLINE int pace_sigismember (const pace_sigset_t * set, int signo);

  /**
     C std 7.14.1.1
   */
  PACE_INLINE void (*pace_signal(int sig, void (*func)(int)))(int);

  /**
     PACE's implementation of the POSIX function sigpending.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.6.
   */
  PACE_INLINE int pace_sigpending (pace_sigset_t * set);

  /**
     PACE's implementation of the POSIX function sigprocmask.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.5.
   */
  PACE_INLINE int pace_sigprocmask (int how,
                                    const pace_sigset_t * set,
                                    pace_sigset_t * oset);

  /**
     PACE's implementation of the POSIX function sigqueue.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.9.
   */
  PACE_INLINE int pace_sigqueue (pace_pid_t pid,
                                 int signo,
                                 const pace_sigval value);

  /**
     PACE's implementation of the POSIX function sigsuspend.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.7.
   */
  PACE_INLINE int pace_sigsuspend (const pace_sigset_t * sigmask);

  /**
     PACE's implementation of the POSIX function sigtimedwait.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.8.
   */
  PACE_INLINE int pace_sigtimedwait (const pace_sigset_t * set,
                                     pace_siginfo_t * info,
                                     const pace_timespec * timeout);

  /**
     PACE's implementation of the POSIX function sigwait.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.8.
   */
  PACE_INLINE int pace_sigwait (const pace_sigset_t * set, int * sig);
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  /**
     PACE's implementation of the POSIX function sigwaitinfo.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.8.
   */
  PACE_INLINE int pace_sigwaitinfo (const pace_sigset_t * set, pace_siginfo_t * info);

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#if defined (PACE_HAS_INLINE)
# include "pace/win32/signal.inl"
#endif /* PACE_HAS_INLINE */

#endif /* PACE_SIGNAL_H */
