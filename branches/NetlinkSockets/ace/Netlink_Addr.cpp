// $Id$

//=============================================================================
/**
 *  @file    Netlink_addr.cpp
 *
 *  SOCK_Netlink.cpp,v 5.4.2 2004/10/10 
 * 
 *  @author Raz Ben Yehuda <raziebe@013.net.il>
 */
//=============================================================================

#include /* */   "ace/OS.h"

#if defined (ACE_HAS_LINUX_NETLINK)

#include  /* */  "ace/Netlink_Addr.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
     # pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (__ACE_INLINE__)
#include "ace/Netlink_Addr.inl"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Netlink_Addr)



int ACE_Netlink_Addr::set (const ACE_Netlink_Addr &sa)
{ 
  ACE_OS::memset ((void *) &this->_nl,0,sizeof this->_nl);
  this->_nl.nl_family = AF_NETLINK;
  this->base_set (sa.get_type (), sa.get_size ());
  return 0;
}

// Copy constructor.

ACE_Netlink_Addr::ACE_Netlink_Addr (const ACE_Netlink_Addr &sa)
  : ACE_Addr (AF_NETLINK, sa.get_size ())
{
  this->set (sa);
}

int ACE_Netlink_Addr::set (const sockaddr_nl *un, int len)
{
  (void) ACE_OS::memcpy ((void *) &this->_nl,un,len);
  return 0;
}

ACE_Netlink_Addr::ACE_Netlink_Addr (const sockaddr_nl *un, int len)
{
  this->set (un, len);
}

int
ACE_Netlink_Addr::get_pid (void) const
{
    return this->_nl.nl_pid;
}

int
ACE_Netlink_Addr::get_gid (void) const
{
    return this->_nl.nl_groups;
}

#endif   //if defined (ACE_HAS_LINUX_NETLINK)


