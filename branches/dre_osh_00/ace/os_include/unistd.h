// -*- C++ -*-

//=============================================================================
/**
 *  @file    unistd.h
 *
 *  standard symbolic constants and types
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@ieee.org>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_UNISTD_H
#define ACE_OS_INCLUDE_UNISTD_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/types.h"
#include "ace/os_include/inttypes.h"

#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)
#  include <process.h>
#  include <io.h>
#endif /* ACE_WIN32 && !ACE_HAS_WINCE */

#if !defined (ACE_LACKS_UNISTD_H)
# include /**/ <unistd.h>
#endif /* !ACE_LACKS_UNISTD_H */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_UNISTD_H */
