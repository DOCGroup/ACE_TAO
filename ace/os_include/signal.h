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

# if defined (ACE_HAS_SIG_MACROS)
#   undef sigemptyset
#   undef sigfillset
#   undef sigaddset
#   undef sigdelset
#   undef sigismember
# endif /* ACE_HAS_SIG_MACROS */

// This must come after signal.h is #included.  It's to counteract
// the sigemptyset and sigfillset #defines, which only happen
// when __OPTIMIZE__ is #defined (really!) on Linux.
# if defined (linux) && defined (__OPTIMIZE__)
#   undef sigemptyset
#   undef sigfillset
# endif /* linux && __OPTIMIZE__ */

# if !defined (ACE_HAS_SIG_ATOMIC_T)
typedef int sig_atomic_t;
# endif /* !ACE_HAS_SIG_ATOMIC_T */

// This must come after signal.h is #included.
# if defined (SCO)
#   define SIGIO SIGPOLL
#   include /**/ <sys/regset.h>  
# endif /* SCO */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_SIGNAL_H */
