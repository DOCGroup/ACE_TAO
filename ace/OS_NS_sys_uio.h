// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_NS_sys_uio.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 *
 *  Originally in OS.h.
 */
//=============================================================================

#ifndef ACE_OS_NS_SYS_UIO_H
# define ACE_OS_NS_SYS_UIO_H

# include /**/ "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/os_uio.h"

namespace ACE_OS {

  ssize_t readv (ACE_HANDLE handle,
                 iovec *iov,
                 int iovlen);

#if defined (ACE_LACKS_READV)
  ssize_t readv_emulation (ACE_HANDLE handle,
                           ACE_READV_TYPE *iov,
                           int iovcnt);
#endif /* ACE_LACKS_READV */

  ssize_t writev (ACE_HANDLE handle,
                  const iovec *iov,
                  int iovcnt);

  // these don't need to be in the header, better to put them in the cpp
  //  or inl.
#if defined (ACE_LACKS_WRITEV)
  int writev_emulation (ACE_HANDLE handle,
                        ACE_WRITEV_TYPE *iov,
                        int iovcnt);
#endif /* ACE_LACKS_WRITEV */

} /* namespace ACE_OS */

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_NS_sys_uio.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

# include /**/ "ace/post.h"
#endif /* ACE_OS_NS_SYS_UIO_H */
