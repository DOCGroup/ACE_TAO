// $Id$

// SimpleAddressServer.cpp

#include "SimpleAddressServer.h"
#include "ace/INET_Addr.h"
#include "ace/OS_NS_string.h"

SimpleAddressServer::SimpleAddressServer (const ACE_INET_Addr& address) {
#if defined (ACE_HAS_IPV6)
  if (address.get_type() == PF_INET6)
    {
      RtecUDPAdmin::UDP_Addr_v6 v6;
      sockaddr_in6 *in6 =
        reinterpret_cast<sockaddr_in6 *>(address.get_addr());
      ACE_OS::memcpy (v6.ipaddr,&in6->sin6_addr,16);
      v6.port = address.get_port_number();
      this->address_.v6_addr (v6);
      return;
    }
#endif /* ACE_HAS_IPV6 */
  RtecUDPAdmin::UDP_Addr v4;
  v4.ipaddr = address.get_ip_address ();
  v4.port   = address.get_port_number ();
  this->address_.v4_addr (v4);
}

void
SimpleAddressServer::get_addr (const RtecEventComm::EventHeader&,
                               RtecUDPAdmin::UDP_Addr& address)
{
  if (this->address_._d() == RtecUDPAdmin::Rtec_inet6)
    throw CORBA::DATA_CONVERSION(0, CORBA::COMPLETED_YES);
  address = this->address_.v4_addr();
}

void
SimpleAddressServer::get_address(const RtecEventComm::EventHeader&,
                                 RtecUDPAdmin::UDP_Address& address)
{
  address = this->address_;
}
