// $Id$

#include "ace/Log_Msg.h"
#include "CB.h"
#include "TimerDispatcher.h"

CB::CB () : count_(0)
{
  ACE_TRACE (ACE_TEXT ("CB::CB"));
}

// Listing 1 code/ch20
int CB::handle_timeout (const ACE_Time_Value &,
                        const void *arg)
{
  ACE_TRACE (ACE_TEXT ("CB::handle_timeout"));

  const int *val = static_cast<const int*> (arg);
  ACE_ASSERT ((*val) == timerID_);

  ACE_UNUSED_ARG (val);

  if (count_ == 5)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Reseting interval for timer %d\n"),
                  timerID_));

      // New interval is 10 ms.
      ACE_Time_Value interval (0L, 1000L);
      int status = Timer::instance ()->reset_interval
                                         (timerID_, interval);
#if defined (ACE_NDEBUG)
      ACE_UNUSED_ARG (status);
#else
      ACE_ASSERT (status != -1);
#endif
    }

  if (count_++ == 10)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Canceling %d\n"),
                  timerID_));
      ACE_ASSERT ((Timer::instance ()->cancel (this)) != 0);
    }

  return 0;
}
// Listing 1

void
CB::setID (long timerID)
{
  ACE_TRACE (ACE_TEXT ("CB::setID"));
  timerID_ = timerID;
}

long
CB::getID (void)
{
  ACE_TRACE (ACE_TEXT ("CB::getID"));
  return timerID_;
}

int
CB::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_TRACE (ACE_TEXT ("CB::handle_close"));
  return 0;
}
