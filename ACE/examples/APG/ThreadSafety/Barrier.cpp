// $Id$

#include "ace/config-lite.h"
#if defined (ACE_HAS_THREADS)

#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_time.h"
#include "ace/Task.h"
#include "ace/Synch.h"

// Listing 2 code/ch14
class HA_CommandHandler : public ACE_Task<ACE_MT_SYNCH>
{
public:
  enum { N_THREADS = 5 };

  HA_CommandHandler (ACE_Barrier& startup_barrier,
                     ACE_Barrier &shutdown_barrier)
    : startup_barrier_(startup_barrier),
      shutdown_barrier_(shutdown_barrier)
    { }

  void initialize_handler (void);
  int handle_command_requests (void);

  int svc (void)
  {
    initialize_handler ();
    startup_barrier_.wait ();
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t: %D) Started\n")));

    while (handle_command_requests () > 0)
      ;

    shutdown_barrier_.wait ();
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t: %D) Ended\n")));

    return 0;
  }

private:
  ACE_Barrier& startup_barrier_;
  ACE_Barrier& shutdown_barrier_;
};
// Listing 2

void
HA_CommandHandler::initialize_handler (void)
{
  ACE_Time_Value tv (0, ACE_OS::rand () * 100);
  timespec_t t = (timespec_t)tv;
  ACE_OS::nanosleep (&t);
}

int
HA_CommandHandler::handle_command_requests (void)
{
  ACE_Time_Value tv (0, ACE_OS::rand () * 100);
  timespec_t t = (timespec_t)tv;

  // Simulate work.
  ACE_OS::nanosleep (&t);

  return -1;
}

// Listing 1 code/ch14
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_Barrier startup_barrier (HA_CommandHandler::N_THREADS);
  ACE_Barrier shutdown_barrier (HA_CommandHandler::N_THREADS);

  HA_CommandHandler handler (startup_barrier, shutdown_barrier);
  handler.activate (THR_NEW_LWP | THR_JOINABLE,
                    HA_CommandHandler::N_THREADS);
  handler.wait ();
  return 0;
}
// Listing 1

#else
#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_OS::puts (ACE_TEXT ("This example requires threads."));
  return 0;
}

#endif /* ACE_HAS_THREADS */
