/* -*- C++ -*- */
// $Id$

// SOCK_Stream.i

#include "ace/SOCK_Stream.h"

// Shut down just the reading end of a ACE_SOCK. 

inline int
ACE_SOCK_Stream::close_reader (void)
{
  ACE_TRACE ("ACE_SOCK_Stream::close_reader");
  return ACE_OS::shutdown (this->get_handle (), 0);
}

// Shut down just the writing end of a ACE_SOCK. 

inline int
ACE_SOCK_Stream::close_writer (void)
{
  ACE_TRACE ("ACE_SOCK_Stream::close_writer");
  return ACE_OS::shutdown (this->get_handle (), 1);
}

inline ssize_t 
ACE_SOCK_Stream::send_n (const void *buf, size_t len, int flags,
			 const ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_SOCK_Stream::send_n");
  return ACE::send_n (this->get_handle (), buf, len, flags, timeout);
}

inline ssize_t 
ACE_SOCK_Stream::recv_n (void *buf, size_t len, int flags, 
			 const ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_SOCK_Stream::recv_n");
  return ACE::recv_n (this->get_handle (), buf, len, flags, timeout);
}

// Receive exactly BUF_SIZE bytes from file descriptor this->handle
// into <buf>.  Keep trying until this many bytes are received.

inline ssize_t
ACE_SOCK_Stream::recv_n (void *buf, int buf_size, int flags) const
{
  ACE_TRACE ("ACE_SOCK_Stream::recv_n");
  return ACE::recv_n (this->get_handle (), buf, buf_size, flags);
}

// Send exactly N bytes from <buf> to <handle>.  Keeping trying 
// until this many bytes are sent. 

inline ssize_t
ACE_SOCK_Stream::send_n (const void *buf, int buf_size, int flags) const
{
  ACE_TRACE ("ACE_SOCK_Stream::send_n");
  return ACE::send_n (this->get_handle (), buf, buf_size, flags);
}

// Receive exactly BUF_SIZE bytes from file descriptor 
// into BUF.  Keep trying until this many bytes are received. 

inline ssize_t
ACE_SOCK_Stream::recv_n (void *buf, int buf_size) const
{
  ACE_TRACE ("ACE_SOCK_Stream::recv_n");
  return ACE::recv_n (this->get_handle (), buf, buf_size);
}

// Send exactly N bytes from BUF to THIS->SOK_FD.  Keeping trying
// until this many bytes are sent.

inline ssize_t
ACE_SOCK_Stream::send_n (const void *buf, int buf_size) const
{
  ACE_TRACE ("ACE_SOCK_Stream::send_n");
  return ACE::send_n (this->get_handle (), buf, buf_size);
}

inline ssize_t  
ACE_SOCK_Stream::send_urg (void *ptr, int len)
{
  ACE_TRACE ("ACE_SOCK_Stream::send_urg");
  return ACE_OS::send (this->get_handle (), (char *) ptr, len, MSG_OOB);
}

inline ssize_t  
ACE_SOCK_Stream::recv_urg (void *ptr, int len)
{
  ACE_TRACE ("ACE_SOCK_Stream::recv_urg");
  return ACE_OS::recv (this->get_handle (), (char *) ptr, len, MSG_OOB);
}

