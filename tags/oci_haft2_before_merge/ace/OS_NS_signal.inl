// -*- C++ -*-
// $Id$

#include "ace/OS_NS_macros.h"
#include "ace/OS_NS_errno.h"

ACE_INLINE int
ACE_OS::kill (pid_t pid, int signum)
{
  ACE_OS_TRACE ("ACE_OS::kill");
#if defined (ACE_WIN32) || defined (CHORUS) || defined (ACE_PSOS)
  ACE_UNUSED_ARG (pid);
  ACE_UNUSED_ARG (signum);
  ACE_NOTSUP_RETURN (-1);
#else
  ACE_OSCALL_RETURN (::kill (pid, signum), int, -1);
#endif /* ACE_WIN32 || CHORUS || ACE_PSOS */
}

ACE_INLINE int
ACE_OS::pthread_sigmask (int how, const sigset_t *nsp, sigset_t *osp)
{
#if defined (ACE_HAS_PTHREADS_STD)  &&  !defined (ACE_LACKS_PTHREAD_SIGMASK)
  ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_sigmask (how, nsp, osp),
                                       ace_result_),
                     int,
                     -1);
#else /* !ACE_HAS_PTHREADS_STD && !ACE_LACKS_PTHREAD_SIGMASK */
  ACE_UNUSED_ARG (how);
  ACE_UNUSED_ARG (nsp);
  ACE_UNUSED_ARG (osp);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_PTHREADS_STD && !ACE_LACKS_PTHREAD_SIGMASK */
}

ACE_INLINE int
ACE_OS::sigaction (int signum,
                   const struct sigaction *nsa,
                   struct sigaction *osa)
{
  ACE_OS_TRACE ("ACE_OS::sigaction");
  if (signum == 0)
    return 0;
#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)
  struct sigaction sa;

  if (osa == 0)
    osa = &sa;

  if (nsa == 0)
    {
      osa->sa_handler = ::signal (signum, SIG_IGN);
      ::signal (signum, osa->sa_handler);
    }
  else
    osa->sa_handler = ::signal (signum, nsa->sa_handler);
  return osa->sa_handler == SIG_ERR ? -1 : 0;
#elif defined (CHORUS) || defined (ACE_HAS_WINCE) || defined(ACE_PSOS)
  ACE_UNUSED_ARG (signum);
  ACE_UNUSED_ARG (nsa);
  ACE_UNUSED_ARG (osa);
  ACE_NOTSUP_RETURN (-1);
#elif defined (ACE_LACKS_POSIX_PROTOTYPES) || defined (ACE_LACKS_SOME_POSIX_PROTOTYPES)
  ACE_OSCALL_RETURN (::sigaction (signum, (struct sigaction*) nsa, osa), int, -1);
#else
  ACE_OSCALL_RETURN (::sigaction (signum, nsa, osa), int, -1);
#endif /* ACE_WIN32 !ACE_HAS_WINCE */
}

ACE_INLINE int
ACE_OS::sigaddset (sigset_t *s, int signum)
{
  ACE_OS_TRACE ("ACE_OS::sigaddset");
#if defined (ACE_LACKS_SIGSET) || defined (ACE_LACKS_SIGSET_DEFINITIONS)
  if (s == 0)
    {
      errno = EFAULT;
      return -1;
    }
  else if (signum < 1 || signum >= ACE_NSIG)
    {
      errno = EINVAL;
      return -1;                 // Invalid signum, return error
    }
#   if defined (ACE_PSOS) && defined (__DIAB) && ! defined(ACE_PSOS_DIAB_MIPS) && !defined (ACE_PSOS_DIAB_PPC)
  // treat 0th u_long of sigset_t as high bits,
  // and 1st u_long of sigset_t as low bits.
  if (signum <= ACE_BITS_PER_ULONG)
    s->s[1] |= (1 << (signum - 1));
  else
    s->s[0] |= (1 << (signum - ACE_BITS_PER_ULONG - 1));
#   else
  *s |= (1 << (signum - 1)) ;
#   endif /* defined (ACE_PSOS) && defined (__DIAB) */
  return 0 ;
#else
  ACE_OSCALL_RETURN (::sigaddset (s, signum), int, -1);
#endif /* ACE_LACKS_SIGSET || ACE_LACKS_SIGSET_DEFINITIONS */
}

