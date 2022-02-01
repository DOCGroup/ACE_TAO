// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_NS_signal.h
 *
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 */
//=============================================================================

#ifndef ACE_OS_NS_SIGNAL_H
# define ACE_OS_NS_SIGNAL_H

# include /**/ "ace/pre.h"

# include "ace/config-lite.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/os_signal.h"
#include /**/ "ace/ACE_export.h"

#if defined (ACE_EXPORT_MACRO)
#  undef ACE_EXPORT_MACRO
#endif
#define ACE_EXPORT_MACRO ACE_Export

#if defined (__Lynx__) || defined (__OpenBSD__)
// LynxOS and OpenBSD define pthread_sigmask() in pthread.h
# include "ace/os_include/os_pthread.h"
#endif /* __Lynx__ || OpenBSD */

/*
 * We inline and undef some functions that may be implemented
 * as macros on some platforms. This way macro definitions will
 * be usable later as there is no way to save the macro definition
 * using the pre-processor.
 */

#if !defined (ACE_LACKS_SIGSET)
inline int ace_sigemptyset_helper (sigset_t *s)
{
#  if defined (sigemptyset)
  return sigemptyset (s);
#  undef sigemptyset
#  else
  return ::sigemptyset (s);
#  endif /* defined (sigemptyset) */
}

inline int ace_sigfillset_helper (sigset_t *s)
{
#  if defined (sigfillset)
  return sigfillset (s);
#  undef sigfillset
#  else
  return ::sigfillset (s);
#  endif /* defined (sigfillset) */
}

inline int ace_sigaddset_helper (sigset_t *s, int signum)
{
#  if defined (sigaddset)
  return sigaddset (s, signum);
#  undef sigaddset
#  else
  return ::sigaddset (s, signum);
#  endif /* defined (sigaddset) */
}

inline int ace_sigdelset_helper (sigset_t *s, int signum)
{
#  if defined (sigdelset)
  return sigdelset (s, signum);
#  undef sigdelset
#  else
  return ::sigdelset (s, signum);
#  endif /* defined (sigdelset) */
}

inline int ace_sigismember_helper (sigset_t *s, int signum)
{
#  if defined (sigismember)
  return sigismember (s, signum);
#  undef sigismember
#  else
  return ::sigismember (s, signum);
#  endif /* defined (sigismember) */
}
#endif /* !defined (ACE_LACKS_SIGSET) */

#if defined (ACE_HAS_SIGSUSPEND)
inline int ace_sigsuspend_helper (const sigset_t *s)
{
#  if defined (sigsuspend)
  return sigsuspend (s);
#  undef sigsuspend
#  else
  return ::sigsuspend (s);
#  endif /* defined (sigsuspen) */
}
#endif /* ACE_HAS_SIGSUSPEND */

# if !defined (SIG_BLOCK)
#   define SIG_BLOCK   1
# endif /* SIG_BLOCK   */

# if !defined (SIG_UNBLOCK)
#   define SIG_UNBLOCK 2
# endif /* SIG_UNBLOCK */

# if !defined (SIG_SETMASK)
#   define SIG_SETMASK 3
# endif /* SIG_SETMASK */

// Create some useful typedefs.

# if !defined (ACE_HAS_SIGINFO_T)
struct ACE_Export siginfo_t
{
  siginfo_t (ACE_HANDLE handle);

  /// Win32 HANDLE that has become signaled.
  ACE_HANDLE si_handle_;
};
# endif /* ACE_HAS_SIGINFO_T */

#if !defined (ACE_WIN32)
extern "C"
{
  typedef void (*ACE_SIGNAL_C_FUNC)(int, siginfo_t *, void *);
}
#endif /* ACE_WIN32 */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

typedef struct sigaction ACE_SIGACTION;

namespace ACE_OS {

  //@{ @name A set of wrappers for Signals.

  ACE_NAMESPACE_INLINE_FUNCTION
  int kill (pid_t pid,
            int signum);

#ifdef pthread_sigmask
# define ACE_PTHREAD_SIGMASK_MACRO pthread_sigmask
# undef pthread_sigmask
#endif
  ACE_NAMESPACE_INLINE_FUNCTION
  int pthread_sigmask (int how,
                       const sigset_t *nsp,
                       sigset_t *osp);

  ACE_NAMESPACE_INLINE_FUNCTION
  int sigaction (int signum,
                 const ACE_SIGACTION *nsa,
                 ACE_SIGACTION *osa);

  ACE_NAMESPACE_INLINE_FUNCTION
  int sigaddset (sigset_t *s,
                 int signum);

  ACE_NAMESPACE_INLINE_FUNCTION
  int sigdelset (sigset_t *s,
                 int signum);

  ACE_NAMESPACE_INLINE_FUNCTION
  int sigemptyset (sigset_t *s);

  ACE_NAMESPACE_INLINE_FUNCTION
  int sigfillset (sigset_t *s);

  ACE_NAMESPACE_INLINE_FUNCTION
  int sigismember (sigset_t *s,
                   int signum);

  ACE_NAMESPACE_INLINE_FUNCTION
  ACE_SignalHandler signal (int signum,
                            ACE_SignalHandler);

  ACE_NAMESPACE_INLINE_FUNCTION
  int sigprocmask (int how,
                   const sigset_t *nsp,
                   sigset_t *osp);

  ACE_NAMESPACE_INLINE_FUNCTION
  int sigsuspend (const sigset_t *s);

  ACE_NAMESPACE_INLINE_FUNCTION
  int raise (const int signum);
  //@}

} /* namespace ACE_OS */

ACE_END_VERSIONED_NAMESPACE_DECL

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_NS_signal.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

# include /**/ "ace/post.h"
#endif /* ACE_OS_NS_SIGNAL_H */
