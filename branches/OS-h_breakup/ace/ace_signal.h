/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_signal.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_SIGNAL_H
# define ACE_ACE_SIGNAL_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

# include "ace/ace_sys_types.h"

# if !defined (ACE_LACKS_SIGNAL_H)
#   include /**/ <signal.h>
# endif /* !ACE_LACKS_SIGNAL_H */

// Defining POSIX4 real-time signal range.
# if defined ACE_HAS_AIO_CALLS
#   define ACE_SIGRTMIN SIGRTMIN
#   define ACE_SIGRTMAX SIGRTMAX
# else /* !ACE_HAS_AIO_CALLS */
#   define ACE_SIGRTMIN 0
#   define ACE_SIGRTMAX 0
# endif /* ACE_HAS_AIO_CALLS */

# if defined (ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES)
// Prototypes for both signal() and struct sigaction are consistent..
#   if defined (ACE_HAS_SIG_C_FUNC)
extern "C" {
#   endif /* ACE_HAS_SIG_C_FUNC */
#   if !defined (ACE_PSOS)
typedef void (*ACE_SignalHandler)(int);
typedef void (*ACE_SignalHandlerV)(int);
#   endif /* !defined (ACE_PSOS) */
#   if defined (ACE_HAS_SIG_C_FUNC)
}
#   endif /* ACE_HAS_SIG_C_FUNC */
# elif defined (ACE_HAS_LYNXOS_SIGNALS)
typedef void (*ACE_SignalHandler)(...);
typedef void (*ACE_SignalHandlerV)(...);
# elif defined (ACE_HAS_TANDEM_SIGNALS)
typedef void (*ACE_SignalHandler)(...);
typedef void (*ACE_SignalHandlerV)(...);
# elif defined (ACE_HAS_IRIX_53_SIGNALS)
typedef void (*ACE_SignalHandler)(...);
typedef void (*ACE_SignalHandlerV)(...);
# elif defined (ACE_HAS_SPARCWORKS_401_SIGNALS)
typedef void (*ACE_SignalHandler)(int, ...);
typedef void (*ACE_SignalHandlerV)(int,...);
# elif defined (ACE_HAS_SUNOS4_SIGNAL_T)
typedef void (*ACE_SignalHandler)(...);
typedef void (*ACE_SignalHandlerV)(...);
# elif defined (ACE_HAS_SVR4_SIGNAL_T)
// SVR4 Signals are inconsistent (e.g., see struct sigaction)..
typedef void (*ACE_SignalHandler)(int);
#   if !defined (m88k)     /*  with SVR4_SIGNAL_T */
typedef void (*ACE_SignalHandlerV)(void);
#   else
typedef void (*ACE_SignalHandlerV)(int);
#   endif  /*  m88k */       /*  with SVR4_SIGNAL_T */
# elif defined (ACE_WIN32)
typedef void (__cdecl *ACE_SignalHandler)(int);
typedef void (__cdecl *ACE_SignalHandlerV)(int);
# elif defined (ACE_HAS_UNIXWARE_SVR4_SIGNAL_T)
typedef void (*ACE_SignalHandler)(int);
typedef void (*ACE_SignalHandlerV)(...);
# else /* This is necessary for some older broken version of cfront */
#   if defined (SIG_PF)
#     define ACE_SignalHandler SIG_PF
#   else
typedef void (*ACE_SignalHandler)(int);
#   endif /* SIG_PF */
typedef void (*ACE_SignalHandlerV)(...);
# endif /* ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES */

// This must come after signal.h is #included.
# if defined (SCO)
#   define SIGIO SIGPOLL
#   include /**/ <sys/regset.h>
# endif /* SCO */

# if defined (ACE_HAS_SIGINFO_T)
#   if !defined (ACE_LACKS_SIGINFO_H)
#     if defined (__QNX__)
#       include /**/ <sys/siginfo.h>
#     else  /* ! __QNX__ */
#       include /**/ <siginfo.h>
#     endif /* ! __QNX__ */
#   endif /* ACE_LACKS_SIGINFO_H */
#   if !defined (ACE_LACKS_UCONTEXT_H)
#     include /**/ <ucontext.h>
#   endif /* ACE_LACKS_UCONTEXT_H */
# else
#   include "ace/OS_Export.h"
struct ACE_OS_Export siginfo_t
{
  siginfo_t (ACE_HANDLE handle);
  siginfo_t (ACE_HANDLE *handles);      // JCEJ 12/23/96

  ACE_HANDLE si_handle_;
  // Win32 HANDLE that has become signaled.

  ACE_HANDLE *si_handles_;
  // Array of Win32 HANDLEs all of which have become signaled.
};
# endif /* ACE_HAS_SIGINFO_T */

// Typedef for the null handler func.
extern "C"
{
  typedef void (*ACE_SIGNAL_C_FUNC)(int,siginfo_t*,void*);
}

# if !defined (ACE_HAS_UCONTEXT_T)
typedef int ucontext_t;
# endif /* ACE_HAS_UCONTEXT_T */

# if !defined (SA_SIGINFO)
#   define SA_SIGINFO 0
# endif /* SA_SIGINFO */

# if !defined (SA_RESTART)
#   define SA_RESTART 0
# endif /* SA_RESTART */

