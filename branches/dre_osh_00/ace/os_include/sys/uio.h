// -*- C++ -*-

//=============================================================================
/**
 *  @file    uio.h
 *
 *  definitions for vector I/O operations
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@ieee.org>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_SYS_UIO_H
#define ACE_OS_INCLUDE_SYS_UIO_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/types.h"

#if !defined (ACE_LACKS_SYS_UIO_H)
# include /**/ <sys/uio.h>
#endif /* !ACE_LACKS_SYS_UIO_H */

// todo: make this a regular ACE_LACKS macro test...
# if defined(__rtems__)
struct iovec {
  /// Base address.
  char *iov_base;
  /// Length.
  size_t iov_len;
};
# endif

# if defined (ACE_HAS_BROKEN_WRITEV)
typedef struct iovec ACE_WRITEV_TYPE;
# else
typedef const struct iovec ACE_WRITEV_TYPE;
# endif /* ACE_HAS_BROKEN_WRITEV */

# if defined (ACE_HAS_BROKEN_READV)
typedef const struct iovec ACE_READV_TYPE;
# else
typedef struct iovec ACE_READV_TYPE;
# endif /* ACE_HAS_BROKEN_READV */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_SYS_UIO_H */
