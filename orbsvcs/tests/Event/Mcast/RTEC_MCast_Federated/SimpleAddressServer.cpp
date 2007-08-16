// $Id$

// SimpleAddressServer.cpp

#include "SimpleAddressServer.h"
#include "tao/SystemException.h"

SimpleAddressServer::SimpleAddressServer (const ACE_INET_Addr& address)
{
  this->addr_ = address;
}

void
SimpleAddressServer::get_addr (const RtecEventComm::EventHeader&,
                               RtecUDPAdmin::UDP_Addr& address)
{
#if defined (ACE_HAS_IPV6)
  if (this->addr_.get_type() == PF_INET6)
    throw CORBA::DATA_CONVERSION(0, CORBA::COMPLETED_YES);
#endif /* ACE_HAS_IPV6 */
  address.ipaddr = this->addr_.get_ip_address ();
  address.port   = this->addr_.get_port_number ();
}

void
SimpleAddressServer::get_address (const RtecEventComm::EventHeader& ,
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