# if defined (ACE_HAS_TIMOD_H)
#   if defined (ACE_HAS_STL_QUEUE_CONFLICT)
#     define queue _Queue_
#   endif /* ACE_HAS_STL_QUEUE_CONFLICT */
#   include /**/ <sys/timod.h>
#   if defined (ACE_HAS_STL_QUEUE_CONFLICT)
#     undef queue
#   endif /* ACE_HAS_STL_QUEUE_CONFLICT */
# elif defined (ACE_HAS_OSF_TIMOD_H)
#   include /**/ <tli/timod.h>
# endif /* ACE_HAS_TIMOD_H */

# if defined (ACE_LACKS_SIGSET_T)
typedef u_int sigset_t;
# endif /* ACE_LACKS_SIGSET_T */

// Type of the extended signal handler.
typedef void (*ACE_Sig_Handler_Ex) (int, siginfo_t *siginfo, ucontext_t *ucontext);

# if defined (DIGITAL_UNIX)
  // sigwait is yet another macro on Digital UNIX 4.0, just causing
  // trouble when introducing member functions with the same name.
  // Thanks to Thilo Kielmann" <kielmann@informatik.uni-siegen.de> for
  // this fix.
#   if defined  (__DECCXX_VER)
#     undef sigwait
      // cxx on Digital Unix 4.0 needs this declaration.  With it,
      // <::_Psigwait> works with cxx -pthread.  g++ does _not_ need
      // it.
      extern "C" int _Psigwait __((const sigset_t *set, int *sig));
#   elif defined (__KCC)
#     undef sigwait
      inline int sigwait (const sigset_t* set, int* sig)
        { return _Psigwait (set, sig); }
#   endif /* __DECCXX_VER */
# elif !defined (ACE_HAS_SIGWAIT)
#   if defined(__rtems__)
  extern "C" int sigwait (const sigset_t *set, int *sig);
#   else
   extern "C" int sigwait (sigset_t *set);
#   endif /* __rtems__ */
# endif /* ! DIGITAL_UNIX && ! ACE_HAS_SIGWAIT */

# if defined (ACE_LACKS_SIGACTION_T)
struct sigaction
{
  int sa_flags;
  ACE_SignalHandlerV sa_handler;
  sigset_t sa_mask;
};
# endif /* ACE_LACKS_SIGACTION_T */

# if !defined (SIGHUP)
#   define SIGHUP 0
# endif /* SIGHUP */

# if !defined (SIGINT)
#   define SIGINT 0
# endif /* SIGINT */

# if !defined (SIGSEGV)
#   define SIGSEGV 0
# endif /* SIGSEGV */

# if !defined (SIGIO)
#   define SIGIO 0
# endif /* SIGSEGV */

# if !defined (SIGUSR1)
#   define SIGUSR1 0
# endif /* SIGUSR1 */

# if !defined (SIGUSR2)
#   define SIGUSR2 0
# endif /* SIGUSR2 */

# if !defined (SIGCHLD)
#   define SIGCHLD 0
# endif /* SIGCHLD */

# if !defined (SIGCLD)
#   define SIGCLD SIGCHLD
# endif /* SIGCLD */

# if !defined (SIGQUIT)
#   define SIGQUIT 0
# endif /* SIGQUIT */

# if !defined (SIGPIPE)
#   define SIGPIPE 0
# endif /* SIGPIPE */

# if !defined (SIGALRM)
#   define SIGALRM 0
# endif /* SIGALRM */

# if !defined (SIG_DFL)
#   if defined (ACE_PSOS_DIAB_MIPS) || defined (ACE_PSOS_DIAB_PPC)
#     define SIG_DFL ((void *) 0)
#   else
#     define SIG_DFL ((__sighandler_t) 0)
#   endif
# endif /* SIG_DFL */

# if !defined (SIG_IGN)
#   if defined (ACE_PSOS_DIAB_MIPS) || defined (ACE_PSOS_DIAB_PPC)
#     define SIG_IGN ((void *) 1)     /* ignore signal */
#   else
#     define SIG_IGN ((__sighandler_t) 1)     /* ignore signal */
#   endif
# endif /* SIG_IGN */

# if !defined (SIG_ERR)
#   if defined (ACE_PSOS_DIAB_MIPS) || defined (ACE_PSOS_DIAB_PPC)
#     define SIG_ERR ((void *) -1)    /* error return from signal */
#   else
#     define SIG_ERR ((__sighandler_t) -1)    /* error return from signal */
#   endif
# endif /* SIG_ERR */

# if !defined (SIG_BLOCK)
#   define SIG_BLOCK   1
# endif /* SIG_BLOCK   */

# if !defined (SIG_UNBLOCK)
#   define SIG_UNBLOCK 2
# endif /* SIG_UNBLOCK */

# if !defined (SIG_SETMASK)
#   define SIG_SETMASK 3
# endif /* SIG_SETMASK */

# if defined (__Lynx__)
    // LynxOS Neutrino sets NSIG to the highest-numbered signal.
#   define ACE_NSIG (NSIG + 1)
# elif defined (__rtems__)
#   define ACE_NSIG (SIGRTMAX)
# else
  // All other platforms set NSIG to one greater than the
  // highest-numbered signal.
#   define ACE_NSIG NSIG
# endif /* __Lynx__ */

// These are used by the <ACE_IPC_SAP::enable> and
// <ACE_IPC_SAP::disable> methods.  They must be unique and cannot
// conflict with the value of <ACE_NONBLOCK>.  We make the numbers
// negative here so they won't conflict with other values like SIGIO,
// etc.
# define ACE_SIGIO -1
# define ACE_SIGURG -2
# define ACE_CLOEXEC -3

typedef const char **SYS_SIGLIST;

#endif /* ACE_ACE_SIGNAL_H */
