// $Id$

#include "ace/OS_NS_sys_time.h"

// Listing 1 code/ch20
#include "ace/Timer_Queue_Adapters.h"
#include "ace/Timer_Heap.h"

typedef ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap>
   ActiveTimer;

// Listing 1
// Listing 2 code/ch20
class CB : public ACE_Event_Handler
{
public:
  CB (int id) : id_(id) { }

  virtual int handle_timeout (const ACE_Time_Value &,
                              const void *arg)
  {
    ACE_TRACE ("CB::handle_timeout");

    const int *val = static_cast<const int*> (arg);
    ACE_ASSERT((*val) == id_);

    ACE_UNUSED_ARG (val);

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Expiry handled by thread %t\n")));
    return 0;
  }

private:
  int id_;
};
// Listing 2

// Listing 3 code/ch20
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("the main thread %t has started\n")));

  // Create an "active" timer and start its thread.
  ActiveTimer atimer;
  atimer.activate ();

  CB cb1 (1);
  CB cb2 (2);
  int arg1 = 1;
  int arg2 = 2;

  // Schedule timers to go off 3 & 4 seconds from now
  // and then with an interval of 1.1 seconds.
  const ACE_Time_Value curr_tv = ACE_OS::gettimeofday ();
  ACE_Time_Value interval = ACE_Time_Value (1, 100000);

  atimer.schedule (&cb1,
                   &arg1,
                   curr_tv + ACE_Time_Value (3L),
                   interval);
  atimer.schedule (&cb2,
                   &arg2,
                   curr_tv + ACE_Time_Value (4L),
                   interval);

  ACE_Thread_Manager::instance ()->wait ();  // Wait forever.

  return 0;
}
// Listing 3

