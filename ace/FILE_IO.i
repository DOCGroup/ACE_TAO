/* -*- C++ -*- */
// $Id$

// FILE_IO.i

ASYS_INLINE ssize_t
ACE_FILE_IO::sendv_n (const iovec iov[], size_t n) const
{
  ACE_TRACE ("ACE_FILE_IO::sendv_n");
  return ACE::writev_n (this->get_handle (),
                        iov,
                        n);
}

// Recv an n byte message from the file.

ASYS_INLINE ssize_t
ACE_FILE_IO::recvv_n (iovec iov[], size_t n) const
{
  ACE_TRACE ("ACE_FILE_IO::recvv_n");
  // @@ Carlos, can you please update this to call the
  // new ACE::recvv_n() method that you write?
  return ACE_OS::readv (this->get_handle (),
                        iov,
                        n);
}

// Send an <iovec> of size <n> to the file.

ASYS_INLINE ssize_t
ACE_FILE_IO::sendv (const iovec iov[], size_t n) const
{
  ACE_TRACE ("ACE_FILE_IO::sendv");
  return ACE_OS::writev (this->get_handle (), iov, n);
}

// Send exactly N bytes from BUF to this file.  Keeping trying until
// this many bytes are sent.

ASYS_INLINE ssize_t
ACE_FILE_IO::send_n (const void *buf, size_t n) const
{
  ACE_TRACE ("ACE_FILE_IO::send_n");
  return ACE::write_n (this->get_handle (), buf, n);
}

// Receive exactly N bytes from this file into BUF.  Keep trying until
// this many bytes are received.

ASYS_INLINE ssize_t
ACE_FILE_IO::recv_n (void *buf, size_t n) const
{
  ACE_TRACE ("ACE_FILE_IO::recv_n");
  return ACE::read_n (this->get_handle (), buf, n);
}

ASYS_INLINE ssize_t
ACE_FILE_IO::send (const void *buf, size_t n) const
{
  ACE_TRACE ("ACE_FILE_IO::send");
  return ACE_OS::write (this->get_handle (), (const char *) buf, n);
}

ASYS_INLINE ssize_t
ACE_FILE_IO::recv (void *buf, size_t n) const
{
  ACE_TRACE ("ACE_FILE_IO::recv");
  return ACE_OS::read (this->get_handle (), (char *) buf, n);
}

ASYS_INLINE ssize_t
ACE_FILE_IO::send (const iovec iov[], size_t n) const
{
  ACE_TRACE ("ACE_FILE_IO::send");
  return ACE_OS::writev (this->get_handle (), iov, n);
}

ASYS_INLINE ssize_t
ACE_FILE_IO::recv (iovec iov[], size_t n) const
{
  ACE_TRACE ("ACE_FILE_IO::recv");
  return ACE_OS::readv (this->get_handle (), iov, n);
}

#if defined (ACE_HAS_STREAM_PIPES)
ASYS_INLINE ssize_t
ACE_FILE_IO::recv (ACE_Str_Buf *cntl, ACE_Str_Buf *data, int *band, int *flags) const
{
  ACE_TRACE ("ACE_FILE_IO::recv");
  return ACE_OS::getpmsg (this->get_handle (), (strbuf *) cntl, (strbuf *) data, band, flags);
}

ASYS_INLINE ssize_t
ACE_FILE_IO::send (const ACE_Str_Buf *cntl, const ACE_Str_Buf *data, int band, int flags) const
{
  ACE_TRACE ("ACE_FILE_IO::send");
  return ACE_OS::putpmsg (this->get_handle (), (strbuf *) cntl, (strbuf *) data, band, flags);
}

ASYS_INLINE ssize_t
ACE_FILE_IO::recv (ACE_Str_Buf *cntl, ACE_Str_Buf *data, int *flags) const
{
  ACE_TRACE ("ACE_FILE_IO::recv");
  return ACE_OS::getmsg (this->get_handle (), (strbuf *) cntl, (strbuf *) data, flags);
}

ASYS_INLINE ssize_t
ACE_FILE_IO::send (const ACE_Str_Buf *cntl, const ACE_Str_Buf *data, int flags) const
{
  ACE_TRACE ("ACE_FILE_IO::send");
  return ACE_OS::putmsg (this->get_handle (), (strbuf *) cntl, (strbuf *) data, flags);
}

ASYS_INLINE ssize_t
ACE_FILE_IO::send (const void *buf, size_t n,
		       ACE_OVERLAPPED *overlapped) const
{
  ACE_TRACE ("ACE_FILE_IO::send");
  return ACE_OS::write (this->get_handle (), 
			(const char *) buf, n,
			overlapped);
}

ASYS_INLINE ssize_t
ACE_FILE_IO::recv (void *buf, size_t n,
		       ACE_OVERLAPPED *overlapped) const
{
  ACE_TRACE ("ACE_FILE_IO::recv");
  return ACE_OS::read (this->get_handle (), (char *) buf, n,
		       overlapped);
}

#endif /* ACE_HAS_STREAM_PIPES */
