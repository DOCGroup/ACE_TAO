/* -*- C++ -*- */
// $Id$

// LSOCK_Connector.i

// Establish a connection.

inline int 
ACE_LSOCK_Connector::connect (ACE_LSOCK_Stream &new_stream, 
			      const ACE_UNIX_Addr &remote_sap, 
			      ACE_Time_Value *timeout,
			      const ACE_Addr &local_sap,
			      int reuse_addr,
			      int flags,
			      int perms,
			      int protocol_family, 
			      int protocol)
{
  ACE_TRACE ("ACE_LSOCK_Connector::connect");
  int result = ACE_SOCK_Connector::connect (new_stream, remote_sap, 
					    timeout, local_sap, 
					    reuse_addr, flags, perms,
					    protocol_family, protocol);
  if (result != -1)
    // This is necessary due to the weird inheritance relationships of ACE_LSOCK_Stream.
    new_stream.set_handle (new_stream.get_handle ());
  return result;
}

