// $Id$

/* Defines the member functions for the base class of the ACE_ATM_Stream
   abstraction. */

#define ACE_BUILD_DLL
#include "ace/ATM_Stream.h"

ACE_RCSID(ace, ATM_Stream, "$Id$")

#if defined (ACE_HAS_ATM)

ACE_ALLOC_HOOK_DEFINE(ACE_ATM_Stream)

void
ACE_ATM_Stream::dump (void) const
{
  ACE_TRACE ("ACE_ATM_Stream::dump");
}

char*
ACE_ATM_Stream::get_peer_name (void) const
{
  ACE_TRACE ("ACE_ATM_Stream::dump");
#if defined (ACE_HAS_FORE_ATM_XTI)
//   // Use t_getprotaddr for XTI/ATM
//   struct t_bind *localaddr = (struct t_bind *) ACE_OS::t_alloc (get_handle (),
//                                                                 T_BIND,
//                                                                 T_ADDR);
//   struct t_bind *peeraddr = (struct t_bind *) ACE_OS::t_alloc (get_handle (),
//                                                                T_BIND,
//                                                                T_ADDR);
//   ::t_getprotaddr(get_handle (),
//                   localaddr,
//                   peeraddr);

//   char* connected_name = (char*) ACE_OS::malloc(peeraddr->addr.len + 1);
//   ACE_OS::strcpy(connected_name,
//                  peeraddr->addr.buf);
//   ACE_OS::t_free ((char *) localaddr,
//                   T_BIND);
//   ACE_OS::t_free ((char *) peeraddr,
//                   T_BIND);
//   return (connected_name);


  ACE_ATM_Addr sa;
  struct netbuf name;
  name.maxlen = sa.get_size ();
  name.buf = (char *) sa.get_addr ();
  ACE_OS::t_getname (this->get_handle (), &name, REMOTENAME);
  //  ACE_OS::ioctl (this->get_handle (),
  //               TI_GETPEERNAME,
  //               &name);
  return (name.buf);

#elif defined (ACE_HAS_FORE_ATM_WS2)
  // Use getpeername for WinSock2
  struct sockaddr name;
  socklen_t nameSize = sizeof(name);
  if (ACE_OS::getpeername(this->get_handle (),
                          &name,
                          &nameSize) != 0)
    {
      return 0;
    }
  return ??
#else
  return 0;
#endif /* ACE_HAS_FORE_ATM_XTI */
}

#endif /* ACE_HAS_ATM */
