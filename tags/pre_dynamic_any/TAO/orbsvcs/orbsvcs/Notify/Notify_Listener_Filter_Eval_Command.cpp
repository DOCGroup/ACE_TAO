// $Id$

#include "orbsvcs/ESF/ESF_Proxy_Collection.h"
#include "Notify_Listener_Filter_Eval_Command.h"
#include "Notify_Event_Dispatch_Command.h"
#include "Notify_Worker_Task.h"
#include "Notify_Listeners.h"
#include "Notify_Event.h"

ACE_RCSID(Notify, Notify_Listener_Filter_Eval_Command, "$Id$")

TAO_Notify_Listener_Filter_Eval_Command::TAO_Notify_Listener_Filter_Eval_Command (TAO_Notify_Event* event, TAO_Notify_EventListener* event_listener, CORBA::Boolean eval_parent)
  :event_ (event),
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
TAO_Notify_Listener_Filter_Eval_Command::execute (TAO_Notify_Worker_Task* parent_task,
                                                  CORBA::Environment& ACE_TRY_ENV)
{
  CORBA::Boolean result =
    this->event_listener_->evaluate_filter (*this->event_, this->eval_parent_, ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (result == 1)
    {
      TAO_Notify_Event_Dispatch_Command* dispatch =
        new TAO_Notify_Event_Dispatch_Command (this->event_, this->event_listener_);

      ACE_ASSERT (parent_task->next () != 0);

      TAO_Notify_Worker_Task* next_task =
        ACE_static_cast (TAO_Notify_Worker_Task*, parent_task->next());

      return next_task->process_event (dispatch, ACE_TRY_ENV);
    }

  return -1;
}
