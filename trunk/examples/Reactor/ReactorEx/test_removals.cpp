// $Id$
//
// ============================================================================
//
// = LIBRARY
//    examples
// 
// = FILENAME
//    test_removals.cpp
//
// = DESCRIPTION
//
//    Tests the Reactor's ability to handle simultaneous events.  If
//    you pass anything on the command-line, then each handler
//    requests to be removed from the Reactor after each event.
//    
// = AUTHOR
//    Tim Harrison
//    Irfan Pyarali
// 
// ============================================================================

#include "ace/Reactor.h"
#include "ace/Service_Config.h"
#include "ace/Synch.h"

class Event_Handler : public ACE_Event_Handler
// = TITLE
//    Generic Event Handler.
//
// = DESCRIPTION
//
//    Creates event.  Registers with Reactor.  Signals event.  If
//    created with -close_down- it returns -1 from handle signal.
{
public:
  Event_Handler (int event_number, 
		 int close_down)
    : event_number_ (event_number),
    close_down_ (close_down)
    {
      if (ACE_Reactor::instance ()->register_handler (this,
						      this->event_.handle ()) == -1)
	ACE_ERROR ((LM_ERROR, "%p\tevent handler %d cannot be added to Reactor\n", "", event_number_));
      this->event_.signal ();
    }

  virtual int handle_signal (int index, siginfo_t *, ucontext_t *)
    {
      ACE_DEBUG ((LM_DEBUG, "event %d occured.\n", event_number_));

      if (this->close_down_)
	return -1;
      else
	return 0;
    }

  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask)
    {
      ACE_DEBUG ((LM_DEBUG, "event handler %d closed.\n", event_number_));
      delete this;
      return 0;
    }

  virtual ACE_HANDLE get_handle (void) const
    {
      return event_.handle ();
    }

private:
  int event_number_;
  // Our event number.

  int close_down_;
  // Shall we close down or not.

  ACE_Event event_;
  // Signaled to shut down the handler.
};

int
main (int argc, char *argv[])
{
  int close_down = argc > 1 ? 1 : 0;

  for (int i = 1; i <= ACE_Reactor::instance ()->size (); i++)
    new Event_Handler (i, close_down);

  int result = 0;
  ACE_Time_Value time (1);
  while (1)
    {
      result = ACE_Reactor::instance ()->handle_events (time);
      if (result == 0 && errno == ETIME)
	{
	  ACE_DEBUG ((LM_DEBUG, "No more work left: timing out\n"));
	  break;
	}
      if (result == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p.\n", "main"), -1);	
    }	
  return 0;
}