ACE_INLINE int
ACE_OS::sigdelset (sigset_t *s, int signum)
{
#if defined (ACE_LACKS_SIGSET) || defined (ACE_LACKS_SIGSET_DEFINITIONS)
  if (s == 0)
    {
      errno = EFAULT;
      return -1;
    }
  else if (signum < 1 || signum >= ACE_NSIG)
    {
      errno = EINVAL;
      return -1;                 // Invalid signum, return error
    }
#   if defined (ACE_PSOS) && defined (__DIAB) && ! defined (ACE_PSOS_DIAB_MIPS) && !defined (ACE_PSOS_DIAB_PPC)
  // treat 0th u_long of sigset_t as high bits,
  // and 1st u_long of sigset_t as low bits.
  if (signum <= ACE_BITS_PER_ULONG)
    s->s[1] &= ~(1 << (signum - 1));
  else
    s->s[0] &= ~(1 << (signum - ACE_BITS_PER_ULONG - 1));
#   else
  *s &= ~(1 << (signum - 1)) ;
#   endif /* defined (ACE_PSOS) && defined (__DIAB) */
  return 0;
#else
  ACE_OSCALL_RETURN (::sigdelset (s, signum), int, -1);
#endif /* ACE_LACKS_SIGSET || ACE_LACKS_SIGSET_DEFINITIONS */
}

ACE_INLINE int
ACE_OS::sigemptyset (sigset_t *s)
{
#if defined (ACE_LACKS_SIGSET) || defined (ACE_LACKS_SIGSET_DEFINITIONS)
  if (s == 0)
    {
      errno = EFAULT;
      return -1;
    }
#   if defined (ACE_PSOS) && defined (__DIAB) && ! defined (ACE_PSOS_DIAB_MIPS) && !defined (ACE_PSOS_DIAB_PPC)
  s->s[0] = 0;
  s->s[1] = 0;
#   else
  *s = 0 ;
#   endif /* defined (ACE_PSOS) && defined (__DIAB) */
  return 0 ;
#else
  ACE_OSCALL_RETURN (::sigemptyset (s), int, -1);
#endif /* ACE_LACKS_SIGSET || ACE_LACKS_SIGSET_DEFINITIONS */
}

ACE_INLINE int
ACE_OS::sigfillset (sigset_t *s)
{
#if defined (ACE_LACKS_SIGSET) || defined (ACE_LACKS_SIGSET_DEFINITIONS)
  if (s == 0)
    {
      errno = EFAULT;
      return -1;
    }
#   if defined (ACE_PSOS) && defined (__DIAB) && ! defined (ACE_PSOS_DIAB_MIPS) && !defined (ACE_PSOS_DIAB_PPC)
  s->s[0] = ~(u_long) 0;
  s->s[1] = ~(u_long) 0;
#   else
  *s = ~(sigset_t) 0;
#   endif /* defined (ACE_PSOS) && defined (__DIAB) */
  return 0 ;
#else
  ACE_OSCALL_RETURN (::sigfillset (s), int, -1);
#endif /* ACE_LACKS_SIGSET || ACE_LACKS_SIGSET_DEFINITIONS */
}

