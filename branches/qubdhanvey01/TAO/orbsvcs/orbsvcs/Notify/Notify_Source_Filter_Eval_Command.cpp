// $Id$

#include "Notify_Source_Filter_Eval_Command.h"
#include "Notify_Listeners.h"
#include "Notify_Lookup_Command.h"
#include "Notify_Worker_Task.h"
#include "Notify_Event.h"
#include "Notify_Event_Processor.h"

ACE_RCSID(Notify, Notify_Source_Filter_Eval_Command, "$Id$")

TAO_Notify_Source_Filter_Eval_Command::TAO_Notify_Source_Filter_Eval_Command (TAO_Notify_Event_Processor* event_processor, TAO_Notify_Event* event, TAO_Notify_EventSource* event_source)
  :TAO_Notify_Command (event_processor, event),
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
TAO_Notify_Source_Filter_Eval_Command::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::Boolean result =
    this->event_source_->evaluate_filter (*this->event_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (result == 1)
    {
      this->event_processor_->
        lookup_subscriptions (this->event_, this->event_source_ ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      return 0;
    }
  else
    return -1;
}
