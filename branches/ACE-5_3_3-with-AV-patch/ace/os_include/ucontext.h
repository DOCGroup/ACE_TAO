// -*- C++ -*-

//=============================================================================
/**
 *  @file    ucontext.h
 *
 *  user context
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@ieee.org>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_UCONTEXT_H
#define ACE_OS_INCLUDE_UCONTEXT_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// @todo need to define sigset_t both here and in signal.h, but 
// there is a circular dependancy since signal.h needs to include ucontext.h
//#include "ace/os_include/signal.h"

#if !defined (ACE_LACKS_UCONTEXT_H)
# include /**/ <ucontext.h>
#endif /* !ACE_LACKS_ucontext_H */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_UCONTEXT_H */
