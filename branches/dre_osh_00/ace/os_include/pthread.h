// -*- C++ -*-

//=============================================================================
/**
 *  @file    pthread.h
 *
 *  threads
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@ieee.org>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_PTHREAD_H
#define ACE_OS_INCLUDE_PTHREAD_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/types.h"

// This needs to go here *first* to avoid problems with AIX.
# if defined (ACE_HAS_PTHREADS)
//extern "C" {
#   define ACE_DONT_INCLUDE_ACE_SIGNAL_H
#     include "ace/os_include/signal.h"
#   undef ACE_DONT_INCLUDE_ACE_SIGNAL_H
  //#   include "ace/os_include/pthread.h"
#   if defined (DIGITAL_UNIX)
#     define pthread_self __pthread_self
extern "C" pthread_t pthread_self (void);
#   endif /* DIGITAL_UNIX */
//}
#   if defined (HPUX_10)
//    HP-UX 10 needs to see cma_sigwait, and since _CMA_NOWRAPPERS_ is defined,
//    this header does not get included from pthreads.h.
#     include /**/x <dce/cma_sigwait.h>
#   endif /* HPUX_10 */
# endif /* ACE_HAS_PTHREADS */


#if !defined (ACE_LACKS_PTHREAD_H)
# include /**/ <pthread.h>
#endif /* !ACE_LACKS_PTHREAD_H */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_PTHREAD_H */
