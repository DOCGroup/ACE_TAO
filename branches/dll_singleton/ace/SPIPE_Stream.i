/* -*- C++ -*- */
// $Id$

// SPIPE_Stream.i

// Create an ACE_SPIPE_Stream.

ASYS_INLINE int
ACE_SPIPE_Stream::get_remote_addr (ACE_SPIPE_Addr &remote_sap) const
{
  ACE_TRACE ("ACE_SPIPE_Stream::get_remote_addr");
  remote_sap = this->remote_addr_;
  return 0;
}

// Send exactly N bytes from BUF to this socket.  Keeping trying until
// this many bytes are sent.

ASYS_INLINE ssize_t
ACE_SPIPE_Stream::send_n (const void *buf, size_t n) const
{
  ACE_TRACE ("ACE_SPIPE_Stream::send_n");
  return ACE::write_n (this->get_handle (), buf, n);
}

// Receive exactly N bytes from this socket into BUF.  Keep trying
// until this many bytes are received.

ASYS_INLINE ssize_t
ACE_SPIPE_Stream::recv_n (void *buf, size_t n) const
{
  ACE_TRACE ("ACE_SPIPE_Stream::recv_n");
  return ACE::read_n (this->get_handle (), buf, n);
}

ASYS_INLINE ssize_t
ACE_SPIPE_Stream::send (const void *buf, size_t n) const
{
  ACE_TRACE ("ACE_SPIPE_Stream::send");
  return ACE_OS::write (this->get_handle (), (const char *) buf, n);
}

ASYS_INLINE ssize_t
ACE_SPIPE_Stream::recv (void *buf, size_t n) const
{
  ACE_TRACE ("ACE_SPIPE_Stream::recv");
  return ACE_OS::read (this->get_handle (), (char *) buf, n);
}

ASYS_INLINE ssize_t
ACE_SPIPE_Stream::send (const ACE_Str_Buf *cntl, const ACE_Str_Buf *data, int flags) const
{
  ACE_TRACE ("ACE_SPIPE_Stream::send");
  return ACE_OS::putmsg (this->get_handle (), (strbuf *) cntl, (strbuf *) data, flags);
}

ASYS_INLINE ssize_t
ACE_SPIPE_Stream::recv (ACE_Str_Buf *cntl, ACE_Str_Buf *data, int *flags) const
{
  ACE_TRACE ("ACE_SPIPE_Stream::recv");
  return ACE_OS::getmsg (this->get_handle (), (strbuf *) cntl, (strbuf *) data, flags);
}

ASYS_INLINE ssize_t
ACE_SPIPE_Stream::send (const ACE_Str_Buf *cntl, const ACE_Str_Buf *data, int band, int flags) const
{
  ACE_TRACE ("ACE_SPIPE_Stream::send");
  return ACE_OS::putpmsg (this->get_handle (), (strbuf *) cntl, (strbuf *) data, band, flags);
}

ASYS_INLINE ssize_t
ACE_SPIPE_Stream::recv (ACE_Str_Buf *cntl, ACE_Str_Buf *data, int *band, int *flags) const
{
  ACE_TRACE ("ACE_SPIPE_Stream::recv");
  return ACE_OS::getpmsg (this->get_handle (), (strbuf *) cntl, (strbuf *) data, band, flags);
}

ASYS_INLINE ssize_t
ACE_SPIPE_Stream::send (const iovec iov[], size_t n) const
{
  ACE_TRACE ("ACE_SPIPE_Stream::send");
  return ACE_OS::writev (this->get_handle (), iov, n);
}

ASYS_INLINE ssize_t
ACE_SPIPE_Stream::recv (iovec iov[], size_t n) const
{
  ACE_TRACE ("ACE_SPIPE_Stream::recv");
  return ACE_OS::readv (this->get_handle (), iov, n);
}

// This routine sends an open file descriptor to this socket.

ASYS_INLINE int
ACE_SPIPE_Stream::send_handle (ACE_HANDLE handle) const
{
  ACE_TRACE ("ACE_SPIPE_Stream::send_handle");
#if defined (ACE_HAS_STREAM_PIPES)
  return ACE_OS::ioctl (this->get_handle (), I_SENDFD, (void *) handle);
#else
  handle = handle;
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_STREAM_PIPES */
}

// This file receives an open file descriptor from this socket.

ASYS_INLINE int
ACE_SPIPE_Stream::recv_handle (ACE_HANDLE &handle) const
{
  ACE_TRACE ("ACE_SPIPE_Stream::recv_handle");
#if defined (ACE_HAS_STREAM_PIPES)
  strrecvfd recvfd;

  if (ACE_OS::ioctl (this->get_handle (), I_RECVFD, (void *) &recvfd) == -1)
    return -1;
  else
    {
      handle = recvfd.fd;
      return 0;
    }
#else
  handle = handle;
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_STREAM_PIPES */
}

// This file receives an open file descriptor from this socket and
// also passes back the information about the address...

ASYS_INLINE int
ACE_SPIPE_Stream::recv_handle (strrecvfd &recvfd) const
{
  ACE_TRACE ("ACE_SPIPE_Stream::recv_handle");
#if defined (ACE_HAS_STREAM_PIPES)
  return ACE_OS::ioctl (this->get_handle (), I_RECVFD, (void *) &recvfd);
#else
  ACE_UNUSED_ARG (recvfd);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_STREAM_PIPES */
}

ASYS_INLINE ssize_t
ACE_SPIPE_Stream::send (const void *buf, size_t n,
                        ACE_OVERLAPPED *overlapped) const
{
  ACE_TRACE ("ACE_SPIPE_Stream::send");
  return ACE_OS::write (this->get_handle (),
                        (const char *) buf, n,
                        overlapped);
}

ASYS_INLINE ssize_t
ACE_SPIPE_Stream::recv (void *buf, size_t n,
                        ACE_OVERLAPPED *overlapped) const
{
  ACE_TRACE ("ACE_SPIPE_Stream::recv");
  return ACE_OS::read (this->get_handle (),
                       (char *) buf, n,
                       overlapped);
}

ASYS_INLINE ssize_t
ACE_SPIPE_Stream::sendv_n (const iovec iov[],
                           size_t n) const
{
  ACE_TRACE ("ACE_SPIPE_Stream::sendv_n");
  return ACE::writev_n (this->get_handle (),
                        iov,
                        n);
}

// Recv an n byte message from the Stream.

ASYS_INLINE ssize_t
ACE_SPIPE_Stream::recvv_n (iovec iov[],
                           size_t n) const
{
  ACE_TRACE ("ACE_SPIPE_Stream::recvv_n");
  // @@ Carlos, can you please update this to call the
  // new ACE::recvv_n() method that you write?
  return ACE_OS::readv (this->get_handle (),
                        iov,
                        n);
}

// Send an <iovec> of size <n> to the Stream.

ASYS_INLINE ssize_t
ACE_SPIPE_Stream::sendv (const iovec iov[],
                         size_t n) const
{
  ACE_TRACE ("ACE_SPIPE_Stream::sendv");
  return ACE_OS::writev (this->get_handle (),
                         iov,
                         n);
}

