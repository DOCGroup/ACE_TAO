// $Id$

#include "ace/Timer_Heap.h"
#include "ace/Timer_List.h"
#include "ace/Timer_Queue.h"

ACE_RCSID(Misc, test_timer_queue, "$Id$")

class Example_Handler : public ACE_Event_Handler
{
public:
  Example_Handler (void)
    : count_ (0)
  {}

  virtual int handle_timeout (const ACE_Time_Value &, const void *arg)
  {
    // Cast the arg to a long, first, because a pointer is the same
    // size as a long on all current ACE platforms.
    int times = (int) (long) arg;

    ACE_DEBUG ((LM_DEBUG,
                "yow, the time has come and gone %d times %d, Horatio!\n",
                this->count_++,
                times));
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
  int timer_id;

  timer_id = tq->schedule (&eh, (const void *) 1, ACE_OS::gettimeofday ());

  // Use timer_id outside of an assert, so that we don't get compile
  // warnings with ACE_NDEBUG about it being unused.
  if (timer_id == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "schedule () failed"));
  ACE_ASSERT (timer_id != -1);

  ACE_ASSERT (tq->schedule (&eh, (const void *) 42,
                           ACE_OS::gettimeofday ()) != -1);
  ACE_ASSERT (tq->schedule (&eh, (const void *) 42,
                           ACE_OS::gettimeofday ()) != -1);
  ACE_ASSERT (tq->cancel (timer_id) == 1);
  ACE_ASSERT (!tq->is_empty ());

  ACE_ASSERT (tq->expire () == 2);

  ACE_ASSERT (tq->schedule (&eh, (const void *) 4, ACE_OS::gettimeofday
                           ()) != -1);
  ACE_ASSERT (tq->schedule (&eh, (const void *) 5, ACE_OS::gettimeofday
                           ()) != -1);
  ACE_ASSERT (tq->cancel (&eh) == 2);
  ACE_ASSERT (tq->is_empty ());
  ACE_ASSERT (tq->expire () == 0);
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
main (int, char *[])
{
  for (int i = 0; timer_queues[i].name_ != 0; i++)
    {
      test_functionality (timer_queues[i].queue_);
      delete timer_queues[i].queue_;
    }

  return 0;
}
