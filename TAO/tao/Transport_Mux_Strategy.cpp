// $Id$

#include "tao/Transport_Mux_Strategy.h"
#include "tao/Reply_Dispatcher.h"

ACE_RCSID(tao, Transport_Mut_Strategy, "$Id$")

TAO_Transport_Mux_Strategy::TAO_Transport_Mux_Strategy (TAO_Transport *transport)
  : transport_ (transport)
    , lock_ (0)
{
}

TAO_Transport_Mux_Strategy::~TAO_Transport_Mux_Strategy (void)
{
}
