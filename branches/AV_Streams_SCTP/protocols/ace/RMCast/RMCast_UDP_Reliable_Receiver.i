// $Id$

ACE_INLINE int
ACE_RMCast_UDP_Reliable_Receiver::init (const ACE_INET_Addr &mcast_group)
{
  return this->io_udp_.subscribe (mcast_group);
}
