// -*- C++ -*-

//=============================================================================
/**
 *  @file    aio.h 
 *  
 *  asynchronous input and output (REALTIME)
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@ieee.org>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_AIO_H
#define ACE_OS_INCLUDE_AIO_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Inclusion of the <aio.h> header may make visible symbols defined in 
// the headers <fcntl.h>, <signal.h>, <sys/types.h>, and <time.h>.

#include "ace/os_include/signal.h" // for sigevent

#if !defined (ACE_LACKS_AIO_H)
# include /**/ <aio.h>
#endif /* !ACE_LACKS_AIO_H */


#endif /* ACE_OS_INCLUDE_AIO_H */
