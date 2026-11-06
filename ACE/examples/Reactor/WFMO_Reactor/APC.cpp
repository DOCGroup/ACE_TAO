
//=============================================================================
/**
 *  @file    APC.cpp
 *
 *  Tests the WFMO_Reactor's ability to handle regular APC
 *  notifications.
 *
 *  @author Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================

#include "ace/OS_main.h"

#if defined (ACE_WIN32) && _WIN32_WINNT >= 0x400

#include "ace/Reactor.h"
#include "ace/Auto_Event.h"
#include "ace/Log_Msg.h"

class Event_Handler : public ACE_Event_Handler
{
public:
  Event_Handler () = default;
  Event_Handler (Event_Handler const &) = delete;
  Event_Handler (Event_Handler &&) = delete;
  Event_Handler &operator= (Event_Handler const &) = delete;
  Event_Handler &operator= (Event_Handler &&) = delete;

  int handle_signal (int signum, siginfo_t * = nullptr, ucontext_t * = nullptr) override;

  int handle_timeout (ACE_Time_Value const &tv, void const *arg = nullptr) override;

  ACE_Auto_Event handle_;
  int iterations_ {};
};

static Event_Handler *global_event_handler {};

static void WINAPI
apc_callback (DWORD)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) apc occurred @ %T\n"));

  global_event_handler->handle_.signal ();
}

static void
queue_apc ()
{
  DWORD result = ::QueueUserAPC (reinterpret_cast<PAPCFUNC> (&apc_callback),
                                                         // pointer to APC function
                                 ::GetCurrentThread (),  // handle to the thread
                                 0);                     // argument for the APC function
  if (result == FALSE)
    ACE_OS::exit (-1);
}

int
Event_Handler::handle_signal (int, siginfo_t *, ucontext_t *)
{
  --this->iterations_;

  if (this->iterations_ == 0)
    {
      ACE_Reactor::instance ()->remove_handler (this->handle_.handle (),
                                                ACE_Event_Handler::DONT_CALL);
      ACE_Reactor::end_event_loop ();
    }

  return 0;
}

int
Event_Handler::handle_timeout (ACE_Time_Value const &,
                               void const *)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) timeout occurred @ %T\n"));
  queue_apc ();
  return 0;
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  Event_Handler event_handler;
  event_handler.iterations_ = 5;
  global_event_handler = &event_handler;

  int result = ACE_Reactor::instance ()->register_handler (&event_handler,
                                                           event_handler.handle_.handle ());
  ACE_TEST_ASSERT (result == 0);

  ACE_Time_Value timeout (2);
  result = ACE_Reactor::instance ()->schedule_timer (&event_handler,
                                                     0,
                                                     timeout,
                                                     timeout);
  ACE_TEST_ASSERT (result != -1);

  ACE_Reactor::run_alertable_event_loop ();

  ACE_Reactor::instance ()->cancel_timer(&event_handler);

  return 0;
}
#else /* !ACE_WIN32 */
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  return 0;
}
#endif /* ACE_WIN32 */
