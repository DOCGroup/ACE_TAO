// -*- C++ -*-
#include "ace/OS_NS_macros.h"
#include "ace/OS_NS_errno.h"
#include "ace/os_include/os_pthread.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE_OS
{

ACE_INLINE int
kill (pid_t pid, int signum)
{
  ACE_OS_TRACE ("ACE_OS::kill");
#if defined (ACE_LACKS_KILL)
  ACE_UNUSED_ARG (pid);
  ACE_UNUSED_ARG (signum);
  ACE_NOTSUP_RETURN (-1);
#elif defined (ACE_VXWORKS)
  /*
   * The VxWorks kill interface is not really POSIX
   * since they use a task id in place of a pid type.
   * This only becomes an issue when using the 64bit compiler
   * as the TASK_ID is no longer defined as an int.
   */
  return ::kill ((ACE_VX_TASK_ID)pid, signum);
#else
  return ::kill (pid, signum);
#endif /* ACE_LACKS_KILL */
}

ACE_INLINE int
pthread_sigmask (int how, const sigset_t *nsp, sigset_t *osp)
{
#if defined (ACE_HAS_PTHREADS) && !defined (ACE_LACKS_PTHREAD_SIGMASK)
  int result;
# ifdef ACE_PTHREAD_SIGMASK_MACRO
  ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (ACE_PTHREAD_SIGMASK_MACRO (how, nsp, osp), result), int);
# elif defined (ACE_HAS_NONCONST_PTHREAD_SIGMASK)
  sigset_t *ncnsp = const_cast<sigset_t *>(nsp);
  ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_sigmask (how, ncnsp, osp), result), int);
# else
  ACE_OSCALL_RETURN (ACE_ADAPT_RETVAL (::pthread_sigmask (how, nsp, osp), result), int);
# endif /* ACE_HAS_NONCONST__PTHREAD_SIGMASK */
#else /* !ACE_HAS_PTHREADS && !ACE_LACKS_PTHREAD_SIGMASK */
  ACE_UNUSED_ARG (how);
  ACE_UNUSED_ARG (nsp);
  ACE_UNUSED_ARG (osp);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_PTHREADS && !ACE_LACKS_PTHREAD_SIGMASK */
}

ACE_INLINE int
sigaction (int signum, const ACE_SIGACTION *nsa, ACE_SIGACTION *osa)
{
  ACE_OS_TRACE ("ACE_OS::sigaction");
  if (signum == 0)
    return 0;
#if defined (ACE_WIN32)
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
#elif defined (ACE_LACKS_SIGACTION) || (defined (ACE_INTEGRITY) && defined (ACE_LACKS_POSIX))
  // INTEGRITY has the sigaction struct but the sigaction call may still be unavailable.
  ACE_UNUSED_ARG (nsa);
  ACE_UNUSED_ARG (osa);
  ACE_NOTSUP_RETURN (-1);
#elif !defined (ACE_HAS_SIGACTION_CONSTP2)
  return ::sigaction (signum, const_cast<ACE_SIGACTION*> (nsa), osa);
#else
  return ::sigaction (signum, nsa, osa);
#endif /* ACE_WIN32 */
}

ACE_INLINE int
sigaddset (sigset_t *s, int signum)
{
  ACE_OS_TRACE ("ACE_OS::sigaddset");
#if defined (ACE_LACKS_SIGADDSET)
  ACE_UNUSED_ARG (s);
  ACE_UNUSED_ARG (signum);
  ACE_NOTSUP_RETURN (-1);
#elif defined (ACE_LACKS_SIGSET)
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
  *s |= (1 << (signum - 1)) ;
  return 0 ;
#else
  return ace_sigaddset_helper (s, signum);
#endif
}

ACE_INLINE int
sigdelset (sigset_t *s, int signum)
{
#if defined (ACE_LACKS_SIGDELSET)
  ACE_UNUSED_ARG (s);
  ACE_UNUSED_ARG (signum);
  ACE_NOTSUP_RETURN (-1);
#elif defined (ACE_LACKS_SIGSET)
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
  *s &= ~(1 << (signum - 1)) ;
  return 0;
#else
  return ace_sigdelset_helper (s, signum);
#endif
}

