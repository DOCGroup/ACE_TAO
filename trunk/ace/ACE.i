/* -*- C++ -*- */
// $Id$

// ACE.i
// Miscellaneous static methods used throughout ACE.

inline u_int
ACE::major_version ()
{
  return ACE_MAJOR_VERSION;
}

inline u_int
ACE::minor_version ()
{
  return ACE_MINOR_VERSION;
}

inline u_int
ACE::beta_version ()
{
  return ACE_BETA_VERSION;
}

inline ssize_t
ACE::send (ACE_HANDLE handle, const void *buf, size_t len)
{
  ACE_TRACE ("ACE::send");
  
#if defined (ACE_WIN32)
  return ACE_OS::send (handle, (const char *) buf, len);
#else
  return ACE_OS::write (handle, (const char *) buf, len);
#endif /* ACE_WIN32 */
}

inline ssize_t
ACE::send (ACE_HANDLE handle, const void *buf, size_t len, int flags)
{
  ACE_TRACE ("ACE::send");
  return ACE_OS::send (handle, (const char *) buf, len, flags);
}

inline ssize_t
ACE::recv (ACE_HANDLE handle, void *buf, size_t len)
{
  ACE_TRACE ("ACE::recv");
#if defined (ACE_WIN32)
    return ACE_OS::recv (handle, (char *) buf, len);
#else
    return ACE_OS::read (handle, (char *) buf, len);
#endif /* ACE_WIN32 */
}

inline ssize_t
ACE::recv (ACE_HANDLE handle, void *buf, size_t len, int flags)
{
  ACE_TRACE ("ACE::recv");

  return ACE_OS::recv (handle, (char *) buf, len, flags);
}

inline char *
ACE::strecpy (char *s, const char *t)
{
  ACE_TRACE ("ACE::strecpy");
  register char *dscan = s;
  register const char *sscan = t;

  while ((*dscan++ = *sscan++) != '\0')
    continue;

  return dscan - 1;
}

// Return flags currently associated with handle.

inline int
ACE::get_flags (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE::get_flags");

  return ACE_OS::fcntl (handle, F_GETFL, 0);
}

inline u_long
ACE::log2 (u_long num)
{
  u_long log = 0;

  for (;
       num > 0;
       log++)
    num >>= 1;

  return log;
}
