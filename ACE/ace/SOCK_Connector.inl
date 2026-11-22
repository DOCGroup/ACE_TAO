// -*- C++ -*-
ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
ACE_SOCK_Connector::~ACE_SOCK_Connector ()
{
  ACE_TRACE ("ACE_SOCK_Connector::~ACE_SOCK_Connector");
}

/// Default constructor.
ACE_INLINE
ACE_SOCK_Connector::ACE_SOCK_Connector ()
{
  ACE_TRACE ("ACE_SOCK_Connector::ACE_SOCK_Connector");
}

ACE_INLINE bool
ACE_SOCK_Connector::reset_new_handle (ACE_HANDLE handle)
{
#if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
  // Reset the event association
  return ::WSAEventSelect ((SOCKET) handle,
                           0,
                           0);
#else /* !defined ACE_HAS_WINSOCK2 */
  ACE_UNUSED_ARG (handle);
  return false;
#endif /* ACE_WIN32 */
}

ACE_END_VERSIONED_NAMESPACE_DECL
