// LSOCK.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/LSOCK.h"

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

ACE_ALLOC_HOOK_DEFINE(ACE_LSOCK)

void
ACE_LSOCK::dump (void) const
{
  ACE_TRACE ("ACE_LSOCK::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "aux_handle_ = %d", this->aux_handle_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

#if defined (ACE_HAS_MSG)
// This routine sends an open file descriptor to <this->handle_>.

int
ACE_LSOCK::send_handle (const ACE_HANDLE fd) const
{
  ACE_TRACE ("ACE_LSOCK::send_handle");
  unsigned char a[2];
  iovec iov;
  msghdr send_msg;

  a[0] = 0xab;
  a[1] = 0xcd; 
  iov.iov_base = (char *) a;
  iov.iov_len = sizeof a;
  send_msg.msg_iov = &iov;
  send_msg.msg_iovlen = 1;
  send_msg.msg_name = 0;
  send_msg.msg_namelen = 0;
  send_msg.msg_accrights = (char *) &fd;
  send_msg.msg_accrightslen = sizeof fd;

  return ACE_OS::sendmsg (this->get_handle (), &send_msg, 0);
}

// This file receives an open file descriptor from THIS->SOK_FD.
// Note, this routine returns -1 if problems occur, 0 if we recv a
// message that does not have file descriptor in it, and 1 otherwise.

int
ACE_LSOCK::recv_handle (ACE_HANDLE &fd, char *pbuf, int *len) const
{
  ACE_TRACE ("ACE_LSOCK::recv_handle");
  unsigned char a[2];
  iovec iov;
  msghdr recv_msg;
   
  if (pbuf != 0 && len != 0)
    {
      iov.iov_base = pbuf;
      iov.iov_len  = *len;
    }
  else
    {
      iov.iov_base = (char *) a;
      iov.iov_len  = sizeof a;
    }
   
  recv_msg.msg_iov = &iov;
  recv_msg.msg_iovlen = 1;
  recv_msg.msg_name = 0;
  recv_msg.msg_namelen = 0;
  recv_msg.msg_accrights = (char *) &fd;
  recv_msg.msg_accrightslen = sizeof fd;
   
#if defined (ACE_HAS_STREAMS)
  ssize_t nbytes = ACE_OS::recvmsg (this->get_handle (), &recv_msg, 0);

  if (nbytes != ACE_INVALID_HANDLE)
    {
      if (len != 0)
	*len = nbytes;

      if (nbytes == sizeof a 
	  && ((unsigned char *) iov.iov_base)[0] == 0xab
	  && ((unsigned char *) iov.iov_base)[1] == 0xcd)
	return 1;
      else
	return 0;
    }
#else
  ssize_t nbytes = ACE_OS::recvmsg (this->get_handle (), &recv_msg, MSG_PEEK);

  if (nbytes != ACE_INVALID_HANDLE)
    {
      if (nbytes == sizeof a 
	  && ((unsigned char *) iov.iov_base)[0] == 0xab
	  && ((unsigned char *) iov.iov_base)[1] == 0xcd)
	{
	  recv_msg.msg_accrights = (char *) &fd;
	  recv_msg.msg_accrightslen = sizeof fd;

	  if (ACE_OS::recvmsg (this->get_handle (), &recv_msg, 0) == ACE_INVALID_HANDLE)
	    return ACE_INVALID_HANDLE;
	  else
	    return 1;
	}
      else
	{
	  if (len != 0)
	    *len = nbytes;
	  return 0;
	}
    }
#endif /* ACE_HAS_STREAMS */
  else
    return ACE_INVALID_HANDLE;
}
#endif /* ACE_HAS_MSG */
#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
