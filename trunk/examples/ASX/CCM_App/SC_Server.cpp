// Simple driver program for the server.
// $Id$


#include "ace/Service_Config.h"
#include "ace/Synch.h"
#include "ace/Signal.h"

class Event_Handler : public ACE_Event_Handler
{
public:
  virtual int handle_input (ACE_HANDLE handle);
  virtual int handle_close (ACE_HANDLE, 
			    ACE_Reactor_Mask);
};

int 
Event_Handler::handle_input (ACE_HANDLE handle)
{
  char buf[BUFSIZ];

  ssize_t n = ACE_OS::read (handle, buf, sizeof buf);

  if (n == -1)
    return -1;
  else if (ACE_OS::write (ACE_STDOUT, buf, n) != n)
    return -1;
  else
    return 0;
}

int 
Event_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, "closing Event_Handler\n"));
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_Service_Config loggerd;
  Event_Handler handler;
  ACE_Sig_Adapter shutdown_handler ((ACE_Sig_Handler_Ex) ACE_Reactor::end_event_loop);

  if (ACE::register_stdin_handler (&handler,
				   ACE_Reactor::instance(),
				   ACE_Thread_Manager::instance ()) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "register_stdin_handler"));

  if (loggerd.open (argc, argv) == -1 && errno != ENOENT)
    ACE_ERROR ((LM_ERROR, "%p\n%a", "open", 1));

  else if (ACE_Reactor::instance()->register_handler
    (SIGINT, &shutdown_handler) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n%a", "register_handler", 1));

  // Perform logging service until we receive SIGINT.

  ACE_Reactor::run_event_loop();

  return 0;
}
