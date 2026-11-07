#include "AddrServer.h"

AddrServer::AddrServer (RtecUDPAdmin::UDP_Addr const &addr)
  : addr_ (addr)
{
}

void
AddrServer::get_addr (RtecEventComm::EventHeader const &,
                      RtecUDPAdmin::UDP_Addr_out addr)
{
  addr = this->addr_;
}

void
AddrServer::get_address (RtecEventComm::EventHeader const &,
                         RtecUDPAdmin::UDP_Address_out addr)
{
  addr.v4_addr(this->addr_);
}
