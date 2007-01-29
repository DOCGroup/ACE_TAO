// $Id$

#include "orbsvcs/Event/EC_UDP_Admin.h"

ACE_RCSID (Event,
           EC_UDP_Admin,
           "$Id$")

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
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  addr.ipaddr = header.type;
  addr.port = this->port_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
