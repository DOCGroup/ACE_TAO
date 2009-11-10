// $Id$

#include "tao/Transport_Mux_Strategy.h"

ACE_RCSID (tao,
           Transport_Mux_Strategy,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Transport_Mux_Strategy::TAO_Transport_Mux_Strategy (
  TAO_Transport *transport)
  : transport_ (transport)
{
}

TAO_Transport_Mux_Strategy::~TAO_Transport_Mux_Strategy (void)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
