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

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_kill (pace_pid_t pid, int sig)
{
  PACE_UNUSED_ARG (pid);
  PACE_UNUSED_ARG (sig);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_raise (int sig)
{
  PACE_UNUSED_ARG (sig);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_sigaction (int sig, const pace_sigaction_s * act,
                pace_sigaction_s * oact)
{
  return sigaction (sig, act, oact);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_sigaddset (pace_sigset_t * set, int signo)
{
  PACE_UNUSED_ARG (set);
  PACE_UNUSED_ARG (signo);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_sigemptyset (pace_sigset_t * set)
{
  PACE_UNUSED_ARG (set);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_sigdelset (pace_sigset_t * set, int signo)
{
  PACE_UNUSED_ARG (set);
  PACE_UNUSED_ARG (signo);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_sigfillset (pace_sigset_t * set)
{
  /* PACE_SIG_BITS is an enum defined in PACE/pace/win32/signal.inl */
  int i = 0;
  for ( ; i < PACE_SIG_BITS; ++i)
  {
    set->sigbits_[i] = ~(unsigned int)0;
  }
  return 0;
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_sigismember (const pace_sigset_t * set, int signo)
{
  PACE_UNUSED_ARG (set);
  PACE_UNUSED_ARG (signo);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
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
  PACE_UNUSED_ARG (set);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_sigprocmask (int how, const pace_sigset_t * set,
                  pace_sigset_t * oset)
{
  PACE_UNUSED_ARG (how);
  PACE_UNUSED_ARG (set);
  PACE_UNUSED_ARG (oset);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sigqueue (pace_pid_t pid, int signo,
               const pace_sigval value)
{
  PACE_UNUSED_ARG (pid);
  PACE_UNUSED_ARG (signo);
  PACE_UNUSED_ARG (value);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_SIG_UOF)
PACE_INLINE
int
pace_sigsuspend (const pace_sigset_t * sigmask)
{
  PACE_UNUSED_ARG (sigmask);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_SIG_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sigtimedwait (const pace_sigset_t * set, pace_siginfo_t * info,
                   const pace_timespec * timeout)
{
  PACE_UNUSED_ARG (set);
  PACE_UNUSED_ARG (info);
  PACE_UNUSED_ARG (timeout);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sigwait (const pace_sigset_t * set, int * sig)
{
  PACE_UNUSED_ARG (set);
  PACE_UNUSED_ARG (sig);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
PACE_INLINE
int
pace_sigwaitinfo (const pace_sigset_t * set, pace_siginfo_t * info)
{
  PACE_UNUSED_ARG (set);
  PACE_UNUSED_ARG (info);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */
