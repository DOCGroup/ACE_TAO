/* -*- C++ -*- $Id$ */

#ifndef MESSENGERSHUTDOWNTIMER_H_
#define MESSENGERSHUTDOWNTIMER_H_

#include "ace/Event_Handler.h"
#include "tao/corba.h"

class MessengerShutdownTimer : public ACE_Event_Handler
{
public:
  // Constructor
  MessengerShutdownTimer (CORBA::ORB_ptr orb);

  virtual int handle_timeout (
    const ACE_Time_Value & current_time,
    const void * act);

private:
  CORBA::ORB_var orb_;
};


#endif /* MESSENGERSHUTDOWNTIMER_H_  */
