/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    uio.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_SYS_UIO_H
# define ACE_OS_INCLUDE_SYS_UIO_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

# if !defined (ACE_LACKS_SYS_UIO_H) && !defined (ACE_LACKS_UNIX_SYS_HEADERS)
#   include /**/ <sys/uio.h>
# endif /* !ACE_LACKS_SYS_UIO_H && !ACE_LACKS_UNIX_SYS_HEADERS */

# if defined(__rtems__)
struct iovec {
  char *iov_base; // Base address.
  size_t iov_len; // Length.
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

#endif /* ACE_OS_INCLUDE_SYS_UIO_H */
