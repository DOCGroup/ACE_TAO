/* -*- C++ -*- */
// $Id$

// MEM_IO.i

// Send an n byte message to the connected socket.
ASYS_INLINE
ACE_MEM_IO::ACE_MEM_IO (void)
  : recv_buffer_ (0),
    buf_size_ (0),
    cur_offset_ (0)
{
  // ACE_TRACE ("ACE_MEM_IO::ACE_MEM_IO");
}

ASYS_INLINE
ACE_MEM_IO::~ACE_MEM_IO (void)
{
  // ACE_TRACE ("ACE_MEM_IO::~ACE_MEM_IO");
}

ASYS_INLINE ssize_t
ACE_MEM_IO::send (const void *buf, size_t n, int flags)
{
  ACE_TRACE ("ACE_MEM_IO::send");
  void *sbuf = this->acquire_buffer (n);
  if (sbuf == 0)
    return -1;                  // Memory buffer not initialized.
  ACE_OS::memcpy (sbuf, buf, n);
  off_t offset = this->set_buf_len (sbuf, n); // <set_buf_len> also calculate
                                              // the offset.

  // Send the offset value over the socket.
  return ACE_OS::send (this->get_handle (),
                       (const char *) &offset,
                       sizeof (offset),
                       flags);
}

// Recv an n byte message from the connected socket.

ASYS_INLINE ssize_t
ACE_MEM_IO::recv (void *buf, size_t n, int flags)
{
  ACE_TRACE ("ACE_MEM_IO::recv");

  size_t count = 0;

//    while (n > 0)
//      {
      size_t buf_len = this->buf_size_ - this->cur_offset_;
      if (buf_len == 0)
        {
          if (this->fetch_recv_buf (flags) == -1)
            return -1;
          buf_len = this->buf_size_;
        }

      size_t length = (n > buf_len ? buf_len : n);

      ACE_OS::memcpy ((char *) buf + count,
                      (char *) this->recv_buffer_ + this->cur_offset_,
                      length);
      this->cur_offset_ += length;
//        n -= length;
//        count += length;
//      }

  return count;
}

// Send an n byte message to the connected socket.

ASYS_INLINE ssize_t
ACE_MEM_IO::send (const void *buf, size_t n)
{
  ACE_TRACE ("ACE_MEM_IO::send");
  return this->send (buf, n, 0);
}

// Recv an n byte message from the connected socket.

ASYS_INLINE ssize_t
ACE_MEM_IO::recv (void *buf, size_t n)
{
  ACE_TRACE ("ACE_MEM_IO::recv");

  return this->recv (buf, n, 0);
}

ASYS_INLINE ssize_t
ACE_MEM_IO::fetch_recv_buf (int flag)
{
  ACE_TRACE ("ACE_MEM_IO::fetch_recv_buf");

  // This method can only be called when <buf_size_> == <cur_offset_>.
  ACE_ASSERT (this->buf_size_ == this->cur_offset_);

  // We have done using the previous buffer, return it to malloc.
  if (this->recv_buffer_ != 0)
    this->shm_malloc_->free (this->recv_buffer_);

  this->cur_offset_ = 0;
  off_t new_offset = 0;
  int retv = ACE_OS::recv (this->get_handle (),
                           (char *) &new_offset,
                           sizeof (off_t),
                           flag);

  if (retv != sizeof (off_t))
    {
      //  Nothing available or we are really screwed.
      this->buf_size_ = 0;
      this->recv_buffer_ = 0;
      return -1;
    }
  else
      this->buf_size_ = this->get_buf_len (new_offset,
                                           this->recv_buffer_);
  return this->buf_size_;
}

/*
// Send a vector of n byte messages to the connected socket.

ASYS_INLINE ssize_t
ACE_MEM_IO::sendv (const iovec iov[], size_t n) const
{
  ACE_TRACE ("ACE_MEM_IO::sendv");
  return ACE_OS::sendv (this->get_handle (), iov, n);
}

ASYS_INLINE ssize_t
ACE_MEM_IO::send (const iovec iov[], size_t n) const
{
  ACE_TRACE ("ACE_MEM_IO::send");
  return this->sendv (iov, n);
}

ASYS_INLINE ssize_t
ACE_MEM_IO::recv (iovec *io_vec,
                   const ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_MEM_IO::recv");
  return this->recvv (io_vec, timeout);
}

ASYS_INLINE ssize_t
ACE_MEM_IO::send (const void *buf, size_t n,
                   ACE_OVERLAPPED *overlapped) const
{
  ACE_TRACE ("ACE_MEM_IO::send");
  return ACE_OS::write (this->get_handle (),
                        (const char *) buf, n,
                        overlapped);
}

ASYS_INLINE ssize_t
ACE_MEM_IO::recv (void *buf, size_t n,
                   ACE_OVERLAPPED *overlapped) const
{
  ACE_TRACE ("ACE_MEM_IO::recv");
  return ACE_OS::read (this->get_handle (), (char *) buf, n,
                       overlapped);
}

ASYS_INLINE ssize_t
ACE_MEM_IO::send (const void *buf,
                   size_t len,
                   int flags,
                   const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_MEM_IO::send");
  return ACE::send (this->get_handle (), buf, len, flags, timeout);
}

ASYS_INLINE ssize_t
ACE_MEM_IO::recv (void *buf,
                   size_t len,
                   const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_MEM_IO::recv");
  return ACE::recv (this->get_handle (), buf, len, timeout);
}

ASYS_INLINE ssize_t
ACE_MEM_IO::send (const void *buf,
                   size_t len,
                   const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_MEM_IO::send");
  return ACE::send (this->get_handle (), buf, len, timeout);
}

ASYS_INLINE ssize_t
ACE_MEM_IO::recv (void *buf,
                   size_t len,
                   int flags,
                   const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_MEM_IO::recv");
  return ACE::recv (this->get_handle (), buf, len, flags, timeout);
}
*/
