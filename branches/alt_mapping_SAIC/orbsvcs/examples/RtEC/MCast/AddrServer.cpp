// $Id$

#include "AddrServer.h"

AddrServer::AddrServer (const RtecUDPAdmin::UDP_Addr& addr)
  : addr_ (addr)
{
}

void
AddrServer::get_addr (const RtecEventComm::EventHeader&,
                      RtecUDPAdmin::UDP_Addr_out addr)
{
  addr = this->addr_;
}

void
AddrServer::get_address (const RtecEventComm::EventHeader&,
                         RtecUDPAdmin::UDP_Address_out addr)
{
  addr.v4_addr(this->addr_);
}
