// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Reactor_Dispatch_Order_Test.cpp
//
// = DESCRIPTION
//    This is a simple test that checks the order of dispatching of
//    ACE Reactors.  Order should be: timeout, output, and then input.
//
// = AUTHOR
//    Irfan Pyarali <irfan@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Reactor.h"
#include "ace/Select_Reactor.h"
#include "ace/WFMO_Reactor.h"
#include "ace/Pipe.h"

ACE_RCSID(tests, Reactor_Dispatch_Order_Test, "$Id$")

static const char *message =
"Hello there! Hope you get this message";

class Handler : public ACE_Event_Handler
{
public:
  Handler (ACE_Reactor &reactor);

  int handle_timeout (const ACE_Time_Value &tv,
                      const void *arg);

  int handle_input (ACE_HANDLE fd);

  int handle_output (ACE_HANDLE fd);

  // We need to add MSG_OOB data transfer to this test to check the
  // order of when <handle_exception> gets called.  I tried with
  // Windows 2000 but only one byte of the message came across as
  // urgent data.  The rest of the data was treated as normal! There
  // was some explanation of Microsoft's TCP/IP deals with out-of-band
  // data in "Out-of-Band Data and Push Bit in TCP/IP" in the MSDN
  // library.  However, I did not comprehend that well enough.  If
  // someone can make this work, please check the order of
  // <handle_exception> getting called.
  // int handle_exception (ACE_HANDLE fd);

  ACE_Pipe pipe_;

  int dispatch_order_;
};

Handler::Handler (ACE_Reactor &reactor)
  : ACE_Event_Handler (&reactor),
    dispatch_order_ (1)
{
  // Create the pipe.
  int result
    = this->pipe_.open ();
  ACE_ASSERT (result == 0);

  // Register for all events.
  result =
    this->reactor ()->register_handler (this->pipe_.read_handle (),
                                        this,
                                        ACE_Event_Handler::ALL_EVENTS_MASK);
  ACE_ASSERT (result == 0);
}

int
Handler::handle_timeout (const ACE_Time_Value &,
                         const void *)
{
  ACE_ASSERT (this->dispatch_order_++ == 1);

  ACE_DEBUG ((LM_DEBUG,
              "Handler::handle_timeout\n"));

  return 0;
}

int
Handler::handle_output (ACE_HANDLE)
{
  ACE_ASSERT (this->dispatch_order_++ == 2);

  ACE_DEBUG ((LM_DEBUG,
              "Handler::handle_output\n"));

#if defined (__OpenBSD__)
  // All that we need written has been written, so don't
  // call handle_output again.
  this->reactor ()->mask_ops (this->pipe_.read_handle (),
                              ACE_Event_Handler::WRITE_MASK,
                              ACE_Reactor::CLR_MASK);
#endif /* __OpenBSD__ */

  return 0;
}

int
Handler::handle_input (ACE_HANDLE fd)
{
  ACE_ASSERT (this->dispatch_order_++ == 3);

  char buffer[BUFSIZ];
  ssize_t result =
    ACE::recv (fd,
               buffer,
               sizeof buffer);

  ACE_ASSERT (result == ssize_t (ACE_OS::strlen (message)));
  buffer[result] = '\0';

  ACE_DEBUG ((LM_DEBUG,
              "Handler::handle_input: %s\n",
              ACE_TEXT_CHAR_TO_TCHAR (buffer)));

  ACE_ASSERT (ACE_OS::strcmp (buffer,
                              message) == 0);

  this->reactor ()->end_reactor_event_loop ();

  return 0;
}

static void
test_reactor_dispatch_order (ACE_Reactor &reactor)
{
  Handler handler (reactor);

  // This should trigger a call to <handle_input>.
  int result =
    ACE::send_n (handler.pipe_.write_handle (),
                 message,
                 ACE_OS::strlen (message));
  ACE_ASSERT (result == ssize_t (ACE_OS::strlen (message)));

  // This should trigger a call to <handle_timeout>.
  long retn =
    reactor.schedule_timer (&handler,
                            0,
                            ACE_Time_Value (0));
  ACE_ASSERT (retn != -1);

  reactor.run_reactor_event_loop ();

  result =
    reactor.remove_handler (handler.pipe_.read_handle (),
                            ACE_Event_Handler::ALL_EVENTS_MASK |
                            ACE_Event_Handler::DONT_CALL);
  ACE_ASSERT (result == 0);
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Reactor_Dispatch_Order_Test"));

  ACE_Select_Reactor select_reactor_impl;
  ACE_Reactor select_reactor (&select_reactor_impl);

  test_reactor_dispatch_order (select_reactor);

  // WinCE can't do the necessary Winsock 2 things for WFMO_Reactor.
#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)

  ACE_WFMO_Reactor wfmo_reactor_impl;
  ACE_Reactor wfmo_reactor (&wfmo_reactor_impl);

  test_reactor_dispatch_order (wfmo_reactor);

#endif /* ACE_WIN32 && !ACE_HAS_WINCE */

  ACE_END_TEST;
  return 0;
}
