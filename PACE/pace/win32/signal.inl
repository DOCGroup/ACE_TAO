/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/signal.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#include "pace/pthread.h"

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_kill (pace_pid_t pid, int sig)
{
  return kill (pid, sig);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_raise (int sig)
{
  /* Implementation from POSIX std 8.1:54 */
  return pthread_kill (pthread_self (), sig);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_sigaction (int sig, const pace_s_sigaction * act,
                pace_s_sigaction * oact)
{
  return sigaction (sig, act, oact);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_sigaddset (pace_sigset_t * set, int signo)
{
  return sigaddset (set, signo);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_sigemptyset (pace_sigset_t * set)
{
  return sigemptyset (set);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_sigdelset (pace_sigset_t * set, int signo)
{
  return sigdelset (set, signo);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_sigfillset (pace_sigset_t * set)
{
  return sigfillset (set);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_sigismember (const pace_sigset_t * set, int signo)
{
  return sigismember (set, signo);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
void
(*pace_signal(int sig, void (*func)(int)))(int)
{
  return signal (sig, func);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_sigpending (pace_sigset_t * set)
{
  return sigpending (set);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_sigprocmask (int how, const pace_sigset_t * set,
                  pace_sigset_t * oset)
{
  return sigprocmask (how, set, oset);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sigqueue (pace_pid_t pid, int signo,
               const pace_sigval value)
{
  return sigqueue (pid, signo, value);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_sigsuspend (const pace_sigset_t * sigmask)
{
  return sigsuspend (sigmask);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sigtimedwait (const pace_sigset_t * set, pace_siginfo_t * info,
                   const pace_timespec * timeout)
{
  return sigtimedwait (set, info, timeout);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sigwait (const pace_sigset_t * set, int * sig)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return sigwait (set, sig);
# else /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_UNUSED_ARG (set);
  PACE_UNUSED_ARG (sig);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
# endif /* ! PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sigwaitinfo (const pace_sigset_t *set, pace_siginfo_t *info)
{
  return sigwaitinfo (set, info);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */
