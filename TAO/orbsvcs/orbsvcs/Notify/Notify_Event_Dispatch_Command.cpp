// $Id$

#include "Notify_Event_Dispatch_Command.h"
#include "Notify_Listeners.h"
#include "Notify_Event.h"
#include "Notify_Event_Processor.h"

#include "tao/debug.h"

ACE_RCSID(Notify, TAO_Notify_Event_Dispatch_Command, "$Id$")

TAO_Notify_Event_Dispatch_Command::TAO_Notify_Event_Dispatch_Command (TAO_Notify_Event_Processor* event_processor, TAO_Notify_Event* event, TAO_Notify_EventListener* event_listener)
  :TAO_Notify_Command (event_processor, event),
   event_listener_ (event_listener)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "in TAO_Notify_Event_Dispatch_Command %X\n", this));

  event_listener_->_incr_refcnt ();
  this->event_->_incr_refcnt ();
}

TAO_Notify_Event_Dispatch_Command::~TAO_Notify_Event_Dispatch_Command ()
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "in ~TAO_Notify_Event_Dispatch_Command %X\n", this));

  event_listener_->_decr_refcnt ();
  this->event_->_decr_refcnt ();
}

int
TAO_Notify_Event_Dispatch_Command::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  this->event_listener_->dispatch_event (*this->event_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}
