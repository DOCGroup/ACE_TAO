// $Id$

#include "ace/Log_Msg.h"
#include "ace/Process_Manager.h"
#include "ace/Reactor.h"

static const int NCHILDREN = 2;

// Listing 1 code/ch10
class DeathHandler: public ACE_Event_Handler
{
public:
  DeathHandler () : count_(0)
  {
    ACE_TRACE ("DeathHandler::DeathHandler");
  }

  virtual int handle_exit (ACE_Process * process)
  {
    ACE_TRACE ("DeathHandler::handle_exit");

    ACE_DEBUG
      ((LM_DEBUG,
        ACE_TEXT ("Process %d exited with exit code %d\n"),
        process->getpid (), process->return_value ()));

    if (++count_ == NCHILDREN)
      ACE_Reactor::instance ()->end_reactor_event_loop ();

    return 0;
  }

private:
  int count_;
};
// Listing 1
// Listing 0 code/ch10
int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (argc > 1)      // Running as a child.
    return 0;

  // Instantiate a process manager with space for
  // 10 processes.
  ACE_Process_Manager pm (10, ACE_Reactor::instance ());

  // Create a process termination handler.
  DeathHandler handler;

  // Specify the options for the new processes to be spawned.
  ACE_Process_Options options;
  options.command_line (ACE_TEXT ("%s a"), argv[0]);

  // Spawn two child processes.
  pid_t pids[NCHILDREN];
  pm.spawn_n (NCHILDREN, options, pids);

  // Register handler to be called when these processes exit.
  for (int i = 0; i < NCHILDREN; i++)
    pm.register_handler (&handler, pids[i]);

  // Run the reactor event loop waiting for events to occur.
  ACE_Reactor::instance ()->run_reactor_event_loop ();

  return 0;
}
// Listing 0
