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

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_SIGNAL_H */
