/* -*- C++ -*- */
// $Id$

// SOCK_Connector.i

// This constructor is used by a client when it wants to connect to
// the specified REMOTE_SAP address using a blocking open.

ASYS_INLINE
ACE_SOCK_Connector::~ACE_SOCK_Connector (void)
{
  ACE_TRACE ("ACE_SOCK_Connector::~ACE_SOCK_Connector");
}

// Do-nothing constructor...

ASYS_INLINE
ACE_SOCK_Connector::ACE_SOCK_Connector (void)
{
  ACE_TRACE ("ACE_SOCK_Connector::ACE_SOCK_Connector");
}

ASYS_INLINE int
ACE_SOCK_Connector::reset_new_handle (ACE_HANDLE handle)
{
#if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
  // Reset the event association
  return ::WSAEventSelect ((SOCKET) handle,
                           0,
                           0);
#else /* !defined ACE_HAS_WINSOCK2 */
  ACE_UNUSED_ARG (handle);
  return 0;
#endif /* ACE_WIN32 */
}
