// $Id$

#include "ace/OS_NS_sys_time.h"
#include "ace/Timer_Heap.h"
#include "ace/Timer_List.h"
#include "ace/Timer_Queue.h"
#include "ace/Log_Msg.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/Null_Mutex.h"
#include "ace/Event_Handler.h"



class Example_Handler : public ACE_Event_Handler
{
public:
  Example_Handler (void)
    : count_ (1)
  {}

  virtual int handle_timeout (const ACE_Time_Value &, const void *arg)
  {
    int *times = (int *) arg;

    ACE_DEBUG ((LM_DEBUG,
                "yow, the time has come and gone %d times %d, Horatio!\n",
                this->count_++,
                *times));
    delete times;
    return 0;
  }

private:
  int count_;
};

static void
test_functionality (ACE_Timer_Queue *tq)
{
  Example_Handler eh;

  ACE_ASSERT (tq->is_empty ());
  ACE_ASSERT (ACE_Time_Value::zero == ACE_Time_Value (0));
  const void *timer_act = 0;

  ACE_NEW (timer_act, int (1));
  long timer_id1 = tq->schedule (&eh, timer_act, ACE_OS::gettimeofday ());

  // Use timer_id outside of an assert, so that we don't get compile
  // warnings with ACE_NDEBUG about it being unused.
  if (timer_id1 == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "schedule () failed"));
  ACE_ASSERT (timer_id1 != -1);

  ACE_NEW (timer_act, int (42));
  long result = tq->schedule (&eh, timer_act, ACE_OS::gettimeofday ());
  ACE_ASSERT (result != -1);
  ACE_NEW (timer_act, int (42));
  result = tq->schedule (&eh, timer_act, ACE_OS::gettimeofday ());
  ACE_ASSERT (result != -1);

  result = tq->cancel (timer_id1, &timer_act);
  ACE_ASSERT (result == 1);
  delete (int *) timer_act;
  result = tq->is_empty ();
  ACE_ASSERT (!result);

  result = tq->expire ();
  ACE_ASSERT (result == 2);

  ACE_NEW (timer_act, int (4));
  timer_id1 = tq->schedule (&eh, timer_act, ACE_OS::gettimeofday ());
  ACE_ASSERT (timer_id1 != -1);
  ACE_NEW (timer_act, int (5));
  long timer_id2 = tq->schedule (&eh, timer_act, ACE_OS::gettimeofday ());
  ACE_ASSERT (timer_id2 != -1);

  result = tq->cancel (timer_id1, &timer_act);
  ACE_ASSERT (result == 1);
  delete (int *) timer_act;
  result = tq->cancel (timer_id2, &timer_act);
  ACE_ASSERT (result == 1);
  delete (int *) timer_act;
  result = tq->is_empty ();
  ACE_ASSERT (result == 1);
  result = tq->expire ();
  ACE_ASSERT (result == 0);
}

struct Timer_Queues
{
  ACE_Timer_Queue *queue_;
  // Pointer to the subclass of <ACE_Timer_Queue> that we're testing.

  const char *name_;
  // Name of the Queue that we're testing.
};

static Timer_Queues timer_queues[] =
{
  { new ACE_Timer_List, "ACE_Timer_List" },
  { new ACE_Timer_Heap, "ACE_Timer_Heap" },
  { 0, 0 },
};

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  for (int i = 0; timer_queues[i].name_ != 0; i++)
    {
      test_functionality (timer_queues[i].queue_);
      delete timer_queues[i].queue_;
    }

  return 0;
}
