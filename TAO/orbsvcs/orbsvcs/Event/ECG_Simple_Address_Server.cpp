// $Id$

#include "orbsvcs/Event/ECG_Simple_Address_Server.h"

#if !defined(__ACE_INLINE__)
#include "orbsvcs/Event/ECG_Simple_Address_Server.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ECG_Simple_Address_Server::TAO_ECG_Simple_Address_Server (void)
{
}

TAO_ECG_Simple_Address_Server::~TAO_ECG_Simple_Address_Server (void)
{
}

int
TAO_ECG_Simple_Address_Server::init (const char *mcast_addr)
{
  return this->addr_.set (mcast_addr);
}

void
TAO_ECG_Simple_Address_Server::get_addr (
    const RtecEventComm::EventHeader& /*header*/,
    RtecUDPAdmin::UDP_Addr_out addr)
{
  // unsigned long
  addr.ipaddr = this->addr_.get_ip_address ();
  // unsigned short
  addr.port = this->addr_.get_port_number ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
