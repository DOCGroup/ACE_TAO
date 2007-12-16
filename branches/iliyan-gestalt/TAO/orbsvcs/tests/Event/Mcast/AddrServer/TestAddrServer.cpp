// $Id$

// TestAddrServer.cpp

#include "TestAddrServer.h"
#include "tao/SystemException.h"

TestAddrServer_i::TestAddrServer_i (const ACE_INET_Addr& addr,
                                    const ACE_INET_Addr& addr6)
{
  this->addr_ = addr;
  this->addr6_ = addr6;

  char abuf[256];
  addr_.addr_to_string (abuf,256);

  ACE_DEBUG ((LM_DEBUG,"Test Addr (v4) = %s\n",abuf));

  addr6_.addr_to_string (abuf,256);

  ACE_DEBUG ((LM_DEBUG,"Test Addr (v6) = %s\n",abuf));
}

void
TestAddrServer_i::get_addr (const RtecEventComm::EventHeader& h,
                               RtecUDPAdmin::UDP_Addr& address)
{
  ACE_INET_Addr &addr = (h.type == 0) ? this->addr_ : this->addr6_;

#if defined (ACE_HAS_IPV6)

  if (addr.get_type() == PF_INET6)
    throw CORBA::DATA_CONVERSION(0, CORBA::COMPLETED_YES);
#endif /* ACE_HAS_IPV6 */
  address.ipaddr = addr.get_ip_address ();
  address.port   = addr.get_port_number ();
}

void
TestAddrServer_i::get_address (const RtecEventComm::EventHeader& h,
                               RtecUDPAdmin::UDP_Address_out outaddr)
{
  ACE_INET_Addr &addr = (h.type == 0) ? this->addr_ : this->addr6_;
  char abuf[256];
  addr.addr_to_string (abuf,256);

  ACE_DEBUG ((LM_DEBUG,"get_address returning = %s\n",abuf));

#if defined (ACE_HAS_IPV6)
  if (addr.get_type() == PF_INET6)
    {
      RtecUDPAdmin::UDP_Addr_v6 v6;
      sockaddr_in6 *in6 =
        reinterpret_cast<sockaddr_in6 *>(addr.get_addr());
      ACE_OS::memcpy (v6.ipaddr,&in6->sin6_addr,16);
      v6.port = addr.get_port_number();
      outaddr.v6_addr (v6);
      return;
    }
#endif /* ACE_HAS_IPV6 */
  RtecUDPAdmin::UDP_Addr v4;
  v4.ipaddr = addr.get_ip_address ();
  v4.port = addr.get_port_number ();
  outaddr.v4_addr (v4);
}
