/* -*- C++ -*- */
// $Id$

// DEV_IO.i

// Send exactly N bytes from BUF to this device.  Keeping trying until
// this many bytes are sent.

ASYS_INLINE ssize_t
ACE_DEV_IO::send_n (const void *buf, size_t n) const
{
  ACE_TRACE ("ACE_DEV_IO::send_n");
  return ACE::write_n (this->get_handle (), buf, n);
}

// Receive exactly N bytes from this file into BUF.  Keep trying until
// this many bytes are received.

ASYS_INLINE ssize_t
ACE_DEV_IO::recv_n (void *buf, size_t n) const
{
  ACE_TRACE ("ACE_DEV_IO::recv_n");
  return ACE::read_n (this->get_handle (), buf, n);
}

ASYS_INLINE ssize_t
ACE_DEV_IO::send (const void *buf, size_t n) const
{
  ACE_TRACE ("ACE_DEV_IO::send");
  return ACE_OS::write (this->get_handle (), (const char *) buf, n);
}

ASYS_INLINE ssize_t
ACE_DEV_IO::recv (void *buf, size_t n) const
{
  ACE_TRACE ("ACE_DEV_IO::recv");
  return ACE_OS::read (this->get_handle (), (char *) buf, n);
}

ASYS_INLINE ssize_t
ACE_DEV_IO::send (const iovec iov[], size_t n) const
{
  ACE_TRACE ("ACE_DEV_IO::send");
  return ACE_OS::writev (this->get_handle (), iov, ACE_static_cast (int, n));
}

ASYS_INLINE ssize_t
ACE_DEV_IO::recv (iovec iov[], size_t n) const
{
  ACE_TRACE ("ACE_DEV_IO::recv");
  return ACE_OS::readv (this->get_handle (), iov, ACE_static_cast (int, n));
}

ASYS_INLINE ssize_t
ACE_DEV_IO::send (const void *buf, size_t n,
                  ACE_OVERLAPPED *overlapped) const
{
  ACE_TRACE ("ACE_DEV_IO::send");
  return ACE_OS::write (this->get_handle (),
                        (const char *) buf, n,
                        overlapped);
}

ASYS_INLINE ssize_t
ACE_DEV_IO::recv (void *buf, size_t n,
                  ACE_OVERLAPPED *overlapped) const
{
  ACE_TRACE ("ACE_DEV_IO::recv");
  return ACE_OS::read (this->get_handle (), (char *) buf, n,
                       overlapped);
}

#if defined (ACE_HAS_STREAM_PIPES)
ASYS_INLINE ssize_t
ACE_DEV_IO::recv (ACE_Str_Buf *cntl, ACE_Str_Buf *data, int *band, int *flags) const
{
  ACE_TRACE ("ACE_DEV_IO::recv");
  return ACE_OS::getpmsg (this->get_handle (), (strbuf *) cntl, (strbuf *) data, band, flags);
}

ASYS_INLINE ssize_t
ACE_DEV_IO::send (const ACE_Str_Buf *cntl, const ACE_Str_Buf *data, int band, int flags) const
{
  ACE_TRACE ("ACE_DEV_IO::send");
  return ACE_OS::putpmsg (this->get_handle (), (strbuf *) cntl, (strbuf *) data, band, flags);
}

ASYS_INLINE ssize_t
ACE_DEV_IO::recv (ACE_Str_Buf *cntl, ACE_Str_Buf *data, int *flags) const
{
  ACE_TRACE ("ACE_DEV_IO::recv");
  return ACE_OS::getmsg (this->get_handle (), (strbuf *) cntl, (strbuf *) data, flags);
}

ASYS_INLINE ssize_t
ACE_DEV_IO::send (const ACE_Str_Buf *cntl, const ACE_Str_Buf *data, int flags) const
{
  ACE_TRACE ("ACE_DEV_IO::send");
  return ACE_OS::putmsg (this->get_handle (), (strbuf *) cntl, (strbuf *) data, flags);
}
#endif /* ACE_HAS_STREAM_PIPES */
