// -*- C++ -*-
// $Id$

ACE_INLINE int
ACE_OS::last_error (void)
{
  // ACE_OS_TRACE ("ACE_OS::last_error");

#if defined (ACE_WIN32)
  int lerror = ::GetLastError ();
  int lerrno = errno;
  return lerrno == 0 ? lerror : lerrno;
#else
  return errno;
#endif /* ACE_WIN32 */
}

ACE_INLINE void
ACE_OS::last_error (int error)
{
  ACE_OS_TRACE ("ACE_OS::last_error");
#if defined (ACE_WIN32)
  ::SetLastError (error);
#endif /* ACE_WIN32 */
  errno = error;
}

ACE_INLINE int
ACE_OS::set_errno_to_last_error (void)
{
# if defined (ACE_WIN32)
// Borland C++ Builder 4 has a bug in the RTL that resets the
// <GetLastError> value to zero when errno is accessed.  Thus, we have
// to use this to set errno to GetLastError.  It's bad, but only for
// WIN32.
#   if defined(__BORLANDC__) && (__BORLANDC__ == 0x540) || defined (__IBMCPP__) && (__IBMCPP__ >= 400)
  int last_error = ::GetLastError ();
  return errno = last_error;
#   else /* defined(__BORLANDC__) && (__BORLANDC__ == 0x540) */
  return errno = ::GetLastError ();
#   endif /* defined(__BORLANDC__) && (__BORLANDC__ == 0x540) */
#else
  return errno;
# endif /* defined(ACE_WIN32) */
}

ACE_INLINE int
ACE_OS::set_errno_to_wsa_last_error (void)
{
# if defined (ACE_WIN32)
// Borland C++ Builder 4 has a bug in the RTL that resets the
// <GetLastError> value to zero when errno is accessed.  Thus, we have
// to use this to set errno to GetLastError.  It's bad, but only for
// WIN32
#   if defined(__BORLANDC__) && (__BORLANDC__ == 0x540) || defined (__IBMCPP__) && (__IBMCPP__ >= 400)
  int last_error = ::WSAGetLastError ();
  return errno = last_error;
#   else /* defined(__BORLANDC__) && (__BORLANDC__ == 0x540) */
  return errno = ::WSAGetLastError ();
#   endif /* defined(__BORLANDC__) && (__BORLANDC__ == 0x540) */
#else
  return errno;
# endif /* defined(ACE_WIN32) */
}
