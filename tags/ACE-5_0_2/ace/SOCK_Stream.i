/* -*- C++ -*- */
// $Id$

// SOCK_Stream.i

#include "ace/SOCK_Stream.h"

ASYS_INLINE
ACE_SOCK_Stream::ACE_SOCK_Stream (void)
{
  // ACE_TRACE ("ACE_SOCK_Stream::ACE_SOCK_Stream");
}

ASYS_INLINE
ACE_SOCK_Stream::ACE_SOCK_Stream (ACE_HANDLE h)
{
  // ACE_TRACE ("ACE_SOCK_Stream::ACE_SOCK_Stream");
  this->set_handle (h);
}

ASYS_INLINE
ACE_SOCK_Stream::~ACE_SOCK_Stream (void)
{
  // ACE_TRACE ("ACE_SOCK_Stream::~ACE_SOCK_Stream");
}

ASYS_INLINE int
ACE_SOCK_Stream::close_reader (void)
{
  ACE_TRACE ("ACE_SOCK_Stream::close_reader");
  if (this->get_handle () != ACE_INVALID_HANDLE)
    return ACE_OS::shutdown (this->get_handle (), 0);
  else
    return 0;
}

// Shut down just the writing end of a ACE_SOCK.

ASYS_INLINE int
ACE_SOCK_Stream::close_writer (void)
{
  ACE_TRACE ("ACE_SOCK_Stream::close_writer");
  if (this->get_handle () != ACE_INVALID_HANDLE)
    return ACE_OS::shutdown (this->get_handle (), 1);
  else
    return 0;
}

ASYS_INLINE ssize_t
ACE_SOCK_Stream::send_n (const void *buf, size_t len, int flags,
			 const ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_SOCK_Stream::send_n");
  return ACE::send_n (this->get_handle (), buf, len, flags, timeout);
}

ASYS_INLINE ssize_t
ACE_SOCK_Stream::recv_n (void *buf, size_t len, int flags,
			 const ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_SOCK_Stream::recv_n");
  return ACE::recv_n (this->get_handle (), buf, len, flags, timeout);
}

// Receive exactly BUF_SIZE bytes from file descriptor this->handle
// into <buf>.  Keep trying until this many bytes are received.

ASYS_INLINE ssize_t
ACE_SOCK_Stream::recv_n (void *buf, int buf_size, int flags) const
{
  ACE_TRACE ("ACE_SOCK_Stream::recv_n");
  return ACE::recv_n (this->get_handle (), buf, buf_size, flags);
}

// Send exactly N bytes from <buf> to <handle>.  Keeping trying
// until this many bytes are sent.

ASYS_INLINE ssize_t
ACE_SOCK_Stream::send_n (const void *buf, int buf_size, int flags) const
{
  ACE_TRACE ("ACE_SOCK_Stream::send_n");
  return ACE::send_n (this->get_handle (), buf, buf_size, flags);
}

// Receive exactly BUF_SIZE bytes from file descriptor
// into BUF.  Keep trying until this many bytes are received.

ASYS_INLINE ssize_t
ACE_SOCK_Stream::recv_n (void *buf, int buf_size) const
{
  ACE_TRACE ("ACE_SOCK_Stream::recv_n");
  return ACE::recv_n (this->get_handle (), buf, buf_size);
}

// Send exactly N bytes from BUF to THIS->SOK_FD.  Keeping trying
// until this many bytes are sent.

ASYS_INLINE ssize_t
ACE_SOCK_Stream::send_n (const void *buf, int buf_size) const
{
  ACE_TRACE ("ACE_SOCK_Stream::send_n");
  return ACE::send_n (this->get_handle (), buf, buf_size);
}

ASYS_INLINE ssize_t
ACE_SOCK_Stream::sendv_n (const iovec iov[], size_t n) const
{
  ACE_TRACE ("ACE_SOCK_Stream::sendv_n");
  return ACE::sendv_n (this->get_handle (),
                       iov,
                       n);
}

// Recv an n byte message from the connected socket.

ASYS_INLINE ssize_t
ACE_SOCK_Stream::recvv_n (iovec iov[], size_t n) const
{
  ACE_TRACE ("ACE_SOCK_Stream::recvv_n");
  // @@ Carlos, can you please update this to call the
  // new ACE::recvv_n() method that you write?
  return ACE_OS::recvv (this->get_handle (), iov, n);
}

ASYS_INLINE ssize_t
ACE_SOCK_Stream::send_urg (void *ptr, int len)
{
  ACE_TRACE ("ACE_SOCK_Stream::send_urg");
  return ACE_OS::send (this->get_handle (), (char *) ptr, len, MSG_OOB);
}

ASYS_INLINE ssize_t
ACE_SOCK_Stream::recv_urg (void *ptr, int len)
{
  ACE_TRACE ("ACE_SOCK_Stream::recv_urg");
  return ACE_OS::recv (this->get_handle (), (char *) ptr, len, MSG_OOB);
}
