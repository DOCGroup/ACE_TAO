// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_limits.h
 *
 *  implementation-defined constants
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_OS_LIMITS_H
#define ACE_OS_INCLUDE_OS_LIMITS_H

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
#  if defined (MAXNAMLEN)
#    define NAME_MAX MAXNAMLEN
#  elif defined (MAXNAMELEN)
#    define NAME_MAX MAXNAMELEN
#  elif defined (FILENAME_MAX)
#    define NAME_MAX FILENAME_MAX
#  elif defined (_MAX_FNAME)
#    define NAME_MAX _MAX_FNAME
#  else /* _MAX_FNAME */
#    define NAME_MAX 256
#  endif /* MAXNAMLEN */
#endif /* !NAME_MAX */

#if !defined (MAXNAMLEN)
#  define MAXNAMLEN NAME_MAX
#endif /* !MAXNAMLEN */

#if !defined (MAXNAMELEN)
#  define MAXNAMELEN NAME_MAX
#endif /* MAXNAMELEN */

#if !defined (HOST_NAME_MAX)
#  define HOST_NAME_MAX 256
#endif /* !HOST_NAME_MAX */

# if !defined(MAXHOSTNAMELEN)
#   define MAXHOSTNAMELEN  HOST_NAME_MAX
# endif /* MAXHOSTNAMELEN */

// Note that we are using PATH_MAX instead of _POSIX_PATH_MAX, since
// _POSIX_PATH_MAX is the *minimun* maximum value for PATH_MAX and is
// defined by POSIX as 256.
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

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_OS_LIMITS_H */
