/* -*- C++ -*- */
// $Id$

// SOCK_Connector.i

// This constructor is used by a client when it wants to connect to
// the specified REMOTE_SAP address using a blocking open.

inline
ACE_SOCK_Connector::ACE_SOCK_Connector (ACE_SOCK_Stream &new_stream, 
					const ACE_Addr &remote_sap, 
					ACE_Time_Value *timeout,
					const ACE_Addr &local_sap,
					int reuse_addr,
					int flags,
					int perms,
					int protocol_family,
					int protocol)
{
  ACE_TRACE ("ACE_SOCK_Connector::ACE_SOCK_Connector");
  if (this->connect (new_stream, remote_sap, timeout, local_sap,
		     reuse_addr, flags, perms, protocol_family, protocol) == -1
      && timeout != 0 && !(errno == EWOULDBLOCK || errno == ETIME))
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_SOCK_Connector::ACE_SOCK_Connector"));
}

// Do-nothing constructor...

inline
ACE_SOCK_Connector::ACE_SOCK_Connector (void)
{
  ACE_TRACE ("ACE_SOCK_Connector::ACE_SOCK_Connector");
}

inline int 
ACE_SOCK_Connector::reset_new_handle (ACE_HANDLE handle)
{
  ACE_UNUSED_ARG (handle);
#if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
  // Reset the event association 
  return ::WSAEventSelect ((SOCKET) handle,
                           NULL,
                           0);    
#else /* !defined ACE_HAS_WINSOCK2 */
  return 0;
#endif /* ACE_WIN32 */
}

