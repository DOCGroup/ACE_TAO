// SOCK_Acceptor.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/SOCK_Acceptor.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/SOCK_Acceptor.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#include "ace/Synch.h"

ACE_RCSID(ace, SOCK_Acceptor, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_SOCK_Acceptor)

// Do nothing routine for constructor. 

ACE_SOCK_Acceptor::ACE_SOCK_Acceptor (void)
{
  ACE_TRACE ("ACE_SOCK_Acceptor::ACE_SOCK_Acceptor");
}

// Performs the timed accept operation. 

int
ACE_SOCK_Acceptor::shared_accept_start (ACE_Time_Value *timeout,
					int restart,
					int &in_blocking_mode) const
{
  ACE_TRACE ("ACE_SOCK_Acceptor::shared_accept_start");

  ACE_HANDLE handle = this->get_handle ();

  // Handle the case where we're doing a timed <accept>.
  if (timeout != 0)
    {
      if (ACE::handle_timed_accept (handle,
				    timeout,
				    restart) == -1)
	return -1;
      else
	{
	  in_blocking_mode = ACE_BIT_DISABLED (ACE::get_flags (handle),
					       ACE_NONBLOCK);
	  // Set the handle into non-blocking mode if it's not already
	  // in it.
	  if (in_blocking_mode 
	      && ACE::set_flags (handle,
				 ACE_NONBLOCK) == -1)
	    return -1;
	}
    }

  return 0;
}

int
ACE_SOCK_Acceptor::shared_accept_finish (ACE_SOCK_Stream new_stream,
					 int in_blocking_mode,
					 int reset_new_handle) const
{
  ACE_TRACE ("ACE_SOCK_Acceptor::shared_accept_finish ()");

  ACE_HANDLE new_handle = new_stream.get_handle ();

  // Check to see if we were originally in blocking mode, and if so,
  // set the <new_stream>'s handle and <this> handle to be in blocking
  // mode.
  if (in_blocking_mode)
    {
      // Save/restore errno.
      ACE_Errno_Guard error (errno);
      // Only disable ACE_NONBLOCK if we weren't in
      // non-blocking mode originally.
      ACE::clr_flags (this->get_handle (),
		      ACE_NONBLOCK);
      ACE::clr_flags (new_handle,
		      ACE_NONBLOCK);
    }

#if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
  if (reset_new_handle)
    // Reset the event association inherited by the new handle.
    ::WSAEventSelect ((SOCKET) new_handle, 0, 0);      
#else
  ACE_UNUSED_ARG (reset_new_handle);
#endif /* ACE_WIN32 */

  return new_handle == ACE_INVALID_HANDLE ? -1 : 0;
}

// General purpose routine for accepting new connections. 

int
ACE_SOCK_Acceptor::accept (ACE_SOCK_Stream &new_stream, 
                           ACE_Addr *remote_addr, 
			   ACE_Time_Value *timeout, 
                           int restart,
                           int reset_new_handle) const
{
  ACE_TRACE ("ACE_SOCK_Acceptor::accept");

  int in_blocking_mode = 0;
  if (this->shared_accept_start (timeout,
				 restart,
				 in_blocking_mode) == -1)
    return -1;
  else
    {
      sockaddr *addr = 0;
      int len = 0;

      if (remote_addr != 0)
	{
	  len = remote_addr->get_size ();
	  addr = (sockaddr *) remote_addr->get_addr ();
	}

      do
	new_stream.set_handle (ACE_OS::accept (this->get_handle (),
					       addr,
					       &len));
      while (new_stream.get_handle () == ACE_INVALID_HANDLE 
	     && restart != 0
	     && errno == EINTR
	     && timeout == 0);

      // Reset the size of the addr, which is only necessary for UNIX
      // domain sockets.
      if (new_stream.get_handle () != ACE_INVALID_HANDLE 
	  && remote_addr != 0)
	remote_addr->set_size (len);
    }

  return this->shared_accept_finish (new_stream,
				     in_blocking_mode,
				     reset_new_handle);
}

int
ACE_SOCK_Acceptor::accept (ACE_SOCK_Stream &new_stream, 
			   ACE_Accept_QoS_Params qos_params,
                           ACE_Addr *remote_addr, 
			   ACE_Time_Value *timeout, 
                           int restart,
                           int reset_new_handle) const
{
  ACE_TRACE ("ACE_SOCK_Acceptor::accept");

  int in_blocking_mode = 0;
  if (this->shared_accept_start (timeout,
				 restart,
				 in_blocking_mode) == -1)
    return -1;
  else
    {
      sockaddr *addr = 0;
      int len = 0;

      if (remote_addr != 0)
	{
	  len = remote_addr->get_size ();
	  addr = (sockaddr *) remote_addr->get_addr ();
	}

      do
	new_stream.set_handle (ACE_OS::accept (this->get_handle (),
					       addr,
					       &len,
					       qos_params));
      while (new_stream.get_handle () == ACE_INVALID_HANDLE 
	     && restart != 0
	     && errno == EINTR
	     && timeout == 0);

      // Reset the size of the addr, which is only necessary for UNIX
      // domain sockets.
      if (new_stream.get_handle () != ACE_INVALID_HANDLE 
	  && remote_addr != 0)
	remote_addr->set_size (len);
    }

  return this->shared_accept_finish (new_stream,
				     in_blocking_mode,
				     reset_new_handle);
}

// General purpose routine for performing server ACE_SOCK creation.

ACE_SOCK_Acceptor::ACE_SOCK_Acceptor (const ACE_Addr &local_sap, 
				      int reuse_addr, 
				      int protocol_family,
				      int backlog, 
				      int protocol)
{
  ACE_TRACE ("ACE_SOCK_Acceptor::ACE_SOCK_Acceptor");
  if (this->open (local_sap,
		  reuse_addr,
		  protocol_family, 
		  backlog,
		  protocol) == -1)
    ACE_ERROR ((LM_ERROR,
		ASYS_TEXT ("%p\n"),
		ASYS_TEXT ("ACE_SOCK_Acceptor")));
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
  
  if (ACE_SOCK::open (SOCK_STREAM,
		      protocol_family, 
		      protocol,
		      reuse_addr) == -1)
    error = 1;

  else if (protocol_family == PF_INET)
    {
      sockaddr_in local_inet_addr;
      ACE_OS::memset ((void *) &local_inet_addr,
                      0,
                      sizeof local_inet_addr);

      if (local_sap == ACE_Addr::sap_any)
        {
          local_inet_addr.sin_port = 0;
          local_inet_addr.sin_addr.s_addr = htonl (INADDR_ANY);
        }
      else
        local_inet_addr = *(sockaddr_in *) local_sap.get_addr ();  
  
      if (local_inet_addr.sin_port == 0)
        {
          if (ACE::bind_port (this->get_handle (),
                              local_inet_addr.sin_addr.s_addr) == -1)
            error = 1;
        }
      else
        {
          if (ACE_OS::bind (this->get_handle (), 
                            ACE_reinterpret_cast (sockaddr *,
						  &local_inet_addr), 
                            sizeof local_inet_addr) == -1)
            error = 1;
        }
    }
  else if (ACE_OS::bind (this->get_handle (),
			 (sockaddr *) local_sap.get_addr (), 
                         local_sap.get_size ()) == -1)
    error = 1;

  if (error || ACE_OS::listen (this->get_handle (),
			       backlog) == -1)
    this->close ();

  return error ? -1 : 0;
}
