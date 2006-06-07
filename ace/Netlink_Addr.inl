// -*- C++ -*-
// $Id$

#ifdef ACE_HAS_NETLINK

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE ACE_Netlink_Addr::~ACE_Netlink_Addr (void){}

ACE_INLINE ACE_Netlink_Addr::ACE_Netlink_Addr (void):
ACE_Addr (this->determine_type(), sizeof (sockaddr_nl))
{
  this->_nl.nl_family = AF_NETLINK;
}

ACE_INLINE void ACE_Netlink_Addr::set(int pid, int gid)
{
  this->_nl.nl_pid = pid;
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
  return (void*)&(this->_nl);
}

ACE_INLINE int ACE_Netlink_Addr::get_addr_size (void) const
{
  return sizeof(this->_nl);
}


ACE_INLINE void ACE_Netlink_Addr::set_addr (void *addr, int len){
  ACE_OS::memcpy (&this->_nl,addr,len);
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_NETLINK */

