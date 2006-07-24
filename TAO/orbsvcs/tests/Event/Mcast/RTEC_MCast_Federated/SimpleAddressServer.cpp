// $Id$

// SimpleAddressServer.cpp

#include "SimpleAddressServer.h"
#include <ace/INET_Addr.h>

SimpleAddressServer::SimpleAddressServer (const ACE_INET_Addr& address) {
  this->address_.ipaddr = address.get_ip_address ();
  this->address_.port   = address.get_port_number ();
}

void
SimpleAddressServer::get_addr (const RtecEventComm::EventHeader&,
             RtecUDPAdmin::UDP_Addr& address)
    throw (CORBA::SystemException) {
  address = this->address_;
}
