// $Id$

#include "ECG_Simple_Address_Server.h"

#if !defined(__ACE_INLINE__)
#include "ECG_Simple_Address_Server.i"
#endif /* __ACE_INLINE__ */

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
TAO_ECG_Simple_Address_Server::get_addr (const RtecEventComm::EventHeader& /*header*/,
                                         RtecUDPAdmin::UDP_Addr_out addr
                                         ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // unsigned long
  addr.ipaddr = this->addr_.get_ip_address ();
  // unsigned short
  addr.port = this->addr_.get_port_number ();
}
