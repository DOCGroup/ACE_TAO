/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/signal.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

PACE_INLINE
int
pace_kill (pid_t pid, int sig)
{
  return kill (pid, sig);
}

PACE_INLINE
int
pace_sigaction (int sig, const struct sigaction * act,
                struct sigaction * oact)
{
  return sigaction (sig, act, oact);
}

PACE_INLINE
int
pace_sigaddset (sigset_t * set, int signo)
{
  return sigaddset (set, signo);
}

PACE_INLINE
int
pace_sigemptyset (sigset_t * set)
{
  return sigemptyset (set);
}

PACE_INLINE
int
pace_sigdelset (sigset_t * set, int signo)
{
  return sigdelset (set, signo);
}

PACE_INLINE
int
pace_sigfillset (sigset_t * set)
{
  return sigfillset (set);
}

PACE_INLINE
int
pace_sigismember (const sigset_t * set, int signo)
{
  return sigismember (set, signo);
}

PACE_INLINE
int
pace_sigpending (sigset_t * set)
{
  return sigpending (set);
}

PACE_INLINE
int
pace_sigprocmask (int how, const sigset_t * set,
                  sigset_t * oset)
{
  return sigprocmask (how, set, oset);
}

PACE_INLINE
int
pace_sigqueue (pid_t pid, int signo,
               const union sigval value)
{
  return sigqueue (pid, signo, value);
}

PACE_INLINE
int
pace_sigsuspend (const sigset_t * sigmask)
{
  return sigsuspend (sigmask);
}

PACE_INLINE
int
pace_sigtimedwait (const sigset_t * set, siginfo_t * info,
                   const struct timespec * timeout)
{
  return sigtimedwait (set, info, timeout);
}

PACE_INLINE
int
pace_sigwait (const sigset_t * set, int * sig)
{
# if defined (PACE_HAS_POSIX_PTHREAD_SEMANTICS)
  return sigwait (set, sig);
# else /* !PACE_HAS_POSIX_PTHREAD_SEMANTICS */
  PACE_RETURN_NO_SUPPORT (PACE_ERRNO_LACKS_POSIX_PTHREAD_SEMANTICS);
# endif /* !PACE_HAS_POSIX_PTHREAD_SEMANTICS */
}

PACE_INLINE
int
pace_sigwaitinfo (const sigset_t *set, siginfo_t *info)
{
  return sigwaitinfo (set, info);
}
