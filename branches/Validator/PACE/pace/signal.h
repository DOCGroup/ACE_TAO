/* $Id$

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/signal.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#ifndef PACE_SIGNAL_H
#define PACE_SIGNAL_H

#include "pace/config/defines.h"

#if (PACE_HAS_POSIX)
# include "pace/posix/signal.h"
#elif (PACE_VXWORKS)
# include "pace/vxworks/signal.h"
#elif (PACE_WIN32)
# include "pace/win32/signal.h"
#endif

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  /**
     PACE's implementation of the POSIX function kill.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.2.
   */
#if (PACE_HAS_POSIX_SIG_UOF)
  PACE_Export PACE_INLINE int pace_kill (pace_pid_t pid, int sig);
#endif /* PACE_HAS_POSIX_SIG_UOF */

  /**
     C std 7.14.2.1
   */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_raise (int sig);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function sigaction.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.4.
   */
#if (PACE_HAS_POSIX_SIG_UOF)
  PACE_Export PACE_INLINE int pace_sigaction (int sig,
                                              const pace_sigaction_s * act,
                                              pace_sigaction_s * oact);
#endif /* PACE_HAS_POSIX_SIG_UOF */

  /**
     PACE's implementation of the POSIX function sigaddset.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.3.
   */
#if (PACE_HAS_POSIX_SIG_UOF)
  PACE_Export PACE_INLINE int pace_sigaddset (pace_sigset_t * set, int signo);
#endif /* PACE_HAS_POSIX_SIG_UOF */

  /**
     PACE's implementation of the POSIX function sigemptyset.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.3.
   */
#if (PACE_HAS_POSIX_SIG_UOF)
  PACE_Export PACE_INLINE int pace_sigemptyset (pace_sigset_t * set);
#endif /* PACE_HAS_POSIX_SIG_UOF */

  /**
     PACE's implementation of the POSIX function sigdelset.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.3.
   */
#if (PACE_HAS_POSIX_SIG_UOF)
  PACE_Export PACE_INLINE int pace_sigdelset (pace_sigset_t * set, int signo);
#endif /* PACE_HAS_POSIX_SIG_UOF */

  /**
     PACE's implementation of the POSIX function sigfillset.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.3.
   */
#if (PACE_HAS_POSIX_SIG_UOF)
  PACE_Export PACE_INLINE int pace_sigfillset (pace_sigset_t * set);
#endif /* PACE_HAS_POSIX_SIG_UOF */

  /**
     PACE's implementation of the POSIX function sigismember.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.3.
   */
#if (PACE_HAS_POSIX_SIG_UOF)
  PACE_Export PACE_INLINE int pace_sigismember (const pace_sigset_t * set,
                                                int signo);
#endif /* PACE_HAS_POSIX_SIG_UOF */

  /**
     C std 7.14.1.1
   */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE pace_sig_pf pace_signal(int sig, pace_sig_pf func);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function sigpending.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.6.
   */
#if (PACE_HAS_POSIX_SIG_UOF)
  PACE_Export PACE_INLINE int pace_sigpending (pace_sigset_t * set);
#endif /* PACE_HAS_POSIX_SIG_UOF */

  /**
     PACE's implementation of the POSIX function sigprocmask.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.5.
   */
#if (PACE_HAS_POSIX_SIG_UOF)
  PACE_Export PACE_INLINE int pace_sigprocmask (int how,
                                                const pace_sigset_t * set,
                                                pace_sigset_t * oset);
#endif /* PACE_HAS_POSIX_SIG_UOF */

  /**
     PACE's implementation of the POSIX function sigqueue.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.9.
   */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_sigqueue (pace_pid_t pid,
                                             int signo,
                                             const pace_sigval value);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function sigsuspend.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.7.
   */
#if (PACE_HAS_POSIX_SIG_UOF)
  PACE_Export PACE_INLINE int pace_sigsuspend (const pace_sigset_t * sigmask);
#endif /* PACE_HAS_POSIX_SIG_UOF */

  /**
     PACE's implementation of the POSIX function sigtimedwait.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.8.
   */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_sigtimedwait (const pace_sigset_t * set,
                                                 pace_siginfo_t * info,
                                                 const pace_timespec * timeout);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

  /**
     PACE's implementation of the POSIX function sigwait.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.8.
   */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_sigwait (const pace_sigset_t * set,
                                            int * sig);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */
  /* Requires PACE_HAS_POSIX_PTHREAD_SEMANTICS. */

  /**
     PACE's implementation of the POSIX function sigwaitinfo.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 3.3.8.
   */
#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  PACE_Export PACE_INLINE int pace_sigwaitinfo (const pace_sigset_t * set,
                                                pace_siginfo_t * info);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if defined (PACE_HAS_INLINE)
#  if (PACE_HAS_POSIX)
#    include "pace/posix/signal.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/signal.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/signal.inl"
#  endif
#endif /* PACE_HAS_INLINE */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SIGNAL_H */
