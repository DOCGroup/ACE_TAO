// $Id$
//
// ============================================================================
//
// = LIBRARY
//    examples
// 
// = FILENAME
//    test_window_messages.cpp
//
// = DESCRIPTION
//
//    Tests the Msg_WFMO_Reactor's ability to handle regular events
//    and window messages.
//    
// = AUTHOR
//    
//    Irfan Pyarali
// 
// ============================================================================

#include "ace/Msg_WFMO_Reactor.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID(ReactorEx, test_window_messages, "$Id$")

class Event_Handler : public ACE_Event_Handler
{
public:
  int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);  

  ACE_Auto_Event handle_;
  int iterations_;
};

int 
Event_Handler::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG, 
              "handling signal: %d iterations left\n", 
              --this->iterations_));

  if (this->iterations_ == 0)
    ACE_Reactor::end_event_loop ();    

  return 0;
}

Event_Handler event_handler;
  
void WINAPI
timer_callback (HWND hwnd,
                UINT uMsg,
                UINT idEvent,
                DWORD dwTime)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) timeout occured @ %d\n", dwTime));

  event_handler.handle_.signal ();
}

int 
main (int argc, char** argv)
{
  // Manage memory automagically.
  // Note that ordering here is important.
  ACE_Reactor_Impl *impl = new ACE_Msg_WFMO_Reactor;
  auto_ptr<ACE_Reactor> reactor (new ACE_Reactor (impl));
  ACE_Reactor::instance (reactor.get ());
  auto_ptr<ACE_Reactor_Impl> delete_impl (impl);

  event_handler.iterations_ = 5;
  ACE_ASSERT (ACE_Reactor::instance ()->register_handler (&event_handler, 
                                                          event_handler.handle_.handle ()) == 0);
  
  ACE_Time_Value timeout (1);
  ACE_ASSERT (::SetTimer (NULL,                         // handle of window for timer messages 
                          NULL,                         // timer identifier 
                          timeout.msec (),              // time-out value 
                          (TIMERPROC) &timer_callback   // address of timer procedure 
                          ) != 0);
  
  ACE_Reactor::run_event_loop ();

  return 0;
}