ACE_INLINE int
sigemptyset (sigset_t *s)
{
#if defined (ACE_LACKS_SIGEMPTYSET)
  ACE_UNUSED_ARG (s);
  ACE_NOTSUP_RETURN (-1);
#elif defined (ACE_LACKS_SIGSET)
  if (s == 0)
    {
      errno = EFAULT;
      return -1;
    }
  *s = 0 ;
  return 0;
#else
  return ace_sigemptyset_helper (s);
#endif
}

ACE_INLINE int
sigfillset (sigset_t *s)
{
#if defined (ACE_LACKS_SIGFILLSET)
  ACE_UNUSED_ARG (s);
  ACE_NOTSUP_RETURN (-1);
#elif defined (ACE_LACKS_SIGSET)
  if (s == 0)
    {
      errno = EFAULT;
      return -1;
    }
  *s = ~(sigset_t) 0;
  return 0 ;
#else
  return ace_sigfillset_helper (s);
#endif
}

ACE_INLINE int
sigismember (sigset_t *s, int signum)
{
#if defined (ACE_LACKS_SIGISMEMBER)
  ACE_UNUSED_ARG (s);
  ACE_UNUSED_ARG (signum);
  ACE_NOTSUP_RETURN (-1);
#elif defined (ACE_LACKS_SIGSET)
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
  return ((*s & (1 << (signum - 1))) != 0) ;
#else
#  if defined (ACE_HAS_SIGISMEMBER_BUG)
  if (signum < 1 || signum >= ACE_NSIG)
    {
      errno = EINVAL;
      return -1;                 // Invalid signum, return error
    }
#  endif /* ACE_HAS_SIGISMEMBER_BUG */
  return ace_sigismember_helper (s, signum);
#endif
}

ACE_INLINE ACE_SignalHandler
signal (int signum, ACE_SignalHandler func)
{
  if (signum == 0)
    return 0;
  else
    {
#if (defined ACE_WIN32) || \
    (!defined ACE_LACKS_UNIX_SIGNALS && !defined ACE_LACKS_SIGNAL)
      return ::signal (signum, func);
#else
      ACE_UNUSED_ARG (signum);
      ACE_UNUSED_ARG (func);
      ACE_NOTSUP_RETURN (0);     // Should return SIG_ERR but maybe not defined on all platforms
#endif /* defined (ACE_WIN32) || !defined (ACE_LACKS_UNIX_SIGNALS) */
    }
}

ACE_INLINE int
sigprocmask (int how, const sigset_t *nsp, sigset_t *osp)
{
#if defined (ACE_LACKS_SIGSET) || defined (ACE_LACKS_SIGSET_DEFINITIONS) || defined (ACE_LACKS_SIGPROCMASK)
  ACE_UNUSED_ARG (how);
  ACE_UNUSED_ARG (nsp);
  ACE_UNUSED_ARG (osp);
  ACE_NOTSUP_RETURN (-1);
#else
  return ::sigprocmask (how, nsp, osp);
#endif /* ACE_LACKS_SIGSET || ACE_LACKS_SIGSET_DEFINITIONS || ACE_LACKS_SIGPROCMASK */
}

ACE_INLINE int
sigsuspend (const sigset_t *s)
{
#if defined (ACE_HAS_SIGSUSPEND)
  sigset_t sigset;

  if (s == 0)
    {
      ACE_OS::sigemptyset (&sigset);
      s = &sigset;
    }
  return ace_sigsuspend_helper (s);
#else
  ACE_UNUSED_ARG (s);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_SIGSUSPEND */
}

ACE_INLINE int
raise (const int signum)
{
#if defined (ACE_LACKS_RAISE)
  ACE_UNUSED_ARG (signum);
  ACE_NOTSUP_RETURN (-1);
#else
  return ::raise (signum);
#endif /* ACE_LACKS_RAISE */
}

}  /* end namespace ACE_OS */

ACE_END_VERSIONED_NAMESPACE_DECL
