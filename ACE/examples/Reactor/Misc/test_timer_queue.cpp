#include "ace/OS_NS_sys_time.h"
#include "ace/Timer_Heap.h"
#include "ace/Timer_List.h"
#include "ace/Timer_Queue.h"
#include "ace/Log_Msg.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/Null_Mutex.h"
#include "ace/Event_Handler.h"

#include <array>
#include <memory>

class Example_Handler : public ACE_Event_Handler
{
public:
  Example_Handler () = default;
  Example_Handler (Example_Handler const &) = delete;
  Example_Handler (Example_Handler &&) = delete;
  Example_Handler &operator= (Example_Handler const &) = delete;
  Example_Handler &operator= (Example_Handler &&) = delete;

  int handle_timeout (ACE_Time_Value const &, void const *arg) override
  {
    int const *times = static_cast<int const *> (arg);

    ACE_DEBUG ((LM_DEBUG,
                "yow, the time has come and gone %d times %d, Horatio!\n",
                this->count_++,
                *times));
    return 0;
  }

private:
  int count_ {1};
};

static void
test_functionality (ACE_Timer_Queue * const tq)
{
  Example_Handler eh;

  ACE_TEST_ASSERT (tq->is_empty ());
  ACE_TEST_ASSERT (ACE_Time_Value::zero == ACE_Time_Value (0));
  int const times1 {1};
  int const times2 {42};
  int const times3 {42};
  int const times4 {4};
  int const times5 {5};
  void const *timer_act {};

  long const timer_id1 = tq->schedule (&eh, &times1, ACE_OS::gettimeofday ());

  // Use timer_id outside of an assert, so that we don't get compile
  // warnings with ACE_NDEBUG about it being unused.
  if (timer_id1 == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "schedule () failed"));
  ACE_TEST_ASSERT (timer_id1 != -1);

  long result = tq->schedule (&eh, &times2, ACE_OS::gettimeofday ());
  ACE_TEST_ASSERT (result != -1);
  result = tq->schedule (&eh, &times3, ACE_OS::gettimeofday ());
  ACE_TEST_ASSERT (result != -1);

  result = tq->cancel (timer_id1, &timer_act);
  ACE_TEST_ASSERT (result == 1);
  ACE_TEST_ASSERT (*static_cast<int const *> (timer_act) == times1);
  result = tq->is_empty ();
  ACE_TEST_ASSERT (!result);

  result = tq->expire ();
  ACE_TEST_ASSERT (result == 2);

  long const timer_id3 = tq->schedule (&eh, &times4, ACE_OS::gettimeofday ());
  ACE_TEST_ASSERT (timer_id3 != -1);
  long const timer_id4 = tq->schedule (&eh, &times5, ACE_OS::gettimeofday ());
  ACE_TEST_ASSERT (timer_id4 != -1);

  result = tq->cancel (timer_id3, &timer_act);
  ACE_TEST_ASSERT (result == 1);
  ACE_TEST_ASSERT (*static_cast<int const *> (timer_act) == times4);
  result = tq->cancel (timer_id4, &timer_act);
  ACE_TEST_ASSERT (result == 1);
  ACE_TEST_ASSERT (*static_cast<int const *> (timer_act) == times5);
  result = tq->is_empty ();
  ACE_TEST_ASSERT (result == 1);
  result = tq->expire ();
  ACE_TEST_ASSERT (result == 0);
}

struct Timer_Queues
{
  std::unique_ptr<ACE_Timer_Queue> queue;
};

static std::array<Timer_Queues, 2> const timer_queues = {{
  {std::make_unique<ACE_Timer_List> ()},
  {std::make_unique<ACE_Timer_Heap> ()}
}};

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  for (Timer_Queues const &timer_queue : timer_queues)
    {
      test_functionality (timer_queue.queue.get ());
    }

  return 0;
}
