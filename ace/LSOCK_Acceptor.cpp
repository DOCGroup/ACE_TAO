// LSOCK_Acceptor.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/LSOCK_Acceptor.h"

#if !defined (__ACE_INLINE__)
#include "ace/LSOCK_Acceptor.i"
#endif /* __ACE_INLINE__ */

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

ACE_ALLOC_HOOK_DEFINE(ACE_LSOCK_Acceptor)

// Return the local endpoint address.

int 
ACE_LSOCK_Acceptor::get_local_addr (ACE_Addr &a) const
{
  ACE_TRACE ("ACE_LSOCK_Acceptor::get_local_addr");
  a = this->local_addr_;
  return 0;
}

void
ACE_LSOCK_Acceptor::dump (void) const
{
  ACE_TRACE ("ACE_LSOCK_Acceptor::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->local_addr_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

// Do nothing routine for constructor.

ACE_LSOCK_Acceptor::ACE_LSOCK_Acceptor (void)
{
  ACE_TRACE ("ACE_LSOCK_Acceptor::ACE_LSOCK_Acceptor");
}

int
ACE_LSOCK_Acceptor::open (const ACE_Addr &remote_sap, 
			  int reuse_addr, 
			  int protocol_family, 
			  int backlog, 
			  int protocol)
{
  ACE_TRACE ("ACE_LSOCK_Acceptor::open");
  this->local_addr_ = *((ACE_UNIX_Addr *) &remote_sap); // This is a gross hack...
  return ACE_SOCK_Acceptor::open (remote_sap, reuse_addr, 
				  protocol_family, backlog, protocol);
}

// General purpose routine for performing server ACE_SOCK creation.

ACE_LSOCK_Acceptor::ACE_LSOCK_Acceptor (const ACE_Addr &remote_sap, 
					int reuse_addr, 
					int protocol_family, 
					int backlog, 
					int protocol)
{
  ACE_TRACE ("ACE_LSOCK_Acceptor::ACE_LSOCK_Acceptor");
  if (this->open (remote_sap, reuse_addr,
		  protocol_family, backlog, protocol) == -1)
    ACE_ERROR ((LM_ERROR, "ACE_LSOCK_Acceptor::ACE_LSOCK_Acceptor"));
}

// General purpose routine for accepting new connections.

int
ACE_LSOCK_Acceptor::accept (ACE_LSOCK_Stream &new_local_ipc_sap, 
			    ACE_Addr *remote_addr, 
			    ACE_Time_Value *timeout,
			    int restart,
                            int reset_new_handle) const
{
  ACE_TRACE ("ACE_LSOCK_Acceptor::accept");
  ACE_HANDLE new_handle = 
    ACE_SOCK_Acceptor::shared_accept (remote_addr, timeout, restart, reset_new_handle);
  new_local_ipc_sap.set_handle (new_handle);
  return new_handle == ACE_INVALID_HANDLE ? -1 : 0;
}

// Close down the UNIX domain stream and remove the rendezvous point
// from the file system.

int
ACE_LSOCK_Acceptor::remove (void)
{
  ACE_TRACE ("ACE_LSOCK_Acceptor::remove");
  int result = this->close ();
  return ACE_OS::unlink (this->local_addr_.get_path_name ()) == -1 
    || result == -1 ? -1 : 0;
}

#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
