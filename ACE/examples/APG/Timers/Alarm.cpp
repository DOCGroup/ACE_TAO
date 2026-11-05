#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_time.h"

// Listing 1 code/ch20
#include "ace/Timer_Queue_Adapters.h"
#include "ace/Timer_Heap.h"

using Timer = ACE_Async_Timer_Queue_Adapter<ACE_Timer_Heap>;
// Listing 1

class CB : public ACE_Event_Handler
{
public:
  explicit CB (int const id) : id_ (id) {}
  CB (CB const &) = delete;
  CB (CB &&) = delete;
  CB &operator= (CB const &) = delete;
  CB &operator= (CB &&) = delete;

  int handle_timeout (ACE_Time_Value const &,
                      void const *arg) override
  {
    ACE_TRACE ("CB::handle_timeout");

    int const *val = static_cast<int const *> (arg);
    ACE_ASSERT ((*val) == id_);

    ACE_UNUSED_ARG (val);

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Timer expired\n")));
    return 0;
  }

private:
  int id_ {};
};

// Listing 2 code/ch20
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  // Create the timer such that it blocks all signals
  // when it goes off.
  Timer timer;

  // Schedule a timer to go off 2 seconds later and then
  // after every 4 seconds.
  CB cb (1);
  int const arg {1};
  ACE_Time_Value initial (2);
  ACE_Time_Value repeat (4);
  initial += ACE_OS::gettimeofday ();
  timer.schedule (&cb, &arg, initial, repeat);

  while (true)   // Don't let the main thread exit.
    ACE_OS::sleep (2);
  ACE_NOTREACHED (return 0);     // Not reached.
}
// Listing 2

