// LSOCK_Stream.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/LSOCK_Stream.h"

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

ACE_ALLOC_HOOK_DEFINE(ACE_LSOCK_Stream)

int
ACE_LSOCK_Stream::get_remote_addr (ACE_Addr &a) const
{
  ACE_TRACE ("ACE_LSOCK_Stream::get_remote_addr");
  return this->get_local_addr (a);
}

void
ACE_LSOCK_Stream::dump (void) const
{
  ACE_TRACE ("ACE_LSOCK_Stream::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_SOCK_Stream::dump ();
  ACE_LSOCK::dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

#if defined (ACE_HAS_MSG)

// Send a readv-style vector of buffers, along with an open I/O
// handle.

ssize_t
ACE_LSOCK_Stream::send_msg (const iovec iov[], 
			    size_t n, 
			    int fd)
{
  ACE_TRACE ("ACE_LSOCK_Stream::send_msg");
  msghdr send_msg;

  send_msg.msg_iov = (iovec *) iov;
  send_msg.msg_iovlen = n;
  send_msg.msg_name = 0;
  send_msg.msg_namelen = 0;
  send_msg.msg_accrights = (char *) &fd;
  send_msg.msg_accrightslen = sizeof fd;

  return ACE_OS::sendmsg (this->ACE_SOCK_Stream::get_handle (), 
		    &send_msg, 0);
}

// Read a readv-style vector of buffers, along with an open I/O
// handle.

ssize_t
ACE_LSOCK_Stream::recv_msg (iovec iov[], 
			    size_t n, 
			    int &fd)
{
  ACE_TRACE ("ACE_LSOCK_Stream::recv_msg");
  msghdr recv_msg;
  
  recv_msg.msg_iov = (iovec *) iov;
  recv_msg.msg_iovlen = n;
  recv_msg.msg_name = 0;
  recv_msg.msg_namelen = 0;
  recv_msg.msg_accrights = (char *) &fd;
  recv_msg.msg_accrightslen = sizeof fd;

  return ACE_OS::recvmsg (this->ACE_SOCK_Stream::get_handle (), 
		    &recv_msg, 0);
}
#endif /* ACE_HAS_MSG */
#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
