
//=============================================================================
/**
 *  @file    Bug_2653_Regression_Test.cpp
 *
 *  $Id$
 *
 *  This bug occurs when schedule_wakeup is called for a handle that does
 *  not already have an event handler registered. This can happen quite
 *  legitimately in multithreaded applications where one thread schedules
 *  the wakeup while another thread is handling the closure of the
 *  connection and unregistering.
 *
 *
 *  @author Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================


#include "test_config.h"
#include "ace/OS_NS_string.h"
#include "ace/Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/Pipe.h"
#include "ace/ACE.h"
#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"



static const char *message =
"Hello there! Hope you get this message";

class Watchdog : public ACE_Task_Base
{
public:
  int svc (void);
  int my_grp_;
};

int
Watchdog::svc (void)
{
  ACE_OS::sleep (5);
  // If we make it through the sleep and haven't been canceled, that
  // means the process is hung.
  if (!this->thr_mgr ()->testcancel (ACE_Thread::self ()))
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Watchdog slept without cancel - we're hung\n")));
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
  if (0 != this->other_pipe_.open () || 0 != this->pipe_.open())
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("pipe")));
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
        }

      if (-1 == this->reactor ()->schedule_wakeup
          (this->other_pipe_.write_handle(),
           ACE_Event_Handler::WRITE_MASK))
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("Schedule")));
        }

      // closing the other pipe sets up the spinner error.
      // leaving it open sets up the segv.
      if (close_other)
        this->other_pipe_.close();
    }
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

#if defined (__OpenBSD__) || defined (ACE_VXWORKS)
  // All that we need written has been written, so don't
  // call handle_output again.
  this->reactor ()->mask_ops (this->pipe_.read_handle (),
                              ACE_Event_Handler::WRITE_MASK,
                              ACE_Reactor::CLR_MASK);
#endif /* __OpenBSD__ || ACE_VXWORKS */

  return 0;
}

int
Handler::handle_input (ACE_HANDLE fd)
{
  char buffer[BUFSIZ];
  ssize_t result = ACE::recv (fd, buffer, sizeof buffer);

  if (result != ssize_t (ACE_OS::strlen (message)))
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Handler recv'd %b bytes; expected %B\n"),
                result, ACE_OS::strlen (message)));
  buffer[result] = '\0';

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Handler::handle_input: %C\n"), buffer));

  if (ACE_OS::strcmp (buffer, message) != 0)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Handler text mismatch; received \"%C\"; ")
                ACE_TEXT ("expected \"%C\"\n"),
                buffer, message));

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
  if (result != ssize_t (ACE_OS::strlen (message)))
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Handler sent %b bytes; should be %B\n"),
                result, ACE_OS::strlen (message)));

  reactor.run_reactor_event_loop ();

  if (0 != reactor.remove_handler (handler.pipe_.read_handle (),
                                   ACE_Event_Handler::ALL_EVENTS_MASK |
                                   ACE_Event_Handler::DONT_CALL))
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("test_for_handler, remove pipe")));

  if (0 == reactor.remove_handler (handler.other_pipe_.write_handle (),
                                   ACE_Event_Handler::ALL_EVENTS_MASK |
                                   ACE_Event_Handler::DONT_CALL))
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("test_for_crash remove other_pipe succeeded ")
                ACE_TEXT ("but shouldn't\n")));
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
  if (result != ssize_t (ACE_OS::strlen (message)))
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Handler sent %b bytes; should be %B\n"),
                result, ACE_OS::strlen (message)));

  reactor.run_reactor_event_loop ();

  if (0 != reactor.remove_handler (handler.pipe_.read_handle (),
                                   ACE_Event_Handler::ALL_EVENTS_MASK |
                                   ACE_Event_Handler::DONT_CALL))
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("test_for_spin, remove pipe")));

  if (0 == reactor.remove_handler (handler.other_pipe_.write_handle (),
                                   ACE_Event_Handler::ALL_EVENTS_MASK |
                                   ACE_Event_Handler::DONT_CALL))
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("test_for_spin remove other_pipe succeeded ")
                ACE_TEXT ("but shouldn't\n")));
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
  wd.activate ();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing for spin\n")));
  test_for_spin(tp_reactor);
  // If test_for_spin returns, all is well.
  wd.thr_mgr ()->cancel_grp (wd.grp_id ());
  wd.wait ();

  ACE_END_TEST;
  return 0;
}