ACE_INLINE int
ACE_OS::sigismember (sigset_t *s, int signum)
{
#if defined (ACE_LACKS_SIGSET) || defined (ACE_LACKS_SIGSET_DEFINITIONS)
  if (s == 0)
    {
      errno = EFAULT;
      return -1;
    }
  else if (signum < 1 || signum >= ACE_NSIG)
    {
      errno = EINVAL;
      return -1;                 // Invalid signum, return error
    }
#   if defined (ACE_PSOS) && defined (__DIAB) && ! defined (ACE_PSOS_DIAB_MIPS) && !defined (ACE_PSOS_DIAB_PPC)
  // treat 0th u_long of sigset_t as high bits,
  // and 1st u_long of sigset_t as low bits.
  if (signum <= ACE_BITS_PER_ULONG)
    return ((s->s[1] & (1 << (signum - 1))) != 0);
  else
    return ((s->s[0] & (1 << (signum - ACE_BITS_PER_ULONG - 1))) != 0);
#   else
  return ((*s & (1 << (signum - 1))) != 0) ;
#   endif /* defined (ACE_PSOS) && defined (__DIAB) */
#else
#  if defined (ACE_HAS_SIGISMEMBER_BUG)
  if (signum < 1 || signum >= ACE_NSIG)
    {
      errno = EINVAL;
      return -1;                 // Invalid signum, return error
    }
#  endif /* ACE_HAS_SIGISMEMBER_BUG */
  ACE_OSCALL_RETURN (::sigismember (s, signum), int, -1);
#endif /* ACE_LACKS_SIGSET || ACE_LACKS_SIGSET_DEFINITIONS */
}

ACE_INLINE ACE_SignalHandler
ACE_OS::signal (int signum, ACE_SignalHandler func)
{
  if (signum == 0)
    return 0;
  else
#if defined (ACE_PSOS) && !defined (ACE_PSOS_TM) && !defined (ACE_PSOS_DIAB_MIPS) && !defined (ACE_PSOS_DIAB_PPC)
    return (ACE_SignalHandler) ::signal (signum, (void (*)(void)) func);
#elif defined (ACE_PSOS_DIAB_MIPS) || defined (ACE_PSOS_DIAB_PPC)
    return 0;
#elif defined (ACE_PSOS_TM)
    // @@ It would be good to rework this so the ACE_PSOS_TM specific
    //    branch is not needed, but prying it out of ACE_LACKS_UNIX_SIGNALS
    //    will take some extra work - deferred for now.
    return (ACE_SignalHandler) ::signal (signum, (void (*)(int)) func);
#elif defined (ACE_WIN32) && !defined (ACE_HAS_WINCE) || !defined (ACE_LACKS_UNIX_SIGNALS)
#  if !defined (ACE_HAS_TANDEM_SIGNALS) && !defined (ACE_HAS_LYNXOS_SIGNALS)
    return ::signal (signum, func);
#  else
    return (ACE_SignalHandler) ::signal (signum, (void (*)(int)) func);
#  endif /* !ACE_HAS_TANDEM_SIGNALS */
#else
    // @@ WINCE: Don't know how to implement signal on WinCE (yet.)
    ACE_UNUSED_ARG (signum);
    ACE_UNUSED_ARG (func);
    ACE_NOTSUP_RETURN (0);     // Should return SIG_ERR but it is not defined on WinCE.
#endif /*  ACE_PSOS && !ACE_PSOS_TM && !ACE_PSOS_DIAB_MIPS && !ACE_PSOS_DIAB_PPC */
}

ACE_INLINE int
ACE_OS::sigprocmask (int how, const sigset_t *nsp, sigset_t *osp)
{
#if defined (ACE_LACKS_SIGSET) || defined (ACE_LACKS_SIGSET_DEFINITIONS)
  ACE_UNUSED_ARG (how);
  ACE_UNUSED_ARG (nsp);
  ACE_UNUSED_ARG (osp);
  ACE_NOTSUP_RETURN (-1);
#else
# if defined (ACE_LACKS_POSIX_PROTOTYPES)
  ACE_OSCALL_RETURN (::sigprocmask (how, (int*) nsp, osp), int, -1);
# else
  ACE_OSCALL_RETURN (::sigprocmask (how, nsp, osp), int, -1);
# endif /* ACE_LACKS_POSIX_PROTOTYPES */
#endif /* ACE_LACKS_SIGSET || ACE_LACKS_SIGSET_DEFINITIONS */
}

ACE_INLINE int
ACE_OS::sigsuspend (const sigset_t *sigset)
{
#if defined (ACE_HAS_SIGSUSPEND)
  sigset_t s;

  if (sigset == 0)
    {
      sigset = &s;
      ACE_OS::sigemptyset (&s);
    }
  ACE_OSCALL_RETURN (::sigsuspend (sigset), int, -1);
#else
  ACE_UNUSED_ARG (sigset);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_SIGSUSPEND */
}
