// $Id$

// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    Window_Messages.cpp
//
// = DESCRIPTION
//
//    Tests the Msg_WFMO_Reactor's ability to handle regular events
//    and window messages.
//
// = AUTHOR
//
//    Irfan Pyarali <irfan@cs.wustl.edu>
//
// ============================================================================

#include "ace/Msg_WFMO_Reactor.h"
#include "ace/Reactor.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID(WFMO_Reactor, Window_Messages, "$Id$")

class Event_Handler : public ACE_Event_Handler
{
public:
  int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);

  ACE_Auto_Event handle_;
  int iterations_;
};

int
Event_Handler::handle_signal (int, siginfo_t *, ucontext_t *)
{
  --this->iterations_;

  if (this->iterations_ == 0)
    ACE_Reactor::end_event_loop ();

  return 0;
}

static Event_Handler *global_event_handler;

void WINAPI
timer_callback (HWND,
                UINT,
                UINT,
                DWORD dwTime)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) timeout occured @ %u\n", dwTime));

  global_event_handler->handle_.signal ();
}

int
ACE_TMAIN (int, ACE_TCHAR*[])
{
  // Manage memory automagically.
  // Note that ordering here is important.
  ACE_Reactor_Impl *impl = new ACE_Msg_WFMO_Reactor;
  auto_ptr<ACE_Reactor> reactor (new ACE_Reactor (impl));
  ACE_Reactor::instance (reactor.get ());
  auto_ptr<ACE_Reactor_Impl> delete_impl (impl);

  Event_Handler event_handler;
  global_event_handler = &event_handler;

  event_handler.iterations_ = 5;
  int result = ACE_Reactor::instance ()->register_handler (&event_handler,
                                                           event_handler.handle_.handle ());
  ACE_ASSERT (result == 0);

  ACE_Time_Value timeout (1);
  result = ::SetTimer (NULL,                         // handle of window for timer messages
                       NULL,                         // timer identifier
                       timeout.msec (),              // time-out value
                       (TIMERPROC) &timer_callback   // address of timer procedure
                       );
  ACE_ASSERT (result != 0);

  ACE_Reactor::run_event_loop ();

  return 0;
}
