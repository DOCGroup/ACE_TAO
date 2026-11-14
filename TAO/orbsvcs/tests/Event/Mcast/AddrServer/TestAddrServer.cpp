// TestAddrServer.cpp

#include "TestAddrServer.h"
#include "tao/SystemException.h"
#include "ace/Log_Msg.h"

#include <array>
#include <cstring>

TestAddrServer_i::TestAddrServer_i (ACE_INET_Addr const& addr,
                                    ACE_INET_Addr const& addr6)
  : addr_ (addr),
    addr6_ (addr6)
{
  std::array<ACE_TCHAR, 256> address_buffer {};
  this->addr_.addr_to_string (address_buffer.data (), address_buffer.size ());
  ACE_DEBUG ((LM_DEBUG, "Test Addr (v4) = %s\n", address_buffer.data ()));
  this->addr6_.addr_to_string (address_buffer.data (), address_buffer.size ());
  ACE_DEBUG ((LM_DEBUG, "Test Addr (v6) = %s\n", address_buffer.data ()));
}

void
TestAddrServer_i::get_addr (RtecEventComm::EventHeader const& h,
                            RtecUDPAdmin::UDP_Addr& address)
{
  ACE_INET_Addr& addr = (h.type == 0) ? this->addr_ : this->addr6_;

#if defined (ACE_HAS_IPV6)
  if (addr.get_type () == PF_INET6)
    throw CORBA::DATA_CONVERSION (0, CORBA::COMPLETED_YES);
#endif /* ACE_HAS_IPV6 */
  address.ipaddr = addr.get_ip_address ();
  address.port = addr.get_port_number ();
}

void
TestAddrServer_i::get_address (RtecEventComm::EventHeader const& h,
                               RtecUDPAdmin::UDP_Address_out outaddr)
{
  ACE_INET_Addr& addr = (h.type == 0) ? this->addr_ : this->addr6_;
  std::array<ACE_TCHAR, 256> address_buffer {};
  addr.addr_to_string (address_buffer.data (), address_buffer.size ());
  ACE_DEBUG ((LM_DEBUG, "get_address returning = %s\n", address_buffer.data ()));

#if defined (ACE_HAS_IPV6)
  if (addr.get_type () == PF_INET6)
    {
      RtecUDPAdmin::UDP_Addr_v6 v6 {};
      sockaddr_in6 const* in6 = reinterpret_cast<sockaddr_in6 const*> (addr.get_addr ());
      std::memcpy (v6.ipaddr, &in6->sin6_addr, sizeof v6.ipaddr);
      v6.port = addr.get_port_number ();
      outaddr.v6_addr (v6);
      return;
    }
#endif /* ACE_HAS_IPV6 */
  RtecUDPAdmin::UDP_Addr v4 {};
  v4.ipaddr = addr.get_ip_address ();
  v4.port = addr.get_port_number ();
  outaddr.v4_addr (v4);
}
