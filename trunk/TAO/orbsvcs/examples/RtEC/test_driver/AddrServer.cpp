// $Id$

#include "AddrServer.h"

ACE_RCSID(EC_Examples, AddrServer, "$Id$")

AddrServer::AddrServer (void)
  : addr_ ()
{
}

AddrServer::AddrServer (const RtecUDPAdmin::UDP_Addr& addr)
  : addr_ (addr)
{
}

void
AddrServer::set_addr (const RtecUDPAdmin::UDP_Addr& addr
                      ACE_ENV_ARG_DECL_NOT_USED)
{
  this->addr_ = addr;
}

void
AddrServer::get_addr (const RtecEventComm::EventHeader&,
                      RtecUDPAdmin::UDP_Addr_out addr
                      ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  addr = this->addr_;
}
