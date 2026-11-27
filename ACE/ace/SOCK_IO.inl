#include "ace/OS_NS_unistd.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
ACE_SOCK_IO::ACE_SOCK_IO ()
{
  // ACE_TRACE ("ACE_SOCK_IO::ACE_SOCK_IO");
}

ACE_INLINE
ACE_SOCK_IO::~ACE_SOCK_IO ()
{
  // ACE_TRACE ("ACE_SOCK_IO::~ACE_SOCK_IO");
}

ACE_INLINE ssize_t
ACE_SOCK_IO::recv (void *buf,
                   size_t len,
                   int flags,
                   ACE_Time_Value const *timeout) const
{
  ACE_TRACE ("ACE_SOCK_IO::recv");
  return ACE::recv (this->get_handle (),
                    buf,
                    len,
                    flags,
                    timeout);
}

ACE_INLINE ssize_t
ACE_SOCK_IO::recv (void *buf,
                   size_t len,
                   ACE_Time_Value const *timeout) const
{
  ACE_TRACE ("ACE_SOCK_IO::recv");
  return ACE::recv (this->get_handle (),
                    buf,
                    len,
                    timeout);
}

ACE_INLINE ssize_t
ACE_SOCK_IO::recvv (iovec iov[],
                    int n,
                    ACE_Time_Value const *timeout) const
{
  ACE_TRACE ("ACE_SOCK_IO::recvv");
  return ACE::recvv (this->get_handle (),
                     iov,
                     n,
                     timeout);
}

ACE_INLINE ssize_t
ACE_SOCK_IO::recv (void *buf,
                   size_t n,
                   ACE_OVERLAPPED *overlapped) const
{
  ACE_TRACE ("ACE_SOCK_IO::recv");
  return ACE_OS::read (this->get_handle (),
                       (char *) buf,
                       n,
                       overlapped);
}

/// Send an n byte message to the connected socket.
ACE_INLINE ssize_t
ACE_SOCK_IO::send (void const *buf,
                   size_t len,
                   int flags,
                   ACE_Time_Value const *timeout) const
{
  ACE_TRACE ("ACE_SOCK_IO::send");
  return ACE::send (this->get_handle (),
                    buf,
                    len,
                    flags,
                    timeout);
}

ACE_INLINE ssize_t
ACE_SOCK_IO::send (void const *buf,
                   size_t len,
                   ACE_Time_Value const *timeout) const
{
  ACE_TRACE ("ACE_SOCK_IO::send");
  return ACE::send (this->get_handle (),
                    buf,
                    len,
                    timeout);
}

ACE_INLINE ssize_t
ACE_SOCK_IO::sendv (const iovec iov[],
                    int n,
                    ACE_Time_Value const *timeout) const
{
  ACE_TRACE ("ACE_SOCK_IO::sendv");
  return ACE::sendv (this->get_handle (),
                     iov,
                     n,
                     timeout);
}

ACE_INLINE ssize_t
ACE_SOCK_IO::send (void const *buf,
                   size_t n,
                   ACE_OVERLAPPED *overlapped) const
{
  ACE_TRACE ("ACE_SOCK_IO::send");
  return ACE_OS::write (this->get_handle (),
                        buf,
                        n,
                        overlapped);
}

ACE_END_VERSIONED_NAMESPACE_DECL
