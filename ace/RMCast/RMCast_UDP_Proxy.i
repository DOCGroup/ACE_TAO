// $Id$

ACE_INLINE const ACE_INET_Addr&
ACE_RMCast_UDP_Proxy::peer_addr (void) const
{
  return this->peer_addr_;
}

ACE_INLINE ACE_RMCast_Module *
ACE_RMCast_UDP_Proxy::module (void) const
{
  return this->module_;
}
