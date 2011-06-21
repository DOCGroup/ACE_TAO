// $Id$
// Reused from: $TAO_ROOT/orbsvcs/examples/RtEC/MCast

#include "AddrServer.h"
#include "ace/INET_Addr.h"

AddrServer::AddrServer (const ACE_INET_Addr& addr)
{
#if defined (ACE_HAS_IPV6)
  if (addr.get_type() == PF_INET6)
    {
      RtecUDPAdmin::UDP_Addr_v6 v6;
      sockaddr_in6 *in6 =
        reinterpret_cast<sockaddr_in6 *>(addr.get_addr());
      ACE_OS::memcpy (v6.ipaddr,&in6->sin6_addr,16);
      v6.port = addr.get_port_number();
      this->addr_.v6_addr (v6);
      return;
    }
#endif /* ACE_HAS_IPV6 */
  RtecUDPAdmin::UDP_Addr v4;
  v4.ipaddr = addr.get_ip_address ();
  v4.port   = addr.get_port_number ();
  this->addr_.v4_addr (v4);
}

void
AddrServer::get_addr (const RtecEventComm::EventHeader&,
                      RtecUDPAdmin::UDP_Addr& addr)
{
  if (this->addr_._d() == RtecUDPAdmin::Rtec_inet6)
    throw CORBA::DATA_CONVERSION(0, CORBA::COMPLETED_YES);
  addr = this->addr_.v4_addr();
}


void
AddrServer::get_address (const RtecEventComm::EventHeader&,
                         RtecUDPAdmin::UDP_Address_out addr)
{
  addr = this->addr_;
}
