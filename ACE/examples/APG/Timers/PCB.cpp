// $Id$

#include "ace/Log_Msg.h"
#include "PCB.h"
#include "PTimerDispatcher.h"

PCB::PCB() : count_(0)
{
  ACE_TRACE ("PCB::PCB");
}

PCB::~PCB()
{
}

int PCB::handleEvent (const void *arg)
{
  ACE_TRACE ("PCB::handle_timeout");

  const int *val = static_cast<const int*> (arg);
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
      if (PTimer::instance ()->reset_interval (timerID_, interval) != -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("reset_interval")),
                          -1);
    }

  if (count_++ == 10)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Canceling %d\n"),
                  timerID_));
      PTimer::instance ()->cancel (this);
    }

  return 0;
}

void
PCB::setID (long timerID)
{
  ACE_TRACE ("PCB::setID");
  timerID_ = timerID;
}

long
PCB::getID (void) const
{
  ACE_TRACE ("PCB::getID");
  return timerID_;
}

int
PCB::handleClose (void)
{
  ACE_TRACE ("PCB::handleClose");
  return 0;
}

int
PCB::handleCancel (void)
{
  ACE_TRACE ("PCB::handleCancel");
  return 0;
}
