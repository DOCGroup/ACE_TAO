// -*- C++ -*-
#include "ace/os_include/os_errno.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE ssize_t
ACE_OS::readv (ACE_HANDLE handle,
               const iovec *iov,
               int iovlen)
{
  ACE_OS_TRACE ("ACE_OS::readv");
#if defined (ACE_LACKS_READV)
  return ACE_OS::readv_emulation (handle, iov, iovlen);
#else /* ACE_LACKS_READV */
#if defined (ACE_HAS_NONCONST_READV)
  return ::readv (handle, const_cast<iovec *>(iov), iovlen);
#else
  return ::readv (handle, iov, iovlen);
#endif /* ACE_HAS_NONCONST_READV */
#endif /* ACE_LACKS_READV */
}

ACE_INLINE ssize_t
ACE_OS::writev (ACE_HANDLE handle,
                const iovec *iov,
                int iovcnt)
{
  ACE_OS_TRACE ("ACE_OS::writev");
#if defined (ACE_LACKS_WRITEV)
  return ACE_OS::writev_emulation (handle, iov, iovcnt);
#else /* ACE_LACKS_WRITEV */
#if defined (ACE_HAS_NONCONST_WRITEV)
  return ::writev (handle, const_cast<iovec *>(iov), iovcnt);
#else
  return ::writev (handle, iov, iovcnt);
#endif /* ACE_HAS_NONCONST_WRITEV */
#endif /* ACE_LACKS_WRITEV */
}

ACE_END_VERSIONED_NAMESPACE_DECL
