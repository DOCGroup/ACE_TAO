//$Id$

#include "Notify_Command.h"
#include "Notify_Event.h"

ACE_INLINE
TAO_Notify_Command::TAO_Notify_Command (TAO_Notify_Event_Processor* event_processor, TAO_Notify_Event* event)
  :event_processor_ (event_processor),
   event_ (event)
{
  // Set the parameters that affect queuing in the message queue.
  if (this->event_)
    this->msg_priority (this->event_->priority ());
  // this->msg_execution_time (this->event_->);
}

ACE_INLINE
TAO_Notify_Command::~TAO_Notify_Command ()
{
}
