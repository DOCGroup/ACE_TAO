// $Id$

#include "Method_Request_Event.h"

#if ! defined (__ACE_INLINE__)
#include "Method_Request_Event.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (Notify, TAO_Notify_Method_Request_Event, "$Id$")

TAO_Notify_Method_Request_Event::TAO_Notify_Method_Request_Event (
      const TAO_Notify_Method_Request_Event_Base & prev_request,
      const TAO_Notify_Event_var & event_var)
  : TAO_Notify_Method_Request (event_var.get ())
  , TAO_Notify_Method_Request_Event_Base (prev_request, event_var.get ())
  , event_var_ (event_var)
{
}

TAO_Notify_Method_Request_Event::~TAO_Notify_Method_Request_Event ()
{
}
int
TAO_Notify_Method_Request_Event::execute (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return -1;
}

