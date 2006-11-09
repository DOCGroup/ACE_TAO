// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Bug_2653_Regression_Test.cpp
//
// = DESCRIPTION
//    This bug occurs when schedule_wakeup is called for a handle that does
//    not already have an event handler registered. This can happen quite
//    legitimately in multithreaded applications where one thread schedules
//    the wakeup while another thread is handling the closure of the
//    connection and unregistering.
//
// = AUTHOR
//    Phil Mesnier <mesnier_p@ociweb.com>
//
// ============================================================================

#include "test_config.h"
#include "ace/OS_NS_string.h"
#include "ace/Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/Pipe.h"
#include "ace/ACE.h"
#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"

ACE_RCSID(tests, Bug_2653_Regression_Test, "$Id$")

static const char *message =
"Hello there! Hope you get this message";

class Watchdog : public ACE_Task_Base
{
public:
  int svc (void);
};

int
Watchdog::svc (void)
{
  ACE_OS::sleep (5);
  // If we make it through the sleep, that means the process is hung
  ACE_ASSERT (0);
  return 0;
}

class Handler : public ACE_Event_Handler
{
public:
  Handler (ACE_Reactor &reactor, bool close_other);

  ~Handler();

  int handle_input (ACE_HANDLE fd);

  int handle_output (ACE_HANDLE fd);

  ACE_Pipe pipe_;
  ACE_Pipe other_pipe_;
};

Handler::Handler (ACE_Reactor &reactor, bool close_other)
  : ACE_Event_Handler (&reactor)
{
  // Create the pipe.
  bool ok = true;
  if (0 != this->other_pipe_.open () || 0 != this->pipe_.open())
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("pipe")));
      ok = false;
    }
  else
    {
      // Register for all events.
      if (0 != this->reactor ()->register_handler
                 (this->pipe_.read_handle (),
                  this,
                  ACE_Event_Handler::READ_MASK))
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("register")));
          ok = false;
        }

      if (-1 == this->reactor ()->schedule_wakeup
          (this->other_pipe_.write_handle(),
           ACE_Event_Handler::WRITE_MASK))
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("Schedule")));
          ok = false;
        }

      // closing the other pipe sets up the spinner error.
      // leaving it open sets up the segv.
      if (close_other)
        this->other_pipe_.close();

    }
  ACE_ASSERT (ok);
}

Handler::~Handler (void)
{
  this->other_pipe_.close();
  this->pipe_.close ();
}

int
Handler::handle_output (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Handler::handle_output\n")));

#if defined (__OpenBSD__) || defined (ACE_VXWORKS) || defined (__Lynx__)
  // All that we need written has been written, so don't
  // call handle_output again.
  this->reactor ()->mask_ops (this->pipe_.read_handle (),
                              ACE_Event_Handler::WRITE_MASK,
                              ACE_Reactor::CLR_MASK);
#endif /* __OpenBSD__ || ACE_VXWORKS || __Lynx__ */

  return 0;
}

int
Handler::handle_input (ACE_HANDLE fd)
{
  char buffer[BUFSIZ];
  ssize_t result = ACE::recv (fd, buffer, sizeof buffer);

  ACE_ASSERT (result == ssize_t (ACE_OS::strlen (message)));
  buffer[result] = '\0';

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Handler::handle_input: %C\n"), buffer));

  ACE_ASSERT (ACE_OS::strcmp (buffer,
                              message) == 0);

  this->reactor ()->end_reactor_event_loop ();
  return 0;
}

static void
test_for_crash (ACE_Reactor &reactor)
{
  Handler handler (reactor, false);

  // This should trigger a call to <handle_input>.
  ssize_t result =
    ACE::send_n (handler.pipe_.write_handle (),
                 message,
                 ACE_OS::strlen (message));
  ACE_ASSERT (result == ssize_t (ACE_OS::strlen (message)));

  reactor.run_reactor_event_loop ();

  result =
    reactor.remove_handler (handler.pipe_.read_handle (),
                            ACE_Event_Handler::ALL_EVENTS_MASK |
                            ACE_Event_Handler::DONT_CALL);
  ACE_ASSERT (result == 0);

  result =
    reactor.remove_handler (handler.other_pipe_.write_handle (),
                            ACE_Event_Handler::ALL_EVENTS_MASK |
                            ACE_Event_Handler::DONT_CALL);
  ACE_ASSERT (result != 0);
}

static void
test_for_spin (ACE_Reactor &reactor)
{
  Handler handler (reactor, true);

  // This should trigger a call to <handle_input>.
  ssize_t result =
    ACE::send_n (handler.pipe_.write_handle (),
                 message,
                 ACE_OS::strlen (message));
  ACE_ASSERT (result == ssize_t (ACE_OS::strlen (message)));

  reactor.run_reactor_event_loop ();

  result =
    reactor.remove_handler (handler.pipe_.read_handle (),
                            ACE_Event_Handler::ALL_EVENTS_MASK |
                            ACE_Event_Handler::DONT_CALL);
  ACE_ASSERT (result == 0);

  result =
    reactor.remove_handler (handler.other_pipe_.write_handle (),
                            ACE_Event_Handler::ALL_EVENTS_MASK |
                            ACE_Event_Handler::DONT_CALL);
  ACE_ASSERT (result != 0);
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_2653_Regression_Test"));

  ACE_TP_Reactor tp_reactor_impl;
  ACE_Reactor tp_reactor (&tp_reactor_impl);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing for crash\n")));
  test_for_crash (tp_reactor);
  // if that passes, start the watchdog. We don't need to wait
  Watchdog wd;
  wd.activate (THR_DETACHED);
  test_for_spin(tp_reactor);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing for spin\n")));

  ACE_END_TEST;
  return 0;
}
