//=============================================================================
/**
 *  @file    Removals.cpp
 *
 *  $Id$
 *
 *
 *  Tests the Reactor's ability to handle simultaneous events.  If
 *  you pass anything on the command-line, then each handler
 *  requests to be removed from the Reactor after each event.
 *
 *
 *  @author Tim Harrison Irfan Pyarali
 */
//=============================================================================


#include "ace/OS_main.h"

#if defined (ACE_WIN32)

#include "ace/Reactor.h"
#include "ace/Service_Config.h"
#include "ace/Event.h"



/**
 * @class Event_Handler
 *
 * @brief Generic Event Handler.
 *
 * Creates event.  Registers with Reactor.  Signals event.  If
 * created with -close_down- it returns -1 from handle signal.
 */
class Event_Handler : public ACE_Event_Handler
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

  virtual int handle_signal (int, siginfo_t *, ucontext_t *)
    {
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
  /// Our event number.
  int event_number_;

  /// Shall we close down or not.
  int close_down_;

  /// Signaled to shut down the handler.
  ACE_Event event_;
};

int
ACE_TMAIN (int argc, ACE_TCHAR *[])
{
  int close_down = argc > 1 ? 1 : 0;

  for (size_t i = 1; i <= ACE_Reactor::instance ()->size (); i++)
    new Event_Handler (static_cast<int> (i), close_down);

  int result = 0;

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_Time_Value time (1);
  //FUZZ: enable check_for_lack_ACE_OS

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
#else /* !ACE_WIN32 */
int
ACE_TMAIN (int , ACE_TCHAR *[])
{
  return 0;
}
#endif /* ACE_WIN32 */
