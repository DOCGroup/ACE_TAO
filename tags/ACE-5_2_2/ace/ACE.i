/* -*- C++ -*- */
// $Id$

// Wrappers for methods that have been moved to ACE_OS.

ASYS_INLINE ssize_t
ACE::read_n (ACE_HANDLE handle,
             void *buf,
             size_t len,
             size_t *bytes_transferred)
{
  return ACE_OS::read_n (handle,
                         buf,
                         len,
                         bytes_transferred);
}

ASYS_INLINE ssize_t
ACE::write_n (ACE_HANDLE handle,
              const void *buf,
              size_t len,
              size_t *bytes_transferred)
{
  return ACE_OS::write_n (handle,
                          buf,
                          len,
                          bytes_transferred);
}

ASYS_INLINE ssize_t
ACE::recv_n (ACE_HANDLE handle,
             void *buf,
             size_t len,
             int flags,
             const ACE_Time_Value *timeout,
             size_t *bytes_transferred)
{
  if (timeout == 0)
    return ACE::recv_n_i (handle,
                          buf,
                          len,
                          flags,
                          bytes_transferred);
  else
    return ACE::recv_n_i (handle,
                          buf,
                          len,
                          flags,
                          timeout,
                          bytes_transferred);
}

#if defined (ACE_HAS_TLI)

ASYS_INLINE ssize_t
ACE::t_rcv_n (ACE_HANDLE handle,
              void *buf,
              size_t len,
              int *flags,
              const ACE_Time_Value *timeout,
              size_t *bytes_transferred)
{
  if (timeout == 0)
    return ACE::t_rcv_n_i (handle,
                           buf,
                           len,
                           flags,
                           bytes_transferred);
  else
    return ACE::t_rcv_n_i (handle,
                           buf,
                           len,
                           flags,
                           timeout,
                           bytes_transferred);
}

#endif /* ACE_HAS_TLI */

ASYS_INLINE ssize_t
ACE::recv_n (ACE_HANDLE handle,
             void *buf,
             size_t len,
             const ACE_Time_Value *timeout,
             size_t *bytes_transferred)
{
  if (timeout == 0)
    return ACE::recv_n_i (handle,
                          buf,
                          len,
                          bytes_transferred);
  else
    return ACE::recv_n_i (handle,
                          buf,
                          len,
                          timeout,
                          bytes_transferred);
}

ASYS_INLINE ssize_t
ACE::recvv_n (ACE_HANDLE handle,
              iovec *iov,
              int iovcnt,
              const ACE_Time_Value *timeout,
              size_t *bytes_transferred)
{
  if (timeout == 0)
    return ACE::recvv_n_i (handle,
                           iov,
                           iovcnt,
                           bytes_transferred);
  else
    return ACE::recvv_n_i (handle,
                           iov,
                           iovcnt,
                           timeout,
                           bytes_transferred);
}

ASYS_INLINE ssize_t
ACE::send_n (ACE_HANDLE handle,
             const void *buf,
             size_t len,
             int flags,
             const ACE_Time_Value *timeout,
             size_t *bytes_transferred)
{
  if (timeout == 0)
    return ACE::send_n_i (handle,
                          buf,
                          len,
                          flags,
                          bytes_transferred);
  else
    return ACE::send_n_i (handle,
                          buf,
                          len,
                          flags,
                          timeout,
                          bytes_transferred);
}

#if defined (ACE_HAS_TLI)

ASYS_INLINE ssize_t
ACE::t_snd_n (ACE_HANDLE handle,
              const void *buf,
              size_t len,
              int flags,
              const ACE_Time_Value *timeout,
              size_t *bytes_transferred)
{
  if (timeout == 0)
    return ACE::t_snd_n_i (handle,
                           buf,
                           len,
                           flags,
                           bytes_transferred);
  else
    return ACE::t_snd_n_i (handle,
                           buf,
                           len,
                           flags,
                           timeout,
                           bytes_transferred);
}

#endif /* ACE_HAS_TLI */

