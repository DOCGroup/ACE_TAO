// LSOCK_CODgram.cpp
// $Id$

/* Contains the definitions for the Local ACE_SOCK connection-oriented
   datagram abstraction. */

#define ACE_BUILD_DLL
#include "ace/LSOCK_CODgram.h"
#include "ace/Log_Msg.h"

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

ACE_ALLOC_HOOK_DEFINE(ACE_LSOCK_CODgram)

void
ACE_LSOCK_CODgram::dump (void) const
{
  ACE_TRACE ("ACE_LSOCK_CODgram::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_SOCK_CODgram::dump ();
  ACE_LSOCK::dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

/* Here's the general-purpose open routine. */

int
ACE_LSOCK_CODgram::open (const ACE_Addr &remote, 
			 const ACE_Addr &local, 
			 int protocol_family, 
			 int protocol)
{
  ACE_TRACE ("ACE_LSOCK_CODgram::open");
  if (ACE_SOCK_CODgram::open (remote, local, protocol_family, 
			      protocol) == -1)
    return -1;
  ACE_LSOCK::set_handle (this->get_handle ());
  return 0;
}

/* Create a local ACE_SOCK datagram. */

ACE_LSOCK_CODgram::ACE_LSOCK_CODgram (const ACE_Addr &remote, 
				      const ACE_Addr &local, 
				      int protocol_family, 
				      int protocol)
{
  ACE_TRACE ("ACE_LSOCK_CODgram::ACE_LSOCK_CODgram");
  if (this->open (remote, local, protocol_family, 
		  protocol) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_LSOCK_CODgram"));
}
#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
