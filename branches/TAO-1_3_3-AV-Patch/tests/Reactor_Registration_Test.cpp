// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Reactor_Registration_Test.cpp
//
// = DESCRIPTION
//    This is a test of registering handlers with the Reactor.
//
// = AUTHOR
//    Irfan Pyarali <irfan@oomworks.com>
//
// ============================================================================

#include "tests/test_config.h"
#include "ace/Pipe.h"
#include "ace/Select_Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/WFMO_Reactor.h"

ACE_RCSID(tests, Reactor_Registration_Test, "$Id$")

static const char message[] = "abcdefghijklmnopqrstuvwxyz";
static const int message_size = 26;
static int iteration = 1;

class Event_Handler : public ACE_Event_Handler
{
public:

  Event_Handler (ACE_Reactor &reactor,
                 ACE_HANDLE read,
                 ACE_HANDLE write);

  ~Event_Handler (void);

  int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);

  int handle_close (ACE_HANDLE handle,
                    ACE_Reactor_Mask close_mask);

  ACE_Pipe pipe_;
};

Event_Handler::Event_Handler (ACE_Reactor &reactor,
                              ACE_HANDLE read,
                              ACE_HANDLE write)
  : ACE_Event_Handler (&reactor),
    pipe_ (read, write)
{
  int result = 0;

  if (read == ACE_INVALID_HANDLE)
    {
      result =
                        this->pipe_.open ();
      ACE_ASSERT (result == 0);
      ACE_UNUSED_ARG (result);
    }

  result =
    this->reactor ()->register_handler (this->pipe_.read_handle (),
                                        this,
                                        ACE_Event_Handler::READ_MASK);
  ACE_ASSERT (result == 0);
  ACE_UNUSED_ARG (result);

  result =
    ACE::send_n (this->pipe_.write_handle (),
                 message,
                 message_size);
  ACE_ASSERT (result == message_size);
  ACE_UNUSED_ARG (result);

  ACE_DEBUG ((LM_DEBUG,
              "Event_Handler::Event_Handler for %x\n",
              this));
}

Event_Handler::~Event_Handler (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Event_Handler::~Event_Handler for %x\n",
              this));
}

int
Event_Handler::handle_input (ACE_HANDLE handle)
{
  char buf[message_size + 1];

  int result =
    ACE::recv_n (handle,
                 buf,
                 sizeof buf - 1);
  ACE_ASSERT (result == message_size);
  ACE_UNUSED_ARG (result);

  buf[message_size] = '\0';

  ACE_DEBUG ((LM_DEBUG,
              "Message %s received for %x\n",
              buf,
              this));

  return -1;
}

int
Event_Handler::handle_close (ACE_HANDLE,
                             ACE_Reactor_Mask)
{
  switch (iteration)
    {
    case 1:
      new Event_Handler (*this->reactor (),
                         ACE_INVALID_HANDLE,
                         ACE_INVALID_HANDLE);
      break;
    case 2:
      new Event_Handler (*this->reactor (),
                         this->pipe_.read_handle (),
                         this->pipe_.write_handle ());
      break;
    case 3:
      this->reactor ()->end_reactor_event_loop ();
      break;
    }

  iteration++;
  delete this;

  return 0;
}

void
test (ACE_Reactor_Impl &reactor_impl,
      const char *reactor_type)
{
  ACE_DEBUG ((LM_DEBUG,
              "\nTesting with %s\n\n",
              reactor_type));

  ACE_Reactor reactor (&reactor_impl,
                       0);

  new Event_Handler (reactor,
                     ACE_INVALID_HANDLE,
                     ACE_INVALID_HANDLE);

  reactor.run_reactor_event_loop ();
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Reactor_Registration_Test"));

  iteration = 1;
  ACE_Select_Reactor select_reactor;
  test (select_reactor, "ACE_Select_Reactor");

  iteration = 1;
  ACE_TP_Reactor tp_reactor;
  test (tp_reactor, "ACE_TP_Reactor");

#if defined (ACE_WIN32)
  iteration = 1;
  ACE_WFMO_Reactor wfmo_reactor;
  test (wfmo_reactor, "ACE_WFMO_Reactor");
#endif /* ACE_WIN32 */

  ACE_END_TEST;

  return 0;
}
