// $Id$

#ifndef TIMER_EVENT_HANDLER_H_
#define TIMER_EVENT_HANDLER_H_

#include <ace/Event_Handler.h>
#include <ace/Time_Value.h>
#include "TimeoutConsumer.h"

class Timer_Event_Handler : public ACE_Event_Handler {

 public:

  Timer_Event_Handler (TimeoutConsumer* cons);

  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void* arg);

 private:

  TimeoutConsumer *_consumer;
};

#endif /* TIMER_EVENT_HANDLER_H_ */
