// $Id$
//
// ============================================================================
//
// = LIBRARY
//    examples
// 
// = FILENAME
//    test_console_input.cpp
//
// = DESCRIPTION
//
//    This application tests the working of ReactorEx when users are
//    interested in console input.
//
// = AUTHOR
//    Irfan Pyarali
// 
// ============================================================================

#include "ace/ReactorEx.h"

class Event_Handler : public ACE_Event_Handler
{
public:
  Event_Handler (ACE_ReactorEx &reactorEx);
  int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);  
  int handle_close (ACE_HANDLE handle,
		    ACE_Reactor_Mask close_mask);
};

Event_Handler::Event_Handler (ACE_ReactorEx &reactorEx)
{
  this->reactorEx (&reactorEx);

  if (this->reactorEx ()->register_handler (this,
					    ACE_STDIN) != 0)
    ACE_ERROR ((LM_ERROR, "Registration with ReactorEx could not be done\n"));		    
}

int 
Event_Handler::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  TCHAR buffer[BUFSIZ];
  int result = ACE_OS::read (ACE_STDIN, buffer, sizeof buffer);
  buffer[result] = '\0';

  if (result <= 0)
    {
      this->reactorEx ()->close ();
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_OS::read"), -1);
    }

  if (ACE_OS::strcmp (__TEXT("quit\r\n"), buffer) == 0)
    this->reactorEx ()->close ();

  ACE_DEBUG ((LM_DEBUG, "User input: %s", buffer));
  
  return 0;
}

int 
Event_Handler::handle_close (ACE_HANDLE handle,
			     ACE_Reactor_Mask close_mask)
{
  ACE_DEBUG ((LM_DEBUG, "Event_Handler removed from ReactorEx\n"));
  return 0;
}

int
main (int, char *[])
{
  ACE_ReactorEx reactorEx;
  Event_Handler handler (reactorEx);

  int result = 0;
  while (result != -1)
    result = reactorEx.handle_events ();

  return 0;
}