ASYS_INLINE ssize_t
ACE::send_n (ACE_HANDLE handle,
             const void *buf,
             size_t len,
             const ACE_Time_Value *timeout,
             size_t *bytes_transferred)
{
  if (timeout == 0)
    return ACE::send_n_i (handle,
                          buf,
                          len,
                          bytes_transferred);
  else
    return ACE::send_n_i (handle,
                          buf,
                          len,
                          timeout,
                          bytes_transferred);
}

ASYS_INLINE ssize_t
ACE::sendv_n (ACE_HANDLE handle,
              const iovec *iov,
              int iovcnt,
              const ACE_Time_Value *timeout,
              size_t *bytes_transferred)
{
  if (timeout == 0)
    return ACE::sendv_n_i (handle,
                           iov,
                           iovcnt,
                           bytes_transferred);
  else
    return ACE::sendv_n_i (handle,
                           iov,
                           iovcnt,
                           timeout,
                           bytes_transferred);
}

ASYS_INLINE ssize_t
ACE::send_i (ACE_HANDLE handle, const void *buf, size_t len)
{
#if defined (ACE_WIN32) || defined (ACE_PSOS) || defined (HPUX)
  return ACE_OS::send (handle, (const char *) buf, len);
#else
  return ACE_OS::write (handle, (const char *) buf, len);
#endif /* ACE_WIN32 */
}

ASYS_INLINE ssize_t
ACE::recv_i (ACE_HANDLE handle, void *buf, size_t len)
{
#if defined (ACE_WIN32) || defined (ACE_PSOS)
  return ACE_OS::recv (handle, (char *) buf, len);
#else
  return ACE_OS::read (handle, (char *) buf, len);
#endif /* ACE_WIN32 */
}

ASYS_INLINE int
ACE::handle_read_ready (ACE_HANDLE handle,
                        const ACE_Time_Value *timeout)
{
  return ACE::handle_ready (handle,
                            timeout,
                            1,
                            0,
                            0);
}

ASYS_INLINE int
ACE::handle_write_ready (ACE_HANDLE handle,
                         const ACE_Time_Value *timeout)
{
  return ACE::handle_ready (handle,
                            timeout,
                            0,
                            1,
                            0);
}

ASYS_INLINE int
ACE::handle_exception_ready (ACE_HANDLE handle,
                             const ACE_Time_Value *timeout)
{
  return ACE::handle_ready (handle,
                            timeout,
                            0,
                            0,
                            1);
}

ASYS_INLINE void
ACE::unique_name (const void *object,
                  ACE_TCHAR *name,
                  size_t length)
{
  ACE_OS::unique_name (object, name, length);
}

ASYS_INLINE u_long
ACE::log2 (u_long num)
{
  u_long log = 0;

  for (;
       num > 0;
       log++)
    num >>= 1;

  return log;
}

ASYS_INLINE ACE_TCHAR
ACE::nibble2hex (u_int n)
{
  // @@ UNICODE does this work?
  return ACE::hex_chars_[n & 0x0f];
}

ASYS_INLINE u_char
ACE::hex2byte (ACE_TCHAR c)
{
  if (isdigit (c))
    return (u_char) (c - ACE_LIB_TEXT ('0'));
  else if (islower (c))
    return (u_char) (10 + c - ACE_LIB_TEXT ('a'));
  else
    return (u_char) (10 + c - ACE_LIB_TEXT ('A'));
}

ASYS_INLINE char
ACE::debug (void)
{
  return ACE::debug_;
}

ASYS_INLINE void
ACE::debug (char c)
{
  ACE::debug_ = c;
}

ASYS_INLINE char *
ACE::strnew (const char *s)
{
  if (s == 0)
    return 0;
  char *t = 0;
  ACE_NEW_RETURN (t, 
                  char [::strlen (s) + 1],
                  0);
  if (t == 0)
    return 0;
  else
    return ACE_OS::strcpy (t, s);
}

#if defined (ACE_HAS_WCHAR)
ASYS_INLINE wchar_t *
ACE::strnew (const wchar_t *s)
{
  wchar_t *t = 0;
  ACE_NEW_RETURN (t, 
                  wchar_t[ACE_OS_String::strlen (s) + 1], 
                  0);
  if (t == 0)
    return 0;
  else
    return ACE_OS::strcpy (t, s);
}
#endif /* ACE_HAS_WCHAR */
