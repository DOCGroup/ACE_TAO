// -*- C++ -*-

//=============================================================================
/**
 *  @file    select.h
 *
 *  select types
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@ieee.org>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_SYS_SELECT_H
#define ACE_OS_INCLUDE_SYS_SELECT_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/time.h"
#include "ace/os_include/signal.h"

#if !defined (ACE_LACKS_SYS_SELECT_H)
# include /**/ <sys/select.h>
#endif /* !ACE_LACKS_SYS_SELECT_H */

#if defined (ACE_WIN32)
// This will help until we figure out everything:
#   define NFDBITS 32 /* only used in unused functions... */
#endif /* ACE_WIN32 */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_SYS_SELECT_H */
