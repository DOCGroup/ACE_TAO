// $Id$

#include "RMCast_Control_Splitter.h"

#if !defined (__ACE_INLINE__)
# include "RMCast_Control_Splitter.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(ace, RMCast_Control_Splitter, "$Id$")

ACE_RMCast_Control_Splitter::~ACE_RMCast_Control_Splitter (void)
{
}

int
ACE_RMCast_Control_Splitter::ack (ACE_RMCast::Ack &ack)
{
  if (this->control_module () != 0)
    return this->control_module ()->ack (ack);
  return 0;
}

int
ACE_RMCast_Control_Splitter::join (ACE_RMCast::Join &join)
{
  if (this->control_module () != 0)
    return this->control_module ()->join (join);
  return 0;
}

int
ACE_RMCast_Control_Splitter::leave (ACE_RMCast::Leave &leave)
{
  if (this->control_module () != 0)
    return this->control_module ()->leave (leave);
  return 0;
}
