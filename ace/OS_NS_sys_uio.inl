// -*- C++ -*-
// $Id$

ACE_INLINE ssize_t
ACE_OS::readv (ACE_HANDLE handle,
               iovec *iov,
               int iovlen)
{
  ACE_OS_TRACE ("ACE_OS::readv");
#if defined (ACE_LACKS_READV)
  ACE_OSCALL_RETURN (ACE_OS::readv_emulation (handle, iov, iovlen),
                     ssize_t,
                     -1);
#else /* ACE_LACKS_READV */
  ACE_OSCALL_RETURN (::readv (handle, iov, iovlen), ssize_t, -1);
#endif /* ACE_LACKS_READV */
}

ACE_INLINE ssize_t
ACE_OS::writev (ACE_HANDLE handle,
                const iovec *iov,
                int iovcnt)
{
  ACE_OS_TRACE ("ACE_OS::writev");
#if defined (ACE_LACKS_WRITEV)
  ACE_OSCALL_RETURN (ACE_OS::writev_emulation (handle,
                                               (ACE_WRITEV_TYPE *) iov,
                                               iovcnt), int, -1);
#else /* ACE_LACKS_WRITEV */
  ACE_OSCALL_RETURN (::writev (handle,
                               (ACE_WRITEV_TYPE *) iov,
                               iovcnt), int, -1);
#endif /* ACE_LACKS_WRITEV */
}

