/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/signal.inl
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================= */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_kill (pace_pid_t pid, int sig)
{
  PACE_TRACE("pace_kill");

  return kill (pid, sig);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_raise (int sig)
{
  PACE_TRACE("pace_raise");

  return raise (sig);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_sigaction (int sig, const pace_sigaction_s * act,
                pace_sigaction_s * oact)
{
  PACE_TRACE("pace_sigaction");

  return sigaction (sig, act, oact);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_sigaddset (pace_sigset_t * set, int signo)
{
  PACE_TRACE("pace_sigaddset");

  return sigaddset (set, signo);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_sigemptyset (pace_sigset_t * set)
{
  PACE_TRACE("pace_sigemptyset");

  return sigemptyset (set);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_sigdelset (pace_sigset_t * set, int signo)
{
  PACE_TRACE("pace_sigdelset");

  return sigdelset (set, signo);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_sigfillset (pace_sigset_t * set)
{
  PACE_TRACE("pace_sigfillset");

  return sigfillset (set);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_sigismember (const pace_sigset_t * set, int signo)
{
  PACE_TRACE("pace_sigismember");

  return sigismember (set, signo);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
pace_sig_pf
pace_signal (int sig, pace_sig_pf func)
{
  PACE_TRACE("pace_signal");

  return signal (sig, func);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_sigpending (pace_sigset_t * set)
{
  PACE_TRACE("pace_sigpending");

  return sigpending (set);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_sigprocmask (int how, const pace_sigset_t * set,
                  pace_sigset_t * oset)
{
  PACE_TRACE("pace_sigprocmask");

  return sigprocmask (how, set, oset);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sigqueue (pace_pid_t pid, int signo,
               const pace_sigval value)
{
  PACE_TRACE("pace_sigqueue");

  return sigqueue (pid, signo, value);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_sigsuspend (const pace_sigset_t * sigmask)
{
  PACE_TRACE("pace_sigsuspend");

  return sigsuspend (sigmask);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sigtimedwait (const pace_sigset_t * set, pace_siginfo_t * info,
                   const pace_timespec * timeout)
{
  PACE_TRACE("pace_sigtimedwait");

  return sigtimedwait (set, info, timeout);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sigwait (const pace_sigset_t * set, int * sig)
{
  PACE_TRACE("pace_sigwait");

  /* It appears that sigwait is not supported for 5.3.1 or 5.4.
   * ACE uses sigtimedwait instead. (The code below is directly
   * from ACE_OS.
   * This is the call that should be made if VxWorks supported
   * sigwait:
   PACE_UNUSED_ARG (sig);
   return sigwait (set);

   * Second arg is a struct siginfo *, which we don't need (the
   * selected signal number is returned).  Third arg is timeout:  0
   * means forever.
   */
  *sig = sigtimedwait (set, 0, 0);
  return *sig;
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sigwaitinfo (const pace_sigset_t *set, pace_siginfo_t *info)
{
  PACE_TRACE("pace_sigwaitinfo");

  return sigwaitinfo (set, info);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */
