// $Id$

// -*- C++ -*-
//
// Netlink_Addr.inl,v 4.1 2004/10/10  jwillemsen Exp

#include /* */   "ace/OS_NS_string.h"
#include /* */   "ace/Global_Macros.h"
#include  /* */  "ace/OS_NS_unistd.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
     # pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_INLINE ACE_Netlink_Addr::~ACE_Netlink_Addr (void){}


ACE_INLINE ACE_Netlink_Addr::ACE_Netlink_Addr (void):
                    ACE_Addr (this->determine_type(), sizeof (sockaddr_nl))
{
  //(void) ACE_OS::memset (&this->_nl,0x00,sizeof (sockaddr_nl));
  this->_nl.nl_family = AF_NETLINK;
}


ACE_INLINE void ACE_Netlink_Addr::set(int pid,int gid)
{
        this->_nl.nl_pid=pid;
        this->_nl.nl_groups = gid;
} 

ACE_INLINE void ACE_Netlink_Addr::reset (void)
{
  ACE_OS::memset (&this->_nl, 0, sizeof (this->_nl));
  this->_nl.nl_family = AF_NETLINK;
}

ACE_INLINE int ACE_Netlink_Addr::determine_type (void) const
{
  return AF_NETLINK;
}

ACE_INLINE void *ACE_Netlink_Addr::get_addr (void) const
{
  ACE_TRACE ("ACE_INET_Addr::get_addr");
  return (void*)&(this->_nl);
}

ACE_INLINE int ACE_Netlink_Addr::get_addr_size (void) const
{
  ACE_TRACE ("ACE_INET_Addr::get_addr_size");
  return sizeof(this->_nl);
}


ACE_INLINE void ACE_Netlink_Addr::set_addr (void *addr, int len){
   ACE_OS::memcpy (&this->_nl,addr,len);
}

