// $Id$
// Reused from: $TAO_ROOT/orbsvcs/examples/RtEC/MCast

#include "AddrServer.h"

ACE_RCSID(EC_MT_Mcast,
          AddrServer,
          "$Id$")

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
