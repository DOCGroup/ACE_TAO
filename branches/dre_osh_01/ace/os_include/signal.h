// -*- C++ -*-

//=============================================================================
/**
 *  @file    signal.h
 *
 *  signals
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@ieee.org>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_SIGNAL_H
#define ACE_OS_INCLUDE_SIGNAL_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/types.h"

#if defined (ACE_HAS_SIGINFO_T)
#  if !defined (ACE_LACKS_SIGINFO_H)
#    if defined (__QNX__) || defined (__OpenBSD__)
#      include /**/ <sys/siginfo.h>
#    elif defined(__rtems__)
//#      include "ace/os_include/signal.h"
#    else  /* __QNX__ || __OpenBSD__ */
#      include /**/ <siginfo.h>
#    endif /* __QNX__ || __OpenBSD__ */
#  endif /* ACE_LACKS_SIGINFO_H */
#endif /* ACE_HAS_SIGINFO_T */

#include "ace/os_include/ucontext.h"
#include "ace/os_include/time.h"

#if !defined (ACE_LACKS_SINGNAL_H)
# include /**/ <signal.h>
#endif /* !ACE_LACKS_SIGNAL_H */

#if defined (ACE_HAS_SIG_MACROS)
#  undef sigemptyset
#  undef sigfillset
#  undef sigaddset
#  undef sigdelset
#  undef sigismember
#endif /* ACE_HAS_SIG_MACROS */

// This must come after signal.h is #included.  It's to counteract
// the sigemptyset and sigfillset #defines, which only happen
// when __OPTIMIZE__ is #defined (really!) on Linux.
#if defined (linux) && defined (__OPTIMIZE__)
#  undef sigemptyset
#  undef sigfillset
#endif /* linux && __OPTIMIZE__ */

#if !defined (ACE_HAS_SIG_ATOMIC_T)
   typedef int sig_atomic_t;
#endif /* !ACE_HAS_SIG_ATOMIC_T */

#if !defined (SIGHUP)
#  define SIGHUP 0
#endif /* SIGHUP */

#if !defined (SIGINT)
#  define SIGINT 0
#endif /* SIGINT */

#if !defined (SIGSEGV)
#  define SIGSEGV 0
#endif /* SIGSEGV */

#if !defined (SIGIO)
#  define SIGIO 0
#endif /* SIGSEGV */

#if !defined (SIGUSR1)
#  define SIGUSR1 0
#endif /* SIGUSR1 */

#if !defined (SIGUSR2)
#  define SIGUSR2 0
#endif /* SIGUSR2 */

#if !defined (SIGCHLD)
#  define SIGCHLD 0
#endif /* SIGCHLD */

#if !defined (SIGCLD)
#  define SIGCLD SIGCHLD
#endif /* SIGCLD */

#if !defined (SIGQUIT)
#  define SIGQUIT 0
#endif /* SIGQUIT */

#if !defined (SIGPIPE)
#  define SIGPIPE 0
#endif /* SIGPIPE */

#if !defined (SIGALRM)
#  define SIGALRM 0
#endif /* SIGALRM */

#if !defined (SIG_DFL)
#  if defined (ACE_PSOS_DIAB_MIPS) || defined (ACE_PSOS_DIAB_PPC)
#    define SIG_DFL ((void *) 0)
#  else
#    define SIG_DFL ((__sighandler_t) 0)
#  endif
#endif /* SIG_DFL */

#if !defined (SIG_IGN)
#  if defined (ACE_PSOS_DIAB_MIPS) || defined (ACE_PSOS_DIAB_PPC)
#    define SIG_IGN ((void *) 1)     /* ignore signal */
#  else
#    define SIG_IGN ((__sighandler_t) 1)     /* ignore signal */
#  endif
#endif /* SIG_IGN */

#if !defined (SIG_ERR)
#  if defined (ACE_PSOS_DIAB_MIPS) || defined (ACE_PSOS_DIAB_PPC)
#    define SIG_ERR ((void *) -1)    /* error return from signal */
#  else
#    define SIG_ERR ((__sighandler_t) -1)    /* error return from signal */
#  endif
#endif /* SIG_ERR */

// This must come after signal.h is #included.
#if defined (SCO)
#  define SIGIO SIGPOLL
#  include /**/ <sys/regset.h>  
#endif /* SCO */

// These are used by the <ACE_IPC_SAP::enable> and
// <ACE_IPC_SAP::disable> methods.  They must be unique and cannot
// conflict with the value of <ACE_NONBLOCK>.  We make the numbers
// negative here so they won't conflict with other values like SIGIO,
// etc.
# define ACE_SIGIO -1
# define ACE_SIGURG -2
# define ACE_CLOEXEC -3

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_SIGNAL_H */
