// $Id$

#include "orbsvcs/Event/EC_UDP_Admin.h"

ACE_RCSID(Event, EC_UDP_Admin, "$Id$")

TAO_EC_Simple_AddrServer::TAO_EC_Simple_AddrServer (CORBA::UShort port)
  : port_ (port)
{
}

TAO_EC_Simple_AddrServer::~TAO_EC_Simple_AddrServer (void)
{
}

void
TAO_EC_Simple_AddrServer::get_addr (const RtecEventComm::EventHeader& header,
					  RtecUDPAdmin::UDP_Addr_out addr,
					  CORBA::Environment&)
{
  addr.ipaddr = header.type;
  addr.port = this->port_;
}
