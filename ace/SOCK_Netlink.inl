// $Id$
// SOCK_Netlink.inl,v 5.4.2 2004/10/10  raz

#include "ace/OS_NS_sys_socket.h"
#include "ace/SOCK_Netlink.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
     # pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_INLINE
ACE_SOCK_Netlink::ACE_SOCK_Netlink (void)
{
  ACE_TRACE ("ACE_SOCK_Netlink::ACE_SOCK_Netlink");
}

ACE_INLINE 
ACE_SOCK_Netlink::~ACE_SOCK_Netlink (void)
{
  ACE_TRACE ("ACE_SOCK_Netlink::~ACE_SOCK_Netlink");
}
// <recvfrom> an n byte datagram (connectionless version).
ACE_INLINE ssize_t ACE_SOCK_Netlink::recv ( void *buf,
                                            size_t n,
                                            int flags) const
{
    ACE_TRACE ("ACE_SOCK_Netlink::recv");
    return  ACE_OS::recv (this->get_handle (),(char *) buf,n,flags);
}

ACE_INLINE ssize_t ACE_SOCK_Netlink::send (void *buf,
                          size_t n,
                          int flags) const
{
     ACE_TRACE ("ACE_SOCK_Netlink::recv");
     return  ACE_OS::send (this->get_handle (),(char *) buf,n,flags);
}
