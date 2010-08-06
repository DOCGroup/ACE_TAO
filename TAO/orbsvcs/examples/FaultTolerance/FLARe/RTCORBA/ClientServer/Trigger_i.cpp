#include "Trigger_i.h"
#include "ace/Reactor.h"
#include "tao/ORB_Core.h"

Trigger_i::Trigger_i (CORBA::ORB_ptr orb,
                      Client_Timer_Handler & handler,
                      unsigned long period)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    timer_handler_ (handler),
    started_ (false),
    period_ (period)
{
}

Trigger_i::~Trigger_i ()
{
}

void
Trigger_i::start (const char * /* prefix */)
{
  if (!started_)
    {
      // register the timer handler with the ORB reactor
      ACE_Time_Value period;
      period.msec (static_cast <long> (period_));

      orb_->orb_core ()->reactor ()->schedule_timer (&timer_handler_,
                                                     0,
                                                     ACE_Time_Value::zero,
                                                     period);

      started_ = true;
    }
}
