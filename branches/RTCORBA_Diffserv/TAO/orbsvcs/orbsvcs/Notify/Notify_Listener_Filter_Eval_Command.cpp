// $Id$

#include "orbsvcs/ESF/ESF_Proxy_Collection.h"
#include "Notify_Listener_Filter_Eval_Command.h"
#include "Notify_Event_Dispatch_Command.h"
#include "Notify_Worker_Task.h"
#include "Notify_Listeners.h"
#include "Notify_Event.h"
#include "Notify_Event_Processor.h"

ACE_RCSID(Notify, Notify_Listener_Filter_Eval_Command, "$Id$")

TAO_Notify_Listener_Filter_Eval_Command::TAO_Notify_Listener_Filter_Eval_Command (TAO_Notify_Event_Processor* event_processor, TAO_Notify_Event* event, TAO_Notify_EventListener* event_listener, CORBA::Boolean eval_parent)
  :TAO_Notify_Command (event_processor, event),
   event_listener_ (event_listener),
   eval_parent_ (eval_parent)
{
  event_listener_->_incr_refcnt ();
  this->event_->_incr_refcnt ();
}

TAO_Notify_Listener_Filter_Eval_Command::~TAO_Notify_Listener_Filter_Eval_Command ()
{
  event_listener_->_decr_refcnt ();
  this->event_->_decr_refcnt ();
}

int
TAO_Notify_Listener_Filter_Eval_Command::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::Boolean result =
    this->event_listener_->evaluate_filter (*this->event_, this->eval_parent_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (result == 1)
    {
      this->event_processor_->dispatch_event (this->event_, this->event_listener_ ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      return 0;
    }

  return -1;
}
