// $Id$
//
// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    Registration.cpp
//
// = DESCRIPTION
//
//    This test application tests a wide range of registration,
//    suspension, resumption, and removal of events from Reactor.
//
//    The application initially registers two events with Reactor. A
//    auxiliary thread is created to do the signaling on the
//    events. When the first event is signaled, the event is suspended
//    from Reactor. The event is then signaled again, but is "lost"
//    since the handler has been suspended. When the second event is
//    signal, the first event is resumed and the second is
//    suspended. When the first event is signaled again, both events
//    are removed from Reactor.
//
//    This test shows off the following features of Reactor:
//    - Registration
//    - Suspension
//    - Resumption
//    - Removal (while active and while suspended)
//
// = AUTHOR
//    Irfan Pyarali
//
// ============================================================================

#include "ace/OS_main.h"

#if defined (ACE_WIN32)

#include "ace/Reactor.h"
#include "ace/Auto_Event.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Log_Msg.h"



// Globals for this test
int stop_test = 0;
ACE_Reactor reactor;


class Simple_Handler : public ACE_Event_Handler
{
public:
  Simple_Handler (void);
  // Default constructor

  virtual int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);
  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);

  ACE_Auto_Event event1_;
  ACE_Auto_Event event2_;
  int handle_signal_count_;
  int handle_close_count_;
};

Simple_Handler::Simple_Handler (void)
  : handle_signal_count_ (0),
    handle_close_count_ (0)
{
}

int
Simple_Handler::handle_signal (int, siginfo_t *s, ucontext_t *)
{
  ACE_HANDLE handle = s->si_handle_;
  ACE_UNUSED_ARG (handle);

  this->handle_signal_count_++;

  if (this->handle_signal_count_ == 1)
    this->reactor ()->suspend_handler (event1_.handle ());
  else if (this->handle_signal_count_ == 2)
    {
      this->reactor ()->resume_handler (event1_.handle ());
      this->reactor ()->suspend_handler (event2_.handle ());
    }
  else if (this->handle_signal_count_ == 3)
    {
      this->reactor ()->remove_handler (event1_.handle (),
                                        ACE_Event_Handler::NULL_MASK);
      this->reactor ()->remove_handler (event2_.handle (),
                                        ACE_Event_Handler::NULL_MASK);
    }
  return 0;
}

int
Simple_Handler::handle_close (ACE_HANDLE handle,
                              ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, "Simple_Handler::handle_close handle = %d\n", handle));
  this->handle_close_count_++;

  if (this->handle_close_count_ == 1)
    stop_test = 0;
  else if (this->handle_close_count_ == 2)
    stop_test = 1;

  return 0;
}

// Globals for this test
Simple_Handler simple_handler;

void
worker (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) Thread creation\n"));
  ACE_DEBUG ((LM_DEBUG, "(%t) Thread sleeping\n"));
  ACE_OS::sleep (1);
  ACE_DEBUG ((LM_DEBUG, "(%t) Thread signaling %d\n", simple_handler.event1_.handle()));
  simple_handler.event1_.signal ();
  ACE_DEBUG ((LM_DEBUG, "(%t) Thread sleeping\n"));
  ACE_OS::sleep (1);
  ACE_DEBUG ((LM_DEBUG, "(%t) Thread signaling %d\n", simple_handler.event1_.handle()));
  ACE_DEBUG ((LM_DEBUG, "Note: This signal should be \"lost\" because of the suspended handler\n"));
  simple_handler.event1_.signal ();
  ACE_DEBUG ((LM_DEBUG, "(%t) Thread sleeping\n"));
  ACE_OS::sleep (1);
  ACE_DEBUG ((LM_DEBUG, "(%t) Thread resetting %d\n", simple_handler.event1_.handle()));
  simple_handler.event1_.reset ();
  ACE_DEBUG ((LM_DEBUG, "(%t) Thread signaling %d\n", simple_handler.event2_.handle()));
  simple_handler.event2_.signal ();
  ACE_DEBUG ((LM_DEBUG, "(%t) Thread sleeping\n"));
  ACE_OS::sleep (1);
  ACE_DEBUG ((LM_DEBUG, "(%t) Thread signaling %d\n", simple_handler.event1_.handle()));
  simple_handler.event1_.signal ();
  ACE_DEBUG ((LM_DEBUG, "(%t) Thread death\n"));
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  int result = reactor.register_handler (&simple_handler,
                                       simple_handler.event1_.handle ());
  ACE_ASSERT (result == 0);

  result = reactor.register_handler (&simple_handler,
                                     simple_handler.event2_.handle ());
  ACE_ASSERT (result == 0);

  result = ACE_OS::thr_create ((ACE_THR_FUNC) worker, 0, 0, 0);
  ACE_ASSERT (result == 0);

  result = 0;
  while (!stop_test && result != -1)
    {
      result = reactor.handle_events ();
    }
  return 0;
};
#else /* !ACE_WIN32 */
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  return 0;
}
#endif /* ACE_WIN32 */
