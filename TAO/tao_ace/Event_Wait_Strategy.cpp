#include "Event_Wait_Strategy.h"


#if !defined (__ACE_INLINE__)
#include "Event_Wait_Strategy.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao_ace, Event_Wait_Strategy, "$Id$")


TAO_ACE_Event_Wait_Strategy::~TAO_ACE_Event_Wait_Strategy (void)
{
  ACE_TRACE ("TAO_ACE_Event_Wait_Strategy::Destructor");
}

int
TAO_ACE_Event_Wait_Strategy::wait_for_multiple_events (ACE_Time_Value *)
{
  ACE_TRACE ("TAO_ACE_Event_Wait_Strategy::wait_for_multiple_events");

  return 0;
}
