// -*- C++ -*-
// $Id$

// SOCK_Stream.i

ASYS_INLINE void
ACE_SSL_SOCK_Stream::set_handle (ACE_HANDLE fd)
{
  if (this->ssl_ == 0 || fd == ACE_INVALID_HANDLE)
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
  : ssl_ (0),
    stream_ ()
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::ACE_SSL_SOCK_Stream");

  ACE_SSL_Context * ctx =
    (context == 0 ? ACE_SSL_Context::instance () : context);

  this->ssl_ = ::SSL_new (ctx->context ());

  if (this->ssl_ == 0)
    ACE_ERROR ((LM_ERROR,
		"(%P|%t) ACE_SSL_SOCK_Stream "
                "- cannot allocate new SSL structure %p\n",
		ACE_TEXT ("")));

  ::SSL_set_verify (this->ssl_,
                    ctx->default_verify_mode (),
                    0);
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

  // @@ FIXME: Not thread safe!

  // No send flags are supported in SSL.
  if (flags != 0)
    ACE_NOTSUP_RETURN (-1);

  int status = ::SSL_write (this->ssl_,
                            ACE_static_cast (const char*, buf),
                            n);

  if (status <= 0)
    {
      switch (::SSL_get_error (this->ssl_, status))
        {
        case SSL_ERROR_WANT_WRITE:
          errno = EWOULDBLOCK;
          break;
        default:
#ifndef ACE_NDEBUG
          ERR_print_errors_fp (stderr);
#endif  /* ACE_NDEBUG */
          break;
        }
    }

  return status;
}

ASYS_INLINE ssize_t
ACE_SSL_SOCK_Stream::recv (void *buf,
                           size_t n,
                           int flags) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::recv");

  // @@ FIXME: Not thread safe!

  if (flags)
    {
      if (ACE_BIT_ENABLED (flags, MSG_PEEK))
        return ::SSL_peek (this->ssl_,
                           ACE_static_cast (char*, buf),
                           n);
      else
        ACE_NOTSUP_RETURN (-1);
    }

  int status = 0;
  do
    {
      status = ::SSL_read (this->ssl_,
                           ACE_static_cast (char *, buf),
                           n);

      switch (::SSL_get_error (this->ssl_, status))
        {
        case SSL_ERROR_NONE:
          return status;
        case SSL_ERROR_WANT_READ:
          break;
        case SSL_ERROR_ZERO_RETURN:
          // @@ This appears to be the right/expected thing to do.
          //    However, it'd be nice if someone could verify this.
          //
          // The peer has notified us that it is shutting down via
          // the SSL "close_notify" message so we need to
          // shutdown, too.
          (void) ::SSL_shutdown (this->ssl_);
          return 0;
        default:
#ifndef ACE_NDEBUG
          ERR_print_errors_fp (stderr);
#endif  /* ACE_NDEBUG */
          return -1;
        }
    }
  while (::SSL_pending (this->ssl_));

  // If we get this far then, we would have blocked.
  errno = EWOULDBLOCK;

  return status;
}

ASYS_INLINE ssize_t
ACE_SSL_SOCK_Stream::send (const void *buf,
                           size_t n) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::send");

  // @@ FIXME: Not thread safe!

  int status = ::SSL_write (this->ssl_,
                            ACE_static_cast (const char *, buf),
                            n);

  if (status <= 0)
    {
      switch (::SSL_get_error (this->ssl_, status))
        {
        case SSL_ERROR_WANT_WRITE:
          errno = EWOULDBLOCK;
          break;
        default:
#ifndef ACE_NDEBUG
          ERR_print_errors_fp (stderr);
#endif  /* ACE_NDEBUG */
          break;
        }
    }

  return status;
}

