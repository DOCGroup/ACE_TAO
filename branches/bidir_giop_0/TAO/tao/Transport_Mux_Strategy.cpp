// $Id$

#include "tao/Transport_Mux_Strategy.h"
#include "tao/Reply_Dispatcher.h"

ACE_RCSID(tao, Transport_Mut_Strategy, "$Id$")

TAO_Transport_Mux_Strategy::TAO_Transport_Mux_Strategy (TAO_Transport *transport)
  : transport_ (transport)
{
}

TAO_Transport_Mux_Strategy::~TAO_Transport_Mux_Strategy (void)
{
}


int
TAO_Transport_Mux_Strategy::bind_dispatcher (CORBA::ULong,
                                             TAO_Reply_Dispatcher *rd)
{
  rd->dispatcher_bound (this->transport_);
  return 0;
}

void
TAO_Transport_Mux_Strategy::unbind_dispatcher (CORBA::ULong)
{
}
