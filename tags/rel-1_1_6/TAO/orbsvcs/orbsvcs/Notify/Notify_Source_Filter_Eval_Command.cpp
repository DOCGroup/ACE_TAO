// $Id$

#include "Notify_Source_Filter_Eval_Command.h"
#include "Notify_Listeners.h"
#include "Notify_Lookup_Command.h"
#include "Notify_Worker_Task.h"
#include "Notify_Event.h"

ACE_RCSID(Notify, Notify_Source_Filter_Eval_Command, "$Id$")

TAO_Notify_Source_Filter_Eval_Command::TAO_Notify_Source_Filter_Eval_Command (TAO_Notify_Event* event, TAO_Notify_EventSource* event_source)
  :event_ (event),
   event_source_ (event_source)
{
  this->event_source_->_incr_refcnt ();
  this->event_->_incr_refcnt ();
}

TAO_Notify_Source_Filter_Eval_Command::~TAO_Notify_Source_Filter_Eval_Command ()
{
  this->event_source_->_decr_refcnt ();
  this->event_->_decr_refcnt ();
}

int
TAO_Notify_Source_Filter_Eval_Command::execute (TAO_Notify_Worker_Task* parent_task,
                                                CORBA::Environment& ACE_TRY_ENV)
{
  CORBA::Boolean result =
    this->event_source_->evaluate_filter (*this->event_, ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (result == 1)
    {
      TAO_Notify_Lookup_Command* lookup =
        new TAO_Notify_Lookup_Command (this->event_);

      ACE_ASSERT (parent_task->next () != 0);

      TAO_Notify_Worker_Task* next_task =
        ACE_static_cast (TAO_Notify_Worker_Task*, parent_task->next());

      return next_task->process_event (lookup, ACE_TRY_ENV);
    }

  return -1;
}