ASYS_INLINE ssize_t
ACE_SSL_SOCK_Stream::recv (void *buf,
                           size_t n) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::recv");

  // @@ FIXME: Not thread safe!

  int status = 0;

  do
    {
      status = ::SSL_read (this->ssl_,
                           ACE_static_cast (char*, buf),
                           n);

      switch (::SSL_get_error (this->ssl_, status))
        {
        case SSL_ERROR_NONE:
          return status;
        case SSL_ERROR_WANT_READ:
          break;
        case SSL_ERROR_ZERO_RETURN:
          // @@ This appears to be the right/expected thing to do.
          //    However, it'd be nice if someone could verify this.
          //
          // The peer has notified us that it is shutting down via
          // the SSL "close_notify" message so we need to
          // shutdown, too.
          (void) ::SSL_shutdown (this->ssl_);
          return 0;
        default:
#ifndef ACE_NDEBUG
          ERR_print_errors_fp (stderr);
#endif  /* ACE_NDEBUG */
          return -1;
        }
    }
  while (::SSL_pending (this->ssl_));

  // If we get this far then, we would have blocked.
  errno = EWOULDBLOCK;

  return status;
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

  if (this->ssl_ == 0 || this->get_handle () == ACE_INVALID_HANDLE)
    return 0;  // SSL_SOCK_Stream was never opened.

  // SSL_shutdown() returns 1 on successful shutdown of the SSL
  // connection, not 0.
  int status = ::SSL_shutdown (this->ssl_);


  if (status <= 0)
    {
      switch (::SSL_get_error (this->ssl_, status))
      {
#if 0
      case SSL_ERROR_WANT_WRITE:
        ACE_DEBUG ((LM_DEBUG, "SSL_ERROR_WANT_WRITE\n"));
        break;
      case SSL_ERROR_WANT_READ:
        ACE_DEBUG ((LM_DEBUG, "SSL_ERROR_WANT_READ\n"));
        break;
      case SSL_ERROR_WANT_X509_LOOKUP:
        ACE_DEBUG ((LM_DEBUG, "SSL_ERROR_WANT_X509_LOOKUP\n"));
        break;
      case SSL_ERROR_NONE:
        ACE_DEBUG ((LM_DEBUG, "SSL_ERROR_NONE\n"));
        break;
#endif  /* 0 */
      case SSL_ERROR_SYSCALL:
        // ACE_DEBUG ((LM_DEBUG, "SSL_ERROR_SYSCALL\n"));

        // Ignore this error condition.

        // Don't set the handle in OpenSSL; only in the
        // SSL_SOCK_Stream.  We do this to avoid any potential side
        // effects.  Invoking ACE_SSL_SOCK::set_handle() bypasses the
        // OpenSSL SSL_set_fd() call ACE_SSL_SOCK_Stream::set_handle()
        // does.
        this->ACE_SSL_SOCK::set_handle (ACE_INVALID_HANDLE);

        return this->stream_.close ();
        // break;
#if 0
      case SSL_ERROR_ZERO_RETURN:
        ACE_DEBUG ((LM_DEBUG, "SSL_ERROR_ZERO_RETURN\n"));
        break;
#endif /* 0 */
      default:
#ifndef ACE_NDEBUG
        // ACE_DEBUG ((LM_DEBUG, "STATUS = %d\n", status));
#endif  /* ACE_NDEBUG */
        break;
      }

      // Save/restore errno
      ACE_Errno_Guard error (errno);
      (void) this->stream_.close ();

      return -1;
    }

  // Don't set the handle in OpenSSL; only in the SSL_SOCK_Stream.  We
  // do this to avoid any potential side effects.
  // Invoking ACE_SSL_SOCK::set_handle() bypasses the OpenSSL
  // SSL_set_fd() call ACE_SSL_SOCK_Stream::set_handle() does.
  this->ACE_SSL_SOCK::set_handle (ACE_INVALID_HANDLE);

  return this->stream_.close ();
}

ASYS_INLINE ACE_SOCK_Stream &
ACE_SSL_SOCK_Stream::peer (void)
{
  ACE_TRACE ("ACE_SSL_SOCK_Stream::peer");
  return this->stream_;
}

ASYS_INLINE SSL *
ACE_SSL_SOCK_Stream::ssl (void) const
{
  return this->ssl_;
}
