// SOCK_Dgram.cpp
// $Id$


#define ACE_BUILD_DLL
#include "ace/SOCK_Dgram.h"
#include "ace/Log_Msg.h"

ACE_ALLOC_HOOK_DEFINE(ACE_SOCK_Dgram)

void
ACE_SOCK_Dgram::dump (void) const
{
  ACE_TRACE ("ACE_SOCK_Dgram::dump");
}

// Here's the shared open function.  Note that if we are using the
// PF_INET protocol family and the address of LOCAL == the address of
// the special variable SAP_ANY then we are going to arbitrarily bind
// to a portnumber.

int
ACE_SOCK_Dgram::shared_open (const ACE_Addr &local, int protocol_family)
{
  ACE_TRACE ("ACE_SOCK_Dgram::shared_open");
  int error = 0;

  if (&local == &ACE_Addr::sap_any && protocol_family == PF_INET)
    {
      if (ACE::bind_port (this->get_handle ()) == -1)
	error = 1;
    }
  else if (ACE_OS::bind (this->get_handle (), (sockaddr *) local.get_addr (), 
			 local.get_size ()) == -1)
    error = 1;

  if (error)
    this->close ();

  return error ? -1 : 0;
}

// Here's the general-purpose constructor used by a connectionless
// datagram ``server''...

ACE_SOCK_Dgram::ACE_SOCK_Dgram (const ACE_Addr &local, 
				int protocol_family, 
				int protocol)
  : ACE_SOCK (SOCK_DGRAM, protocol_family, protocol)
{
  ACE_TRACE ("ACE_SOCK_Dgram::ACE_SOCK_Dgram");
  if (this->shared_open (local, protocol_family) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_SOCK_Dgram"));
}

// Here's the general-purpose open routine.

int
ACE_SOCK_Dgram::open (const ACE_Addr &local, 
		      int protocol_family, 
		      int protocol)
{
  ACE_TRACE ("ACE_SOCK_Dgram::open");
  if (ACE_SOCK::open (SOCK_DGRAM, protocol_family, 
		      protocol) == -1)
    return -1;
  else
    return this->shared_open (local, protocol_family);
}

#if defined (ACE_HAS_MSG)
// Send an iovec of size N to ADDR as a datagram (connectionless
// version).

ssize_t
ACE_SOCK_Dgram::send (const iovec iov[], 
		      size_t n, 
		      const ACE_Addr &addr, 
		      int flags) const
{
  ACE_TRACE ("ACE_SOCK_Dgram::send");
  msghdr send_msg;  

  send_msg.msg_iov          = (iovec *) iov;
  send_msg.msg_iovlen       = n;
  send_msg.msg_name	    = (char *) addr.get_addr ();
  send_msg.msg_namelen      = addr.get_size ();
  send_msg.msg_accrights    = 0;
  send_msg.msg_accrightslen = 0;
  return ACE_OS::sendmsg (this->get_handle (), &send_msg, flags);
}

// Recv an iovec of size N to ADDR as a datagram (connectionless
// version).

ssize_t
ACE_SOCK_Dgram::recv (iovec iov[], 
		      size_t n, 
		      ACE_Addr &addr, 
		      int flags) const
{
  ACE_TRACE ("ACE_SOCK_Dgram::recv");
  msghdr recv_msg;  

  recv_msg.msg_iov = (iovec *) iov;
  recv_msg.msg_iovlen = n;
  recv_msg.msg_name = (char *) addr.get_addr ();
  recv_msg.msg_namelen = addr.get_size ();
  recv_msg.msg_accrights = 0;
  recv_msg.msg_accrightslen = 0;

  ssize_t status = ACE_OS::recvmsg (this->get_handle (), 
				    &recv_msg, flags);
  addr.set_size (recv_msg.msg_namelen);
  return status;
}

#endif /* ACE_HAS_MSG */
