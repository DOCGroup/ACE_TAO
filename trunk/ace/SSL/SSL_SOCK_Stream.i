// -*- C++ -*-
// $Id$

// SOCK_Stream.i

#if defined (ACE_HAS_SSL)

ASYS_INLINE void
ACE_SSL_SOCK_Stream::set_handle (ACE_HANDLE fd)
{
  if (this->ssl_ == 0)
    {
      this->ACE_SSL_SOCK::set_handle (ACE_INVALID_HANDLE);
      return;
    }
  else
    {
      (void) ::SSL_set_fd (this->ssl_, (int) fd);
      this->ACE_SSL_SOCK::set_handle (fd);
      this->stream_.set_handle (fd);
    }
}

ASYS_INLINE
ACE_SSL_SOCK_Stream::ACE_SSL_SOCK_Stream (ACE_SSL_Context *context)
  : context_ (context == 0 ? ACE_SSL_Context::instance () : context),
    ssl_ (::SSL_new (this->context_->context ()))
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::ACE_SSL_SOCK_Stream");

  if (this->ssl_ == 0)
    ACE_ERROR ((LM_ERROR,
		"(%P|%t) ACE_SSL_SOCK_Stream - cannot allocate new "
                "SSL structure%p\n",
		""));

  ::SSL_set_verify (this->ssl_,
                    this->context_->default_verify_mode (),
                    0);
}

ASYS_INLINE
ACE_SSL_SOCK_Stream::ACE_SSL_SOCK_Stream (ACE_HANDLE h,
                                          ACE_SSL_Context *context)
  : context_ (context == 0 ? ACE_SSL_Context::instance () : context),
    ssl_ (::SSL_new (this->context_->context ())),
    stream_ (h)
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::ACE_SSL_SOCK_Stream");

  if (this->ssl_ == 0)
    ACE_ERROR ((LM_ERROR,
		"(%P|%t) ACE_SSL - cannot allocate new SSL session:%p\n",
		""));

  ::SSL_set_verify (this->ssl_,
                    this->context_->default_verify_mode (),
                    0);

  this->set_handle (h);
}

ASYS_INLINE
ACE_SSL_SOCK_Stream::~ACE_SSL_SOCK_Stream (void)
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::~ACE_SSL_SOCK_Stream");

  ::SSL_free (this->ssl_);
  this->ssl_ = 0;

  // @@ Question: should we reference count the Context object or
  // leave that to the application developer? We do not reference
  // count reactors (for example) and following some simple rules
  // seems to work fine!
}

ASYS_INLINE ssize_t
ACE_SSL_SOCK_Stream::send (const void *buf,
                           size_t n,
                           int flags) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::send");

  // No send flags are supported in SSL.
  if (flags != 0)
    ACE_NOTSUP_RETURN (-1);

  return ::SSL_write (this->ssl_,
                      ACE_static_cast (const char*, buf),
                      n);
}

ASYS_INLINE ssize_t
ACE_SSL_SOCK_Stream::recv (void *buf,
                           size_t n,
                           int flags) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::recv");

  if (flags)
    {
      if (ACE_BIT_ENABLED (flags, MSG_PEEK))
        return ::SSL_peek (this->ssl_,
                           ACE_static_cast (char*, buf),
                           n);
      else
        ACE_NOTSUP_RETURN (-1);
    }

  return ::SSL_read (this->ssl_, ACE_static_cast (char *, buf), n);
}

ASYS_INLINE ssize_t
ACE_SSL_SOCK_Stream::send (const void *buf,
                           size_t n) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::send");

  return ::SSL_write (this->ssl_,
                      ACE_static_cast (const char *, buf),
                      n);
}

ASYS_INLINE ssize_t
ACE_SSL_SOCK_Stream::recv (void *buf,
                           size_t n) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::recv");

  return ::SSL_read (this->ssl_, ACE_static_cast (char*, buf), n);
}

ASYS_INLINE ssize_t
ACE_SSL_SOCK_Stream::send (const void *buf,
                           size_t len,
                           const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::send");
  return this->send (buf, len, 0, timeout);
}

ASYS_INLINE ssize_t
ACE_SSL_SOCK_Stream::recv (void *buf,
                           size_t n,
                           const ACE_Time_Value *timeout) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::recv");
  return this->recv (buf, n, 0, timeout);
}

ASYS_INLINE ssize_t
ACE_SSL_SOCK_Stream::recv_n (void *buf, int buf_size) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::recv_n");
  return this->recv_n (buf, buf_size, 0);
}

ASYS_INLINE ssize_t
ACE_SSL_SOCK_Stream::send_n (const void *buf, int len) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::send_n");
  return this->send_n (buf, len, 0);
}

ASYS_INLINE int
ACE_SSL_SOCK_Stream::close_reader (void)
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::close_reader");
  return this->stream_.close_reader ();
}

ASYS_INLINE int
ACE_SSL_SOCK_Stream::close_writer (void)
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::close_writer");
  return this->stream_.close_writer ();
}

ASYS_INLINE int
ACE_SSL_SOCK_Stream::close (void)
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::close");

  if (this->ssl_ == 0)
    return -1;

  ::SSL_shutdown (this->ssl_);

  return this->stream_.close ();
}

ASYS_INLINE ACE_SOCK_Stream &
ACE_SSL_SOCK_Stream::peer (void)
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::peer");
  return this->stream_;
}

ASYS_INLINE ACE_SSL_Context *
ACE_SSL_SOCK_Stream::context (void) const
{
  return this->context_;
}

ASYS_INLINE SSL *
ACE_SSL_SOCK_Stream::ssl (void) const
{
  return this->ssl_;
}



#endif /* ACE_HAS_SSL */
