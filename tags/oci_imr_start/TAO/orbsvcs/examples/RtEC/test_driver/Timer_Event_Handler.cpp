// $Id$

#include "Timer_Event_Handler.h"
#include "TimeoutConsumer.h"
#include "orbsvcs/Event_Service_Constants.h"

Timer_Event_Handler::Timer_Event_Handler (TimeoutConsumer* cons)
  : _consumer(cons)
{
}

int
Timer_Event_Handler::handle_timeout (const ACE_Time_Value &tv,
                                     const void* arg)
{
  ACE_UNUSED_ARG(tv);
  ACE_UNUSED_ARG(arg);

  RtecEventComm::EventSet event(1);
  event.length(1);

  event[0].header.type   = ACE_ES_EVENT_INTERVAL_TIMEOUT;
  event[0].header.source = ACE_ES_EVENT_UNDEFINED; //for now, fake the supplier ID
  event[0].header.ttl    = 1;

  //need to create ACE_ENV, since one isn't passed into this function
  ACE_DECLARE_NEW_CORBA_ENV;
  this->_consumer->push(event ACE_ENV_ARG_PARAMETER);

  return 0;
}
