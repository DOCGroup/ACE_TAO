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
#define ACE_ACE_SYS_SIGNAL_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include /**/ <sys/signal.h>

// this doesn't belong here, but until I get an ace_aio.h file, here will be fine.
// Defining POSIX4 real-time signal range.
# if defined ACE_HAS_AIO_CALLS
#   define ACE_SIGRTMIN SIGRTMIN
#   define ACE_SIGRTMAX SIGRTMAX
# else /* !ACE_HAS_AIO_CALLS */
#   define ACE_SIGRTMIN 0
#   define ACE_SIGRTMAX 0
# endif /* ACE_HAS_AIO_CALLS */



#endif /* ACE_ACE_SYS_SIGNAL_H */
