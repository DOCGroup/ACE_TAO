/* $Id$ */

#include "SimpleAddressServer.h"
#include <ace/INET_Addr.h>
#include <ace/OS_NS_string.h>

SimpleAddressServer::SimpleAddressServer (const ACE_INET_Addr& address) {
  this->address_.ipaddr = address.get_ip_address ();
  this->address_.port   = address.get_port_number ();
}

SimpleAddressServer::SimpleAddressServer (const RtecUDPAdmin::UDP_Addr& addr)
  : address_ (addr)
{
}

void
SimpleAddressServer::get_addr (const RtecEventComm::EventHeader&,
                               RtecUDPAdmin::UDP_Addr& address)
    throw (CORBA::SystemException) {
  address = this->address_;
}
/*
void
SimpleAddressServer::get_ip_address (const RtecEventComm::EventHeader&,
                                     RtecUDPAdmin::UDP_IP_Address_out address)
    throw (CORBA::SystemException) {
  address = new RtecUDPAdmin::UDP_IP_Address;

  ACE_INET_Addr x (this->address_.port,
       static_cast<ACE_UINT32>(this->address_.ipaddr));
  address->length (x.get_addr_size ());
  ACE_OS::memcpy (address->get_buffer (),
                  x.get_addr (),
                  x.get_addr_size ());
}
*/
