// -*- C++ -*-
// $Id$

#include "Transport_Timer.h"
#include "Transport.h"

ACE_RCSID(tao, Transport_Timer, "$Id$")

TAO_Transport_Timer::TAO_Transport_Timer (TAO_Transport *transport)
  : transport_ (transport)
{
}

int
TAO_Transport_Timer::handle_timeout (const ACE_Time_Value &current_time,
                                     const void *act)
{
  return this->transport_->handle_timeout (current_time, act);
}
