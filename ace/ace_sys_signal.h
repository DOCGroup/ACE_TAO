/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_sys_signal.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_SYS_SIGNAL_H
# define ACE_ACE_SYS_SIGNAL_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */




# if !defined (ACE_LACKS_SYS_SIGNAL_H) && !defined (ACE_LACKS_UNIX_SYS_HEADERS)
#   include /**/ <sys/signal.h>
# endif /* !ACE_LACKS_SYS_SIGNAL_H && !ACE_LACKS_UNIX_SYS_HEADERS */

// this doesn't belong here(?), but until I get an ace_aio.h file, here will be fine.
// Defining POSIX4 real-time signal range.
# if defined ACE_HAS_AIO_CALLS
#   define ACE_SIGRTMIN SIGRTMIN
#   define ACE_SIGRTMAX SIGRTMAX
# else /* !ACE_HAS_AIO_CALLS */
#   define ACE_SIGRTMIN 0
#   define ACE_SIGRTMAX 0
# endif /* ACE_HAS_AIO_CALLS */

// Need to get rid of specific platforms below!!!
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




# if !defined (ACE_HAS_SIGINFO_T)
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


#   if defined (ACE_HAS_SIGINFO_T)
#     if !defined (ACE_LACKS_SIGINFO_H)
#       if defined (__QNX__)
#         include /**/ <sys/siginfo.h>
#       elif defined(__rtems__)
#         include /**/ <signal.h>
#       else  /* ! __QNX__ */
#         include /**/ <siginfo.h>
#       endif /* ! __QNX__ */
#     endif /* ACE_LACKS_SIGINFO_H */
#     if !defined (ACE_LACKS_UCONTEXT_H)
#       include /**/ <ucontext.h>
#     endif /* ACE_LACKS_UCONTEXT_H */
#   endif /* ACE_HAS_SIGINFO_T */


// Type of the extended signal handler.
typedef void (*ACE_Sig_Handler_Ex) (int, siginfo_t *siginfo, ucontext_t *ucontext);



#endif /* ACE_ACE_SYS_SIGNAL_H */
