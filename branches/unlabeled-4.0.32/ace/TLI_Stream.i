/* -*- C++ -*- */
// $Id$

// TLI_Stream.i

#include "ace/TLI_Stream.h"

inline ssize_t
ACE_TLI_Stream::send (const void *buf, size_t n) const
{
  ACE_TRACE ("ACE_TLI_Stream::send");
  return ACE_OS::write (this->get_handle (), (const char *) buf, n);
}

inline ssize_t
ACE_TLI_Stream::send (const void *buf, size_t n, int flags) const
{
  ACE_TRACE ("ACE_TLI_Stream::send");
  return ACE_OS::t_snd (this->get_handle (), (char *) buf, n, flags);
}

inline ssize_t
ACE_TLI_Stream::recv (void *buf, size_t n) const
{
  ACE_TRACE ("ACE_TLI_Stream::recv");
  return ACE_OS::read (this->get_handle (), (char *) buf, n);
}

inline ssize_t
ACE_TLI_Stream::recv (void *buf, size_t n, int *flags) const
{
  ACE_TRACE ("ACE_TLI_Stream::recv");
  int f = 0;

  if (flags == 0)
    flags = &f;
  return ACE_OS::t_rcv (this->get_handle (), (char *) buf, n, flags);
}

inline ssize_t
ACE_TLI_Stream::send_n (const void *buf, size_t n) const
{
  ACE_TRACE ("ACE_TLI_Stream::send_n");
  return ACE::send_n (this->get_handle (), buf, n);
}

inline ssize_t
ACE_TLI_Stream::send_n (const void *buf, size_t n, int flags) const
{
  ACE_TRACE ("ACE_TLI_Stream::send_n");
  int  b_sent;
  int  b_written;
  
  for (b_sent = 0; b_sent < n; b_sent += b_written)
    if ((b_written = ACE_OS::t_snd (this->get_handle (), 
			      (char *) buf + b_sent, 
			      n - b_sent, flags)) == -1)
      return -1;

  return b_sent;
}

inline ssize_t
ACE_TLI_Stream::recv_n (void *buf, size_t n) const
{
  ACE_TRACE ("ACE_TLI_Stream::recv_n");
  return ACE::recv_n (this->get_handle (), buf, n);
}

inline ssize_t
ACE_TLI_Stream::recv_n (void *buf, size_t n, int *flags) const
{
  ACE_TRACE ("ACE_TLI_Stream::recv_n");
  int  b_read = 0;
  int  b_recv = 0;
  int  f      = 0;

  if (flags == 0)
    flags = &f;

  for (b_read = 0; b_read < n; b_read += b_recv)
    if ((b_recv = ACE_OS::t_rcv (this->get_handle (), 
			   (char *) buf + b_read, 
			   n - b_read, flags)) == -1)
      return -1;
    else if (b_recv == 0)
      break;

  return b_read;      
}

inline void
ACE_TLI_Stream::set_rwflag (int value)
{
  ACE_TRACE ("ACE_TLI_Stream::set_rwflag");
  this->rwflag_ = value;
}

inline int
ACE_TLI_Stream::get_rwflag (void)
{
  ACE_TRACE ("ACE_TLI_Stream::get_rwflag");
  return this->rwflag_;
}

