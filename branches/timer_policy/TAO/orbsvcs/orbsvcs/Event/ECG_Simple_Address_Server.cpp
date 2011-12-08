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
#if defined (ACE_HAS_IPV6)
  if (this->addr_.get_type() == PF_INET6)
    throw CORBA::DATA_CONVERSION(0, CORBA::COMPLETED_YES);
#endif /* ACE_HAS_IPV6 */
  // unsigned long
  addr.ipaddr = this->addr_.get_ip_address ();
  // unsigned short
  addr.port = this->addr_.get_port_number ();
}


void
TAO_ECG_Simple_Address_Server::get_address (const RtecEventComm::EventHeader& ,
                                            RtecUDPAdmin::UDP_Address_out addr)
{
#if defined (ACE_HAS_IPV6)
  if (this->addr_.get_type() == PF_INET6)
    {
      RtecUDPAdmin::UDP_Addr_v6 v6;
      sockaddr_in6 *in6 =
        reinterpret_cast<sockaddr_in6 *>(this->addr_.get_addr());
      ACE_OS::memcpy (v6.ipaddr,&in6->sin6_addr,16);
      v6.port = this->addr_.get_port_number();
      addr.v6_addr (v6);
      return;
    }
#endif /* ACE_HAS_IPV6 */
  RtecUDPAdmin::UDP_Addr v4;
  v4.ipaddr = this->addr_.get_ip_address();
  v4.port = this->addr_.get_port_number();
  addr.v4_addr (v4);
}


TAO_END_VERSIONED_NAMESPACE_DECL
