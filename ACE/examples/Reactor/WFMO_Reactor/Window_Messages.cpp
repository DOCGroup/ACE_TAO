
//=============================================================================
/**
 *  @file    Window_Messages.cpp
 *
 *  Tests the Msg_WFMO_Reactor's ability to handle regular events
 *  and window messages.
 *
 *  @author Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================

#include "ace/OS_main.h"

#if defined (ACE_WIN32) && !defined (ACE_LACKS_MSG_WFMO)

#include "ace/Msg_WFMO_Reactor.h"
#include "ace/Reactor.h"
#include "ace/Auto_Event.h"
#include <memory>
#include <new>

class Event_Handler : public ACE_Event_Handler
{
public:
  Event_Handler () = default;
  Event_Handler (Event_Handler const &) = delete;
  Event_Handler (Event_Handler &&) = delete;
  Event_Handler &operator= (Event_Handler const &) = delete;
  Event_Handler &operator= (Event_Handler &&) = delete;
  int handle_signal (int signum, siginfo_t * = nullptr, ucontext_t * = nullptr) override;

  ACE_Auto_Event handle_;
  int iterations_ {};
};

int
Event_Handler::handle_signal (int, siginfo_t *, ucontext_t *)
{
  --this->iterations_;

  if (this->iterations_ == 0)
    ACE_Reactor::end_event_loop ();

  return 0;
}

static Event_Handler *global_event_handler {};

static void CALLBACK
timer_callback (HWND,
                UINT,
                UINT_PTR,
                DWORD dwTime)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) timeout occurred @ %u\n", dwTime));

  global_event_handler->handle_.signal ();
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  // Manage memory automagically.
  ACE_Reactor_Impl *impl_raw {};
  ACE_NEW_RETURN (impl_raw, ACE_Msg_WFMO_Reactor, 1);
  std::unique_ptr<ACE_Reactor_Impl> impl (impl_raw);
  ACE_Reactor *reactor_raw {};
  ACE_NEW_RETURN (reactor_raw, ACE_Reactor (impl.get (), 1), 1);
  impl.release ();
  std::unique_ptr<ACE_Reactor> reactor (reactor_raw);
  ACE_Reactor::instance (reactor.get ());

  Event_Handler event_handler;
  global_event_handler = &event_handler;

  event_handler.iterations_ = 5;
  int const result =
    ACE_Reactor::instance ()->register_handler (&event_handler,
                                                event_handler.handle_.handle ());
  ACE_TEST_ASSERT (result == 0);

  ACE_Time_Value const timeout (1);
  UINT_PTR const timer_id =
    ::SetTimer (nullptr,           // handle of window for timer messages
                0,                 // timer identifier
                timeout.msec (),   // time-out value
                &timer_callback); // address of timer procedure

  ACE_TEST_ASSERT (timer_id != 0);

  ACE_Reactor::run_event_loop ();
  ::KillTimer (nullptr, timer_id);
  global_event_handler = nullptr;
  ACE_Reactor::instance (nullptr);

  return 0;
}
#else /* !ACE_WIN32 */
int
ACE_TMAIN (int , ACE_TCHAR *[])
{
  return 0;
}
#endif /* ACE_WIN32 && !ACE_LACKS_MSG_WFMO */
