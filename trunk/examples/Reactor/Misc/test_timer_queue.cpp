
// $Id$

#include "ace/Timer_Queue.h"

class Example_Handler : public ACE_Event_Handler
{
public:
  Example_Handler (void)
    : count_ (0) 
  {}

  virtual int handle_timeout (const ACE_Time_Value &, const void *arg)
  {
    ACE_DEBUG ((LM_DEBUG, 
		"yow, the time has come and gone %d times %d, Horatio!\n", 
		this->count_++, int (arg)));
    return 0;
  }

private:
  int count_;
};

int
main (int, char *[])
{
  ACE_Timer_Queue tq;
  Example_Handler eh;

  ACE_ASSERT (tq.is_empty ());
  ACE_ASSERT (ACE_Time_Value::zero == ACE_Time_Value (0));
  int timer_id; 
  
  timer_id = tq.schedule (&eh, (const void *) 1, ACE_OS::gettimeofday ());
  tq.schedule (&eh, (const void *) 2, ACE_OS::gettimeofday ());
  tq.schedule (&eh, (const void *) 3, ACE_OS::gettimeofday ());
  tq.cancel (timer_id);
  ACE_ASSERT (!tq.is_empty ());
  tq.expire (ACE_OS::gettimeofday ());
  tq.schedule (&eh, (const void *) 4, ACE_OS::gettimeofday ());
  tq.schedule (&eh, (const void *) 5, ACE_OS::gettimeofday ());
  tq.cancel (&eh);
  ACE_ASSERT (tq.is_empty ());
  tq.expire (ACE_OS::gettimeofday ());
  return 0;
}

