// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_wait.h
 *
 *  declarations for waiting
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_SYS_OS_WAIT_H
#define ACE_OS_INCLUDE_SYS_OS_WAIT_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/signal.h"
#include "ace/os_include/sys/resource.h"

#if !defined (ACE_LACKS_SYS_WAIT_H)
#  include /**/ <sys/wait.h>
#endif /* !ACE_LACKS_SYS_WAIT_H */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_SYS_OS_WAIT_H */
