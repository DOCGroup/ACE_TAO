// SOCK_Acceptor.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/SOCK_Acceptor.h"

#if !defined (__ACE_INLINE__)
#include "ace/SOCK_Acceptor.i"
#endif /* __ACE_INLINE__ */

#include "ace/Synch.h"

ACE_ALLOC_HOOK_DEFINE(ACE_SOCK_Acceptor)

// Do nothing routine for constructor. 

ACE_SOCK_Acceptor::ACE_SOCK_Acceptor (void)
{
  ACE_TRACE ("ACE_SOCK_Acceptor::ACE_SOCK_Acceptor");
}

// General purpose routine for accepting new connections. 

int
ACE_SOCK_Acceptor::accept (ACE_SOCK_Stream &new_stream, ACE_Addr *remote_addr, 
			   ACE_Time_Value *timeout, int restart) const
{
  ACE_TRACE ("ACE_SOCK_Acceptor::accept");
  ACE_HANDLE new_handle = this->shared_accept (remote_addr, timeout, restart);
  new_stream.set_handle (new_handle);
  return new_handle == ACE_INVALID_HANDLE ? -1 : 0;
}

// General purpose routine for performing server ACE_SOCK creation.

ACE_SOCK_Acceptor::ACE_SOCK_Acceptor (const ACE_Addr &local_sap, 
				      int reuse_addr, 
				      int protocol_family,
				      int backlog, 
				      int protocol)
{
  ACE_TRACE ("ACE_SOCK_Acceptor::ACE_SOCK_Acceptor");
  if (this->open (local_sap, reuse_addr, protocol_family, 
		  backlog, protocol) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_SOCK_Acceptor"));
}

void
ACE_SOCK_Acceptor::dump (void) const
{
  ACE_TRACE ("ACE_SOCK_Acceptor::dump");
}

// General purpose routine for performing server ACE_SOCK creation.

int
ACE_SOCK_Acceptor::open (const ACE_Addr &local_sap, 
			 int reuse_addr,
			 int protocol_family, 
			 int backlog, 
			 int protocol)
{
  ACE_TRACE ("ACE_SOCK_Acceptor::open");
  int error = 0;

  if (ACE_SOCK::open (SOCK_STREAM, protocol_family, 
		      protocol, reuse_addr) == -1)
    error = 1;
  else if (&local_sap == &ACE_Addr::sap_any 
	   && protocol_family == PF_INET)
    {
      if (ACE::bind_port (this->get_handle ()) == -1)
	error = 1;
    }
  else if (ACE_OS::bind (this->get_handle (), (sockaddr *) local_sap.get_addr (), 
		   local_sap.get_size ()) == -1)
    error = 1;

  if (error || ACE_OS::listen (this->get_handle (), backlog) == -1)
    this->close ();

  return error ? -1 : 0;
}
    
// Performs the timed accept operation. 

ACE_HANDLE
ACE_SOCK_Acceptor::shared_accept (ACE_Addr *remote_addr, 
				  ACE_Time_Value *timeout, 
				  int restart) const
{
  ACE_TRACE ("ACE_SOCK_Acceptor::shared_accept");
  sockaddr *addr = 0;
  int *len_ptr = 0;
  int len;
  ACE_HANDLE new_handle;

  if (remote_addr != 0)
    {
      len = remote_addr->get_size ();
      len_ptr = &len;
      addr = (sockaddr *) remote_addr->get_addr ();
    }

  // Handle the timeout case.
  if (timeout != 0 && ACE::handle_timed_accept (this->get_handle (), timeout, restart) == -1)
    return ACE_INVALID_HANDLE;
  else
    {
      // Perform a blocking accept.

      do
	new_handle = ACE_OS::accept (this->get_handle (), addr, len_ptr);
      while (new_handle == ACE_INVALID_HANDLE && restart && errno == EINTR);
      
#if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
      // Reset the event association inherited by the new handle
      ::WSAEventSelect ((SOCKET) new_handle,
                        NULL,
                        0);      
#endif /* ACE_WIN32 */

      // Reset the size of the addr (really only necessary for the
      // UNIX domain sockets).
      if (new_handle != ACE_INVALID_HANDLE && remote_addr != 0)
	remote_addr->set_size (*len_ptr);

      return new_handle;
    }
}
