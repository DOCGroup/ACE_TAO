#include "AddrServer.h"
#include "tao/SystemException.h"

#include <cstring>
#include <memory>

AddrServer::AddrServer (ACE_INET_Addr const &addr)
  : addr_ (addr)
{
}

void
AddrServer::get_addr (RtecEventComm::EventHeader const &,
                      RtecUDPAdmin::UDP_Addr_out addr)
{
#if defined (ACE_HAS_IPV6)
  if (this->addr_.get_type () == PF_INET6)
    throw CORBA::DATA_CONVERSION (0, CORBA::COMPLETED_YES);
#endif /* ACE_HAS_IPV6 */
  addr.ipaddr = this->addr_.get_ip_address ();
  addr.port   = this->addr_.get_port_number ();
}

void
AddrServer::get_address (RtecEventComm::EventHeader const &,
                         RtecUDPAdmin::UDP_Address_out addr)
{
#if defined (ACE_HAS_IPV6)
  if (this->addr_.get_type () == PF_INET6)
    {
      RtecUDPAdmin::UDP_Addr_v6 v6;
      sockaddr_in6 const *in6 =
        reinterpret_cast<sockaddr_in6 *>(this->addr_.get_addr());
      std::memcpy (v6.ipaddr, std::addressof (in6->sin6_addr), sizeof (v6.ipaddr));
      v6.port = this->addr_.get_port_number ();
      addr.v6_addr (v6);
      return;
    }
#endif /* ACE_HAS_IPV6 */
  RtecUDPAdmin::UDP_Addr v4;
  v4.ipaddr = this->addr_.get_ip_address ();
  v4.port = this->addr_.get_port_number ();
  addr.v4_addr (v4);
}
