/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    limits.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_LIMITS_H
# define ACE_OS_INCLUDE_LIMITS_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

# if !defined (ACE_LACKS_LIMITS_H)
#   include /**/ <limits.h>
# endif /* ACE_LACKS_LIMITS_H */

// If we still have a problem with this, it can be put in a new
// file, e.g., ace_xopen_lim.h, that can be included by both ace_stdio.h
// and ace_limits.h
// This is defined by XOPEN to be a minimum of 16.  POSIX.1g
// also defines this value.  platform-specific config.h can
// override this if need be.
# if !defined (IOV_MAX)
#   define IOV_MAX 16
# endif /* IOV_MAX */

# if !defined (ACE_IOV_MAX)
#   define ACE_IOV_MAX IOV_MAX
# endif /* ACE_IOV_MAX */

// Need this before MAXNAMELEN--does it belong here?
#include "ace/os_include/sys/param.h"

# if !defined (MAXNAMELEN)
#   if !defined (ACE_MAXNAMELEN)
#     if defined (FILENAME_MAX)
#       define ACE_MAXNAMELEN FILENAME_MAX
#     else
#       define ACE_MAXNAMELEN 256
#     endif /* FILENAME_MAX */
#   endif /* ACE_MAXNAMELEN */
#   define MAXNAMELEN ACE_MAXNAMELEN
# endif /* MAXNAMELEN */

# if !defined (PIPE_BUF)
#   define PIPE_BUF 5120
# endif /* PIPE_BUF */

#endif /* ACE_OS_INCLUDELIMITS_H */
