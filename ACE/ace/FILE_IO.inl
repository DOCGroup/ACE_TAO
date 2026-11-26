#include "ace/ACE.h"
#include "ace/OS_NS_sys_uio.h"
#include "ace/OS_NS_unistd.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE ssize_t
ACE_FILE_IO::sendv_n (iovec const iov[], int n) const
{
  ACE_TRACE ("ACE_FILE_IO::sendv_n");
  return ACE::writev_n (this->get_handle (),
                        iov,
                        n);
}

ACE_INLINE ssize_t
ACE_FILE_IO::send_n (ACE_Message_Block const *message_block,
                     ACE_Time_Value const *timeout,
                     size_t *bytes_transferred)
{
  ACE_TRACE ("ACE_FILE_IO::send_n");
  ACE_UNUSED_ARG (timeout);
  return ACE::write_n (this->get_handle (),
                      message_block,
                      bytes_transferred);
}

/// Recv an n byte message from the file.

ACE_INLINE ssize_t
ACE_FILE_IO::recvv_n (iovec iov[], int n) const
{
  ACE_TRACE ("ACE_FILE_IO::recvv_n");
  // @@ Carlos, can you please update this to call the
  // new ACE::recvv_n() method that you write?
  return ACE_OS::readv (this->get_handle (),
                        iov,
                        n);
}

/// Send an @c iovec of size @c n to the file.

ACE_INLINE ssize_t
ACE_FILE_IO::sendv (iovec const iov[], int n) const
{
  ACE_TRACE ("ACE_FILE_IO::sendv");
  return ACE_OS::writev (this->get_handle (), iov, n);
}

/// Send exactly N bytes from BUF to this file.  Keeping trying until
/// this many bytes are sent.

ACE_INLINE ssize_t
ACE_FILE_IO::send_n (void const *buf, size_t n) const
{
  ACE_TRACE ("ACE_FILE_IO::send_n");
  return ACE::write_n (this->get_handle (), buf, n);
}

/// Receive exactly N bytes from this file into BUF.  Keep trying until
/// this many bytes are received.

ACE_INLINE ssize_t
ACE_FILE_IO::recv_n (void *buf, size_t n) const
{
  ACE_TRACE ("ACE_FILE_IO::recv_n");
  return ACE::read_n (this->get_handle (), buf, n);
}

ACE_INLINE ssize_t
ACE_FILE_IO::send (void const *buf, size_t n) const
{
  ACE_TRACE ("ACE_FILE_IO::send");
  return ACE_OS::write (this->get_handle (), buf, n);
}

ACE_INLINE ssize_t
ACE_FILE_IO::recv (void *buf, size_t n) const
{
  ACE_TRACE ("ACE_FILE_IO::recv");
  return ACE_OS::read (this->get_handle (), buf, n);
}

ACE_INLINE ssize_t
ACE_FILE_IO::send (iovec const iov[], int n) const
{
  ACE_TRACE ("ACE_FILE_IO::send");
  return ACE_OS::writev (this->get_handle (), iov, n);
}

ACE_INLINE ssize_t
ACE_FILE_IO::recv (iovec iov[], int n) const
{
  ACE_TRACE ("ACE_FILE_IO::recv");
  return ACE_OS::readv (this->get_handle (), iov, n);
}

#if defined (ACE_HAS_STREAM_PIPES)
ACE_INLINE ssize_t
ACE_FILE_IO::recv (ACE_Str_Buf *cntl, ACE_Str_Buf *data, int *band, int *flags) const
{
  ACE_TRACE ("ACE_FILE_IO::recv");
  return ACE_OS::getpmsg (this->get_handle (), cntl, data, band, flags);
}

ACE_INLINE ssize_t
ACE_FILE_IO::send (ACE_Str_Buf const *cntl, ACE_Str_Buf const *data, int band, int flags) const
{
  ACE_TRACE ("ACE_FILE_IO::send");
  return ACE_OS::putpmsg (this->get_handle (), cntl, data, band, flags);
}

ACE_INLINE ssize_t
ACE_FILE_IO::recv (ACE_Str_Buf *cntl, ACE_Str_Buf *data, int *flags) const
{
  ACE_TRACE ("ACE_FILE_IO::recv");
  return ACE_OS::getmsg (this->get_handle (), cntl, data, flags);
}

ACE_INLINE ssize_t
ACE_FILE_IO::send (ACE_Str_Buf const *cntl, ACE_Str_Buf const *data, int flags) const
{
  ACE_TRACE ("ACE_FILE_IO::send");
  return ACE_OS::putmsg (this->get_handle (), cntl, data, flags);
}

ACE_INLINE ssize_t
ACE_FILE_IO::send (void const *buf, size_t n,
                   ACE_OVERLAPPED *overlapped) const
{
  ACE_TRACE ("ACE_FILE_IO::send");
  return ACE_OS::write (this->get_handle (),
                        buf, n,
                        overlapped);
}

ACE_INLINE ssize_t
ACE_FILE_IO::recv (void *buf, size_t n,
                   ACE_OVERLAPPED *overlapped) const
{
  ACE_TRACE ("ACE_FILE_IO::recv");
  return ACE_OS::read (this->get_handle (), buf, n,
                       overlapped);
}

#endif /* ACE_HAS_STREAM_PIPES */

ACE_END_VERSIONED_NAMESPACE_DECL
