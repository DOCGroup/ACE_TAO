// -*- C++ -*-
//
// $Id$

#include "ace/OS_NS_unistd.h"
#include "ace/Truncate.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Send an n byte message to the connected socket.

ACE_INLINE
ACE_SOCK_IO::ACE_SOCK_IO (void)
{
  // ACE_TRACE ("ACE_SOCK_IO::ACE_SOCK_IO");
}

ACE_INLINE
ACE_SOCK_IO::~ACE_SOCK_IO (void)
{
  // ACE_TRACE ("ACE_SOCK_IO::~ACE_SOCK_IO");
}

ACE_INLINE ssize_t
ACE_SOCK_IO::recv (void *buf,
                   size_t len,
                   int flags,
                   const ACE_Time_Value *timeout) const
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
                   const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_SOCK_IO::recv");
  return ACE::recv (this->get_handle (),
                    buf,
                    len,
                    timeout);
}

ACE_INLINE ssize_t
ACE_SOCK_IO::recvv (iovec iov[],
                    size_t n,
                    const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_SOCK_IO::recvv");
  return ACE::recvv (this->get_handle (),
                     iov,
                     ACE_Utils::Truncate<size_t> (n),
                     timeout);
}

ACE_INLINE ssize_t
ACE_SOCK_IO::recv (iovec iov[],
                   size_t n,
                   const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_SOCK_IO::recv");
  return this->recvv (iov,
                      n,
                      timeout);
}

ACE_INLINE ssize_t
ACE_SOCK_IO::recv (iovec *io_vec,
                   const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_SOCK_IO::recv");
  return this->recvv (io_vec,
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

ACE_INLINE ssize_t
ACE_SOCK_IO::send (const void *buf,
                   size_t len,
                   int flags,
                   const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_SOCK_IO::send");
  return ACE::send (this->get_handle (),
                    buf,
                    len,
                    flags,
                    timeout);
}

ACE_INLINE ssize_t
ACE_SOCK_IO::send (const void *buf,
                   size_t len,
                   const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_SOCK_IO::send");
  return ACE::send (this->get_handle (),
                    buf,
                    len,
                    timeout);
}

ACE_INLINE ssize_t
ACE_SOCK_IO::sendv (const iovec iov[],
                    size_t n,
                    const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_SOCK_IO::sendv");
  return ACE::sendv (this->get_handle (),
                     iov,
                     ACE_Utils::Truncate<size_t> (n),
                     timeout);
}

ACE_INLINE ssize_t
ACE_SOCK_IO::send (const iovec iov[],
                   size_t n,
                   const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_SOCK_IO::send");
  return this->sendv (iov,
                      n,
                      timeout);
}

ACE_INLINE ssize_t
ACE_SOCK_IO::send (const void *buf,
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
