// SOCK_CODgram.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/SOCK_CODgram.h"

ACE_ALLOC_HOOK_DEFINE(ACE_SOCK_CODgram)

void
ACE_SOCK_CODgram::dump (void) const
{
  ACE_TRACE ("ACE_SOCK_CODgram::dump");
}

// Here's the general-purpose constructor.

ACE_SOCK_CODgram::ACE_SOCK_CODgram (const ACE_Addr &remote, const ACE_Addr &local, 
				    int protocol_family, int protocol)
{
  ACE_TRACE ("ACE_SOCK_CODgram::ACE_SOCK_CODgram");
  if (this->open (remote, local, protocol_family, protocol) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_SOCK_CODgram"));
}

/* This is the general-purpose open routine.  Note that it performs
   a different set of functions depending on the LOCAL and REMOTE
   addresses passed to it.  Here's the basic logic:
   
   1. remote == ACE_Addr::sap_any && local == ACE_Addr::sap_any
         if protocol_family == PF_INET then 
	     bind the local address to a randomly generated port number... 

   2. remote == ACE_Addr::sap_any && local != ACE_Addr::sap_any
         we are just binding the local address
	 (used primarily by servers)

   3. remote != ACE_Addr::sap_any && local == ACE_Addr::sap_any
         we are connecting to the remote address
	 (used primarily by clients)

   4. remote != ACE_Addr::sap_any && local != ACE_Addr::sap_any
         we are binding to the local address 
	 and connecting to the remote address 
*/

int
ACE_SOCK_CODgram::open (const ACE_Addr &remote, const ACE_Addr &local,
		    int protocol_family, int protocol)
{
  ACE_TRACE ("ACE_SOCK_CODgram::open");
  if (ACE_SOCK::open (SOCK_DGRAM, protocol_family, protocol) == -1)
    return -1;
  else
    {
      int error = 0;

      if (&local == &ACE_Addr::sap_any && &remote == &ACE_Addr::sap_any)
	{
	  // Assign an arbitrary port number from the transient range!!

	  if (protocol_family == PF_INET 
	      && ACE::bind_port (this->get_handle ()) == -1)
	    error = 1;
	}
      // We are binding just the local address. 
      else if (&local != &ACE_Addr::sap_any && &remote == &ACE_Addr::sap_any)
	{
	  if (ACE_OS::bind (this->get_handle (), (sockaddr *) local.get_addr (), 
		      local.get_size ()) == -1)
	    error = 1;
	}
      // We are connecting to the remote address. 
      else if (&local == &ACE_Addr::sap_any && &remote != &ACE_Addr::sap_any)
	{
	  if (ACE_OS::connect (this->get_handle (), (sockaddr *) remote.get_addr (), 
			 remote.get_size ()) == -1)
	    error = 1;
	}
      // We are binding to the local address and connecting to the
      // remote addresses.
      else
	{
	  if (ACE_OS::bind (this->get_handle (), (sockaddr *) local.get_addr (), 
		      local.get_size ()) == -1
	      || ACE_OS::connect (this->get_handle (), (sockaddr *) remote.get_addr (), 
			    remote.get_size ()) == -1)
	    error = 1;
	}
      if (error)
	{
	  this->close ();
	  this->set_handle (ACE_INVALID_HANDLE);
	}
      return error ? -1 : 0;
    }
}
