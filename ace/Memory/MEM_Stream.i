/* -*- C++ -*- */
// $Id$

// MEM_Stream.i

#include "ace/Memory/MEM_Stream.h"

ASYS_INLINE
ACE_MEM_Stream::ACE_MEM_Stream (void)
{
  // ACE_TRACE ("ACE_MEM_Stream::ACE_MEM_Stream");
}

ASYS_INLINE
ACE_MEM_Stream::ACE_MEM_Stream (ACE_HANDLE h)
{
  // ACE_TRACE ("ACE_MEM_Stream::ACE_MEM_Stream");
  this->set_handle (h);
}

ASYS_INLINE
ACE_MEM_Stream::~ACE_MEM_Stream (void)
{
  // ACE_TRACE ("ACE_MEM_Stream::~ACE_MEM_Stream");
}

ASYS_INLINE int
ACE_MEM_Stream::close_reader (void)
{
  ACE_TRACE ("ACE_MEM_Stream::close_reader");
  if (this->get_handle () != ACE_INVALID_HANDLE)
    return ACE_OS::shutdown (this->get_handle (), ACE_SHUTDOWN_READ);
  else
    return 0;
}

// Shut down just the writing end of a ACE_SOCK.

ASYS_INLINE int
ACE_MEM_Stream::close_writer (void)
{
  ACE_TRACE ("ACE_MEM_Stream::close_writer");
  if (this->get_handle () != ACE_INVALID_HANDLE)
    return ACE_OS::shutdown (this->get_handle (), ACE_SHUTDOWN_WRITE);
  else
    return 0;
}

ASYS_INLINE ssize_t
ACE_MEM_Stream::send_n (const void *buf, int n)
{
  return this->send (buf, n);
}


ASYS_INLINE ssize_t
ACE_MEM_Stream::recv_n (void *buf, int n)
{
  return this->recv (buf, n);
}

ASYS_INLINE ssize_t
ACE_MEM_Stream::send_n (const void *buf, int n, int flags)
{
  return this->send (buf, n, flags);
}

ASYS_INLINE ssize_t
ACE_MEM_Stream::recv_n (void *buf, int n, int flags)
{
  return this->recv (buf, n, flags);
}

#if 0
ASYS_INLINE ssize_t
ACE_MEM_Stream::recv_n (void *buf,
                         size_t len,
                         int flags,
                         const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_MEM_Stream::recv_n");
  return ACE::recv_n (this->get_handle (),
                      buf,
                      len,
                      flags,
                      timeout);
}

ASYS_INLINE ssize_t
ACE_MEM_Stream::recv_n (void *buf,
                         size_t len,
                         const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_MEM_Stream::recv_n");
  return ACE::recv_n (this->get_handle (),
                      buf,
                      len,
                      timeout);
}

ASYS_INLINE ssize_t
ACE_MEM_Stream::recvv_n (iovec iov[],
                          size_t n,
                          const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_MEM_Stream::recvv_n");
  return ACE::recvv_n (this->get_handle (),
                       iov,
                       n,
                       timeout);
}

ASYS_INLINE ssize_t
ACE_MEM_Stream::send_n (const void *buf,
                         size_t len,
                         int flags,
                         const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_MEM_Stream::send_n");
  return ACE::send_n (this->get_handle (),
                      buf,
                      len,
                      flags,
                      timeout);
}

ASYS_INLINE ssize_t
ACE_MEM_Stream::send_n (const void *buf,
                         size_t len,
                         const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_MEM_Stream::send_n");
  return ACE::send_n (this->get_handle (),
                      buf,
                      len,
                      timeout);
}

ASYS_INLINE ssize_t
ACE_MEM_Stream::sendv_n (iovec iov[],
                          size_t n,
                          const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_MEM_Stream::sendv_n");
  return ACE::sendv_n (this->get_handle (),
                       iov,
                       n,
                       timeout);
}

ASYS_INLINE ssize_t
ACE_MEM_Stream::send_urg (const void *ptr,
                           size_t len,
                           const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_MEM_Stream::send_urg");
  return ACE::send (this->get_handle (),
                    ptr,
                    len,
                    MSG_OOB,
                    timeout);
}

ASYS_INLINE ssize_t
ACE_MEM_Stream::recv_urg (void *ptr,
                           size_t len,
                           const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_MEM_Stream::recv_urg");
  return ACE::recv (this->get_handle (),
                    ptr,
                    len,
                    MSG_OOB,
                    timeout);
}
#endif /* 0 */
