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

ASYS_INLINE ssize_t
ACE_MEM_IO::fetch_recv_buf (int flag, const ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_MEM_IO::fetch_recv_buf");

  // This method can only be called when <buf_size_> == <cur_offset_>.
  ACE_ASSERT (this->buf_size_ == this->cur_offset_);

  // We have done using the previous buffer, return it to malloc.
  if (this->recv_buffer_ != 0)
    this->release_buffer (this->recv_buffer_);

  this->cur_offset_ = 0;
  off_t new_offset = 0;
  int retv = ACE::recv (this->get_handle (),
                        (char *) &new_offset,
                        sizeof (off_t),
                        flag,
                        timeout);

  if (retv == 0)
    return 0;
  else if (retv != sizeof (off_t))
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

ASYS_INLINE
ACE_MEM_IO::~ACE_MEM_IO (void)
{
  // ACE_TRACE ("ACE_MEM_IO::~ACE_MEM_IO");
}

ASYS_INLINE ssize_t
ACE_MEM_IO::send (const void *buf,
                   size_t len,
                   int flags,
                   const ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_MEM_IO::send");
  void *sbuf = this->acquire_buffer (len);
  if (sbuf == 0)
    return -1;                  // Memory buffer not initialized.
  ACE_OS::memcpy (sbuf, buf, len);
  off_t offset = this->set_buf_len (sbuf, len); // <set_buf_len> also calculate
                                              // the offset.

  // Send the offset value over the socket.
  if (ACE::send (this->get_handle (),
                 (const char *) &offset,
                 sizeof (offset),
                 flags,
                 timeout) != sizeof (offset))
    {
      // unsucessful send, release the memory in the shared-memory.
      this->release_buffer (sbuf);

      return -1;
    }
  return len;
}

ASYS_INLINE ssize_t
ACE_MEM_IO::recv (void *buf,
                   size_t len,
                   int flags,
                   const ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_MEM_IO::recv");

  size_t count = 0;

//    while (len > 0)
//      {
      size_t buf_len = this->buf_size_ - this->cur_offset_;
      if (buf_len == 0)
        {
          ssize_t blen =         // Buffer length
            this->fetch_recv_buf (flags, timeout);
          if (blen <= 0)
            return blen;
          buf_len = this->buf_size_;
        }

      size_t length = (len > buf_len ? buf_len : len);

      ACE_OS::memcpy ((char *) buf + count,
                      (char *) this->recv_buffer_ + this->cur_offset_,
                      length);
      this->cur_offset_ += length;
//        len -= length;
      count += length;
//      }

  return count;
}

ASYS_INLINE ssize_t
ACE_MEM_IO::send (const void *buf, size_t n, int flags)
{
  ACE_TRACE ("ACE_MEM_IO::send");
  return this->send (buf, n, flags, 0);
}

// Recv an n byte message from the connected socket.

ASYS_INLINE ssize_t
ACE_MEM_IO::recv (void *buf, size_t n, int flags)
{
  ACE_TRACE ("ACE_MEM_IO::recv");
  return this->recv (buf, n, flags, 0);
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
ACE_MEM_IO::recv (void *buf,
                   size_t len,
                   const ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_MEM_IO::recv");
  return this->recv (buf, len, 0, timeout);
}

ASYS_INLINE ssize_t
ACE_MEM_IO::send (const void *buf,
                   size_t len,
                   const ACE_Time_Value *timeout)
{
  ACE_TRACE ("ACE_MEM_IO::send");
  return this->send (buf, len, 0, timeout);
}
