// SOCK.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/SOCK.h"

ACE_ALLOC_HOOK_DEFINE(ACE_SOCK)

#if defined (ACE_WIN32)
// Static used to initialize WinSock.
ACE_SOCK ACE_SOCK::dummy_;
#endif /* ACE_WIN32 */

void
ACE_SOCK::dump (void) const
{
  ACE_TRACE ("ACE_SOCK::dump");
}

ACE_SOCK::ACE_SOCK (void)
{
  // ACE_TRACE ("ACE_SOCK::ACE_SOCK");

  // Make sure that we've initialized the WinSock library before using
  // sockets!
  ACE_OS::socket_init (ACE_WSOCK_VERSION);
}

// Returns information about the remote peer endpoint (if there is
// one).

int
ACE_SOCK::get_remote_addr (ACE_Addr &sa) const
{
  ACE_TRACE ("ACE_SOCK::get_remote_addr");
  int len = sa.get_size ();
  sockaddr *addr = (sockaddr *) sa.get_addr ();

  if (ACE_OS::getpeername (this->get_handle (), addr, &len) == -1)
    return -1;
  
  sa.set_size (len);
  return 0;
}

int
ACE_SOCK::get_local_addr (ACE_Addr &sa) const
{
  ACE_TRACE ("ACE_SOCK::get_local_addr");
  int len = sa.get_size ();
  sockaddr *addr = (sockaddr *) sa.get_addr ();

  if (ACE_OS::getsockname (this->get_handle (), addr, &len) == -1)
    return -1;

  sa.set_size (len);
  return 0;
}

int
ACE_SOCK::open (int type, 
		int protocol_family, 
		int protocol,
		int reuse_addr)
{
  ACE_TRACE ("ACE_SOCK::open");
  int one = 1;

  this->set_handle (ACE_OS::socket (protocol_family, type, protocol));

  if (this->get_handle () == ACE_INVALID_HANDLE)
    return -1;
  else if (reuse_addr && this->set_option (SOL_SOCKET, SO_REUSEADDR,
					   &one, sizeof one)) 
    {
      this->close ();
      return -1;
    }
  return 0;
}

// Close down a ACE_SOCK.

int
ACE_SOCK::close (void)
{
  ACE_TRACE ("ACE_SOCK::close");
  int result = 0;

  if (this->get_handle () != ACE_INVALID_HANDLE)
    {
      result = ACE_OS::closesocket (this->get_handle ());
      this->set_handle (ACE_INVALID_HANDLE);
    }
  return result;
}

// General purpose constructor for performing server ACE_SOCK
// creation.

ACE_SOCK::ACE_SOCK (int type, 
		    int protocol_family, 
		    int protocol,
		    int reuse_addr)
{
  ACE_TRACE ("ACE_SOCK::ACE_SOCK");
  if (this->open (type, protocol_family, 
		  protocol, reuse_addr) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_SOCK::ACE_SOCK"));
}
