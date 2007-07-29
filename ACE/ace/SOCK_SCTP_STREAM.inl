// -*- C++ -*-
//
// $Id$

#include "ace/SOCK_SCTP_STREAM.h"
#include "ace/OS_NS_sys_socket.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
ACE_SOCK_SCTP_STREAM::ACE_SOCK_SCTP_STREAM (void)
{
  // ACE_TRACE ("ACE_SOCK_SCTP_STREAM::ACE_SOCK_SCTP_STREAM");
}

ACE_INLINE
ACE_SOCK_SCTP_STREAM::ACE_SOCK_SCTP_STREAM (ACE_HANDLE h)
{
  // ACE_TRACE ("ACE_SOCK_SCTP_STREAM::ACE_SOCK_SCTP_STREAM");
  this->set_handle (h);
}

ACE_INLINE
ACE_SOCK_SCTP_STREAM::~ACE_SOCK_SCTP_STREAM (void)
{
  // ACE_TRACE ("ACE_SOCK_SCTP_STREAM::~ACE_SOCK_SCTP_STREAM");
}

ACE_INLINE ssize_t
ACE_SOCK_SCTP_STREAM::recv_n (void *buf,
                         size_t len,
                         int flags,
                         const ACE_Time_Value *timeout,
                         size_t *bytes_transferred) const
{
  ACE_TRACE ("ACE_SOCK_SCTP_STREAM::recv_n");
  return ACE::recv_n (this->get_handle (),
                      buf,
                      len,
                      flags,
                      timeout,
                      bytes_transferred);
}

ACE_INLINE ssize_t
ACE_SOCK_SCTP_STREAM::recv_n (void *buf,
                         size_t len,
                         const ACE_Time_Value *timeout,
                         size_t *bytes_transferred) const
{
  ACE_TRACE ("ACE_SOCK_SCTP_STREAM::recv_n");
  return ACE::recv_n (this->get_handle (),
                      buf,
                      len,
                      timeout,
                      bytes_transferred);
}

ACE_INLINE ssize_t
ACE_SOCK_SCTP_STREAM::recvv_n (iovec iov[],
                                       int n,
                                       const ACE_Time_Value *timeout,
                                       size_t *bytes_transferred) const
{
  ACE_TRACE ("ACE_SOCK_SCTP_STREAM::recvv_n");
  return ACE::recvv_n (this->get_handle (),
                       iov,
                       n,
                       timeout,
                       bytes_transferred);
}

ACE_INLINE ssize_t
ACE_SOCK_SCTP_STREAM::send_n (const void *buf,
                         size_t len,
                         int flags,
                         const ACE_Time_Value *timeout,
                         size_t *bytes_transferred) const
{
  ACE_TRACE ("ACE_SOCK_SCTP_STREAM::send_n");
  return ACE::send_n (this->get_handle (),
                      buf,
                      len,
                      flags,
                      timeout,
                      bytes_transferred);
}

ACE_INLINE ssize_t
ACE_SOCK_SCTP_STREAM::send_n (const void *buf,
                         size_t len,
                         const ACE_Time_Value *timeout,
                         size_t *bytes_transferred) const
{
  ACE_TRACE ("ACE_SOCK_SCTP_STREAM::send_n");
  return ACE::send_n (this->get_handle (),
                      buf,
                      len,
                      timeout,
                      bytes_transferred);
}

ACE_INLINE ssize_t
ACE_SOCK_SCTP_STREAM::sendv_n (const iovec iov[],
                                       int n,
                                       const ACE_Time_Value *timeout,
                                       size_t *bytes_transferred) const
{
  ACE_TRACE ("ACE_SOCK_SCTP_STREAM::sendv_n");
  return ACE::sendv_n (this->get_handle (),
                       iov,
                       n,
                       timeout,
                       bytes_transferred);
}

ACE_INLINE ssize_t
ACE_SOCK_SCTP_STREAM::send_n (const ACE_Message_Block *message_block,
                         const ACE_Time_Value *timeout,
                         size_t *bytes_transferred) const
{
  ACE_TRACE ("ACE_SOCK_SCTP_STREAM::send_n");
  return ACE::send_n (this->get_handle (),
                      message_block,
                      timeout,
                      bytes_transferred);
}

ACE_INLINE ssize_t
ACE_SOCK_SCTP_STREAM::send_urg (const void *ptr,
                           size_t len,
                           const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_SOCK_SCTP_STREAM::send_urg");
  return ACE::send (this->get_handle (),
                    ptr,
                    len,
                    MSG_OOB,
                    timeout);
}

ACE_INLINE ssize_t
ACE_SOCK_SCTP_STREAM::recv_urg (void *ptr,
                           size_t len,
                           const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_SOCK_SCTP_STREAM::recv_urg");
  return ACE::recv (this->get_handle (),
                    ptr,
                    len,
                    MSG_OOB,
                    timeout);
}

ACE_END_VERSIONED_NAMESPACE_DECL
