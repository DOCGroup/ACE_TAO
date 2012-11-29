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

#include "test_config.h"
#include "ace/Pipe.h"
#include "ace/Reactor.h"
#include "ace/Select_Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/WFMO_Reactor.h"
#include "ace/ACE.h"

static const char message[] = "abcdefghijklmnopqrstuvwxyz";
static const size_t message_size = 26;
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
  bool ok_;
};

Event_Handler::Event_Handler (ACE_Reactor &reactor,
                              ACE_HANDLE read,
                              ACE_HANDLE write)
  : ACE_Event_Handler (&reactor),
    pipe_ (read, write),
    ok_ (false)
{
  if (read == ACE_INVALID_HANDLE)
    {
      if (0 != this->pipe_.open ())
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n"),
                      ACE_TEXT ("Event_Handler pipe")));
          return;
        }
    }

  if (0 != this->reactor ()->register_handler (this->pipe_.read_handle (),
                                               this,
                                               ACE_Event_Handler::READ_MASK))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p\n"),
                  ACE_TEXT ("Event_Handler register_handler")));
      return;
    }

  ssize_t result = ACE::send_n (this->pipe_.write_handle (),
                                message,
                                message_size);
  if (result != ssize_t (message_size))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Event_Handler sent %b bytes; should be %B\n"),
                  result, message_size));
      if (result <= 0)
        return;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Event_Handler::Event_Handler for %@\n"),
              this));
  this->ok_ = true;
}

Event_Handler::~Event_Handler (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Event_Handler::~Event_Handler for %@\n"),
              this));
}

int
Event_Handler::handle_input (ACE_HANDLE handle)
{
  char buf[message_size + 1];

  ssize_t result =
    ACE::recv_n (handle,
                 buf,
                 sizeof buf - 1);
  if (result != static_cast<ssize_t> (message_size))
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Handler recv'd %b bytes; expected %B\n"),
                result, message_size));
  buf[result > 0 ? result : 0] = '\0';

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Message %C received for %@\n"),
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
              ACE_TEXT ("\nTesting with %C\n\n"),
              reactor_type));

  ACE_Reactor reactor (&reactor_impl,
                       0);

  Event_Handler *e = new Event_Handler (reactor,
                                        ACE_INVALID_HANDLE,
                                        ACE_INVALID_HANDLE);
  if (!e->ok_)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Error initializing test; aborting.\n")));
      delete e;
      return;
    }

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

  // The ACE_WFMO_Reactor stuff needs Winsock2
#if defined (ACE_WIN32) && \
    (defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0))
  iteration = 1;
  ACE_WFMO_Reactor wfmo_reactor;
  test (wfmo_reactor, "ACE_WFMO_Reactor");
#endif /* ACE_WIN32 && ACE_HAS_WINSOCK2 */

  ACE_END_TEST;

  return 0;
}
