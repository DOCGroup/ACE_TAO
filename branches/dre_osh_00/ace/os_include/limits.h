// -*- C++ -*-

//=============================================================================
/**
 *  @file    limits.h
 *
 *  implementation-defined constants
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@ieee.org>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_LIMITS_H
#define ACE_OS_INCLUDE_LIMITS_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/unistd.h"

#if !defined (ACE_LACKS_LIMITS_H)
# include /**/ <limits.h>
#endif /* !ACE_LACKS_LIMITS_H */

#if !defined (ACE_LACKS_PARAM_H)
#  include /**/ <sys/param.h>
#endif /* ACE_LACKS_PARAM_H */

#if ! defined (howmany)
#  define howmany(x, y)   (((x)+((y)-1))/(y))
#endif /* howmany */

#if !defined (NAME_MAX)
#endif /* !NAME_MAX */

// Note that we are using PATH_MAX instead of _POSIX_PATH_MAX, since
// _POSIX_PATH_MAX is the *minimun* maximum value for PATH_MAX and is
// defined by POSIX as 255 (or 256 depending on how you read it).
#if !defined (PATH_MAX)
#  if defined (_MAX_PATH)
#    define PATH_MAX _MAX_PATH
#  else /* !_MAX_PATH */
#    define 1024
#  endif /* _MAX_PATH */
#endif /* !PATH_MAX */

// Leaving this for backward compatibility, but PATH_MAX should always be
// used directly.
#if !defined (MAXPATHLEN)
#  define MAXPATHLEN  PATH_MAX
#endif /* !MAXPATHLEN */

//#   define MAXNAMLEN   _MAX_FNAME
//#   define EADDRINUSE WSAEADDRINUSE

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_LIMITS_H */
