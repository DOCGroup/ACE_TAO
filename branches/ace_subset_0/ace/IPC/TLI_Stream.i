/* -*- C++ -*- */
// $Id$

// TLI_Stream.i

#include "ace/IPC/TLI_Stream.h"

ACE_INLINE
ssize_t
ACE_TLI_Stream::send (const void *buf,
                      size_t n,
                      const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_TLI_Stream::send");
  return ACE::send (this->get_handle (),
                    buf,
                    n,
                    timeout);
}

ACE_INLINE
ssize_t
ACE_TLI_Stream::send (const void *buf,
                      size_t n,
                      int flags,
                      const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_TLI_Stream::send");
  return ACE::t_snd (this->get_handle (),
                     buf,
                     n,
                     flags,
                     timeout);
}

ACE_INLINE
ssize_t
ACE_TLI_Stream::recv (void *buf,
                      size_t n,
                      const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_TLI_Stream::recv");
  return ACE::recv (this->get_handle (),
                    buf,
                    n,
                    timeout);
}

ACE_INLINE
ssize_t
ACE_TLI_Stream::recv (void *buf,
                      size_t n,
                      int *flags,
                      const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_TLI_Stream::recv");
  int f = 0;

  if (flags == 0)
    flags = &f;

  return ACE::t_rcv (this->get_handle (),
                     buf,
                     n,
                     flags,
                     timeout);
}

ACE_INLINE
ssize_t
ACE_TLI_Stream::send_n (const void *buf,
                        size_t n,
                        const ACE_Time_Value *timeout,
                        size_t *bytes_transferred) const
{
  ACE_TRACE ("ACE_TLI_Stream::send_n");
  return ACE::send_n (this->get_handle (),
                      buf,
                      n,
                      timeout,
                      bytes_transferred);
}

ACE_INLINE
ssize_t
ACE_TLI_Stream::send_n (const void *buf,
                        size_t n,
                        int flags,
                        const ACE_Time_Value *timeout,
                        size_t *bytes_transferred) const
{
  ACE_TRACE ("ACE_TLI_Stream::send_n");

  return ACE::t_snd_n (this->get_handle (),
                       buf,
                       n,
                       flags,
                       timeout,
                       bytes_transferred);
}

ACE_INLINE
ssize_t
ACE_TLI_Stream::recv_n (void *buf,
                        size_t n,
                        const ACE_Time_Value *timeout,
                        size_t *bytes_transferred) const
{
  ACE_TRACE ("ACE_TLI_Stream::recv_n");
  return ACE::recv_n (this->get_handle (),
                      buf,
                      n,
                      timeout,
                      bytes_transferred);
}

ACE_INLINE
ssize_t
ACE_TLI_Stream::recv_n (void *buf,
                        size_t n,
                        int *flags,
                        const ACE_Time_Value *timeout,
                        size_t *bytes_transferred) const
{
  ACE_TRACE ("ACE_TLI_Stream::recv_n");

  return ACE::t_rcv_n (this->get_handle (),
                       buf,
                       n,
                       flags,
                       timeout,
                       bytes_transferred);
}

ACE_INLINE
void
ACE_TLI_Stream::set_rwflag (int value)
{
  ACE_TRACE ("ACE_TLI_Stream::set_rwflag");
  this->rwflag_ = value;
}

ACE_INLINE
int
ACE_TLI_Stream::get_rwflag (void)
{
  ACE_TRACE ("ACE_TLI_Stream::get_rwflag");
  return this->rwflag_;
}
