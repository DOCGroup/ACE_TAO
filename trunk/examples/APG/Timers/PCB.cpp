// $Id$

#include "ace/Log_Msg.h"
#include "PCB.h"
#include "PTimerDispatcher.h"

PCB::PCB() : count_(0)
{
  ACE_TRACE (ACE_TEXT ("PCB::PCB"));
}
	
int PCB::handleEvent (const void *arg)
{
  ACE_TRACE (ACE_TEXT ("PCB::handle_timeout"));

  const int *val = ACE_static_cast (const int*, arg);
  ACE_ASSERT ((*val) == timerID_); 

  ACE_UNUSED_ARG (val);

  ACE_DEBUG ((LM_DEBUG, 
              ACE_TEXT("Timer %d expiry handled by thread %t\n"), 
              timerID_));
  if (count_ == 5)
    {
      ACE_DEBUG ((LM_DEBUG, 
                  ACE_TEXT ("Reseting interval for timer %d\n"),
                  timerID_));

      // New interval is 10 ms.
      ACE_Time_Value interval (0L, 1000L); 
      ACE_ASSERT (PTimer::instance ()->reset_interval
                    (timerID_, interval) != -1);

      ACE_UNUSED_ARG (interval);
    }

  if (count_++ == 10)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Canceling %d\n"),
                  timerID_));
      ACE_ASSERT ((PTimer::instance ()->cancel (this)) != 0);
    }

  return 0;
}

void
PCB::setID (long timerID)
{
  ACE_TRACE (ACE_TEXT ("PCB::setID"));
  timerID_ = timerID;
}

long
PCB::getID (void) const
{
  ACE_TRACE (ACE_TEXT ("PCB::getID"));
  return timerID_;
}

int
PCB::handleClose (void)
{
  ACE_TRACE (ACE_TEXT ("PCB::handle_close"));
  return 0; 
}

int 
PCB::handleCancel (void)
{
  ACE_TRACE (ACE_TEXT ("PCB::handleCancel"));
  return 0;
}
