// -*- C++ -*-
// $Id$

#include "tao/Transport_Timer.h"
#include "tao/Transport.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

TAO_END_VERSIONED_NAMESPACE_DECL
