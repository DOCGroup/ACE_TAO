/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    selct.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_SYS_SELECT_H
# define ACE_OS_INCLUDE_SYS_SELECT_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

// Only include if platform/compiler provide the header.
# if !defined (ACE_LACKS_SYS_SELECT_H)
#   include /**/ <sys/select.h>
# endif /* !ACE_LACKS_SYS_SELECT_H */

# if defined (ACE_NEEDS_SELECT_T) // __rtems__
extern "C"
{
  int select (int n, fd_set *readfds, fd_set *writefds,
              fd_set *exceptfds, const struct timeval *timeout);
};
# endif /* ACE_NEEDS_SELECT_T */

# if !defined (NFDBITS)
#   define NFDBITS (sizeof(fd_mask) * NBBY)        /* bits per mask */
# endif /* ! NFDBITS */

#endif /* ACE_OS_INCLUDE_SYS_SELECT_H */
