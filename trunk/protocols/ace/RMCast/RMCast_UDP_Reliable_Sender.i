// $Id$

ACE_INLINE int
ACE_RMCast_UDP_Reliable_Sender::open (const ACE_INET_Addr &mcast_group)
{
  return this->io_udp_.open (mcast_group, ACE_Addr::sap_any);
}
