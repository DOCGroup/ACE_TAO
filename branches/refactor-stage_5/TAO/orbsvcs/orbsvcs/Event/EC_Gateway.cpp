// $Id$

#include "orbsvcs/Event/EC_Gateway.h"

ACE_RCSID(Event, EC_Gateway, "$Id$")

TAO_EC_Gateway::TAO_EC_Gateway (void)
  :  handle_ (0)
{
}

TAO_EC_Gateway::~TAO_EC_Gateway (void)
{
}

void
TAO_EC_Gateway::observer_handle (RtecEventChannelAdmin::Observer_Handle h)
{
  this->handle_ = h;
}

RtecEventChannelAdmin::Observer_Handle
TAO_EC_Gateway::observer_handle (void) const
{
  return this->handle_;
}

