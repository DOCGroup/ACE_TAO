// $Id$

#include "orbsvcs/Event/EC_UDP_Admin.h"
#include "ace/INET_Addr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_Simple_AddrServer::TAO_EC_Simple_AddrServer (CORBA::UShort port)
  : port_ (port)
{
}

TAO_EC_Simple_AddrServer::~TAO_EC_Simple_AddrServer (void)
{
}

void
TAO_EC_Simple_AddrServer::get_addr (const RtecEventComm::EventHeader& header,
                                    RtecUDPAdmin::UDP_Addr_out addr)
{
  addr.ipaddr = header.type;
  addr.port = this->port_;
}


void
TAO_EC_Simple_AddrServer::get_address (const RtecEventComm::EventHeader& header,
                                          RtecUDPAdmin::UDP_Address_out addr)
{
  RtecUDPAdmin::UDP_Addr v4;
  v4.ipaddr = header.type;
  v4.port = this->port_;
  addr.v4_addr(v4);
}


TAO_END_VERSIONED_NAMESPACE_DECL
