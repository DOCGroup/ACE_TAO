// $Id$

// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    test_apc.cpp
//
// = DESCRIPTION
//
//    Tests the WFMO_Reactor's ability to handle regular APC
//    notifications.
//
// = AUTHOR
//
//    Irfan Pyarali <irfan@cs.wustl.edu>
//
// ============================================================================

#include "ace/Reactor.h"

ACE_RCSID(WFMO_Reactor, test_apc, "$Id$")

class Event_Handler : public ACE_Event_Handler
{
public:
  int handle_signal (int signum,
                     siginfo_t * = 0,
                     ucontext_t * = 0);

  int handle_timeout (const ACE_Time_Value &tv,
                      const void *arg = 0);

  ACE_Auto_Event handle_;
  int iterations_;
};

static Event_Handler event_handler;

static void WINAPI
apc_callback (DWORD)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) apc occured @ %T\n"));

  event_handler.handle_.signal ();
}

void
queue_apc (void)
{
  DWORD result = ::QueueUserAPC (&apc_callback,          // pointer to APC function
                                 ::GetCurrentThread (),  // handle to the thread
                                 0);                     // argument for the APC function
  if (result == FALSE)
    ACE_OS::exit (-1);
}

int
Event_Handler::handle_signal (int signum,
                              siginfo_t *,
                              ucontext_t *)
{
  --this->iterations_;

  if (this->iterations_ == 0)
    ACE_Reactor::end_event_loop ();

  return 0;
}

int
Event_Handler::handle_timeout (const ACE_Time_Value &tv,
                               const void *arg)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) timeout occured @ %T\n"));
  queue_apc ();
  return 0;
}

int
main (int argc, char *argv[])
{
  event_handler.iterations_ = 5;
  int result = ACE_Reactor::instance ()->register_handler (&event_handler,
                                                           event_handler.handle_.handle ());
  ACE_ASSERT (result == 0);

  ACE_Time_Value timeout (2);
  result = ACE_Reactor::instance ()->schedule_timer (&event_handler,
                                                     0,
                                                     timeout,
                                                     timeout);
  ACE_ASSERT (result != -1);

  ACE_Reactor::run_alertable_event_loop ();

  return 0;
}
