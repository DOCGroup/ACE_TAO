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
#include "ace/os_include/ucontext.h"
#include "ace/os_include/time.h"

#if !defined (ACE_LACKS_SINGNAL_H)
# include /**/ <signal.h>
#endif /* !ACE_LACKS_SIGNAL_H */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_SIGNAL_H */
