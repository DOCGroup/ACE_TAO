// ============================================================================
// @(#)test_remove_handler.cpp	1.1	10/18/96

//
// = LIBRARY
//    examples
// 
// = FILENAME
//    test_remove_handler.cpp
//
// = DESCRIPTION
//    Tests the ReactorEx's ability to handle simultaneous events.  If
//    you pass anything on the command-line, then each handler
//    requests to be removed from the ReactorEx after each event.
//    This has a funky effect on the order in which handlers are
//    serviced.  So, if no parameters are passed in, the handlers
//    should be serviced 1 through MAXIMUM_WAIT_OBJECTS.  If handlers
//    to request to be removed as signals occur, they will be serviced
//    1, MAX, MAX-1, ..., 2.  This is because of a ReactorEx
//    bookkeeping optimization.
//
// = AUTHOR
//    Tim Harrison
// 
// ============================================================================

#include "ace/ReactorEx.h"
#include "ace/Service_Config.h"
#include "ace/Synch.h"

class Event_Handler : public ACE_Event_Handler
// = TITLE
//    Generic Event Handler.
//
// = DESCRIPTION
//
//    Creates event.  Registers with ReactorEx.  Signals event.  If
//    created with -close_down- it returns -1 from handle signal.
{
public:
  Event_Handler (int event_number, 
		 int close_down)
    : event_number_ (event_number),
      close_down_ (close_down)
    {
      ACE_Service_Config::reactorEx ()->register_handler (this,
							  this->event_.handle ());
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
      //      ACE_DEBUG ((LM_DEBUG, "event handler %d closed.\n", event_number_));
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
  int close_down = arg > 1 ? 1 : 0;

  for (int i = 0; i < ACE_ReactorEx::MAX_SIZE; i++)
    new Event_Handler (i, close_down);

  ACE_Service_Config::reactorEx ()->handle_events ();
  return 42;
}
