// $Id$

#include "Method_Request_Event.h"

#if ! defined (__ACE_INLINE__)
#include "Method_Request_Event.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_Method_Request_Event, "$id$")

TAO_Notify_Method_Request_Event::TAO_Notify_Method_Request_Event (const TAO_Notify_Event_var& event)
  :event_ (event)
{
  this->init (event);
}

TAO_Notify_Method_Request_Event::~TAO_Notify_Method_Request_Event ()
{
}
int
TAO_Notify_Method_Request_Event::execute (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return -1;
}

const TAO_Notify_Event_var&
TAO_Notify_Method_Request_Event::event (void)
{
  return this->event_;
}
