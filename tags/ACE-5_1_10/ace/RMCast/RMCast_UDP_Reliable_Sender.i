// $Id$

ACE_INLINE int
ACE_RMCast_UDP_Reliable_Sender::open (const ACE_INET_Addr &mcast_group)
{
  return this->io_udp_.open (mcast_group, ACE_Addr::sap_any);
}

ACE_INLINE int
ACE_RMCast_UDP_Reliable_Sender::has_data (void)
{
  return this->retransmission_.has_data ();
}

ACE_INLINE int
ACE_RMCast_UDP_Reliable_Sender::has_members (void)
{
  return this->membership_.has_members ();
}
