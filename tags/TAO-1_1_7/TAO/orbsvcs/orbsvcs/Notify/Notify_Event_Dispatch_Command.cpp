// $Id$

#include "Notify_Event_Dispatch_Command.h"
#include "Notify_Listeners.h"
#include "Notify_Event.h"

ACE_RCSID(Notify, TAO_Notify_Event_Dispatch_Command, "$Id$")

TAO_Notify_Event_Dispatch_Command::TAO_Notify_Event_Dispatch_Command (TAO_Notify_Event* event, TAO_Notify_EventListener* event_listener)
  :event_ (event),
   event_listener_ (event_listener)
{
  event_listener_->_incr_refcnt ();
  this->event_->_incr_refcnt ();
}

TAO_Notify_Event_Dispatch_Command::~TAO_Notify_Event_Dispatch_Command ()
{
  ACE_DEBUG ((LM_DEBUG, "in ~TAO_Notify_Event_Dispatch_Command\n"));
  event_listener_->_decr_refcnt ();
  this->event_->_decr_refcnt ();
}

int
TAO_Notify_Event_Dispatch_Command::execute (TAO_Notify_Worker_Task* /*parent_task*/, CORBA::Environment& ACE_TRY_ENV)
{
  this->event_listener_->dispatch_event (*this->event_, ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  return 0;
}
