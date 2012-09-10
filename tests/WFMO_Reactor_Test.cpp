
//=============================================================================
/**
 *  @file    WFMO_Reactor_Test.cpp
 *
 *  $Id$
 *
 *  This is a simple test of the WFMO_Reactor.  It makes sure that
 *  removals and suspensions work correctly.
 *
 *
 *  @author Irfan Pyarali <irfan@oomworks.com>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Reactor.h"
#include "ace/WFMO_Reactor.h"
#include "ace/Pipe.h"



#if defined (ACE_WIN32)

static int number_of_handlers = 6;
static int number_of_closes = 0;

class Event_Handler : public ACE_Event_Handler
{
public:

  Event_Handler (ACE_Reactor &reactor);

  ~Event_Handler (void);

  ACE_Pipe pipe_;

};

Event_Handler::Event_Handler (ACE_Reactor &reactor)
{
  this->reference_counting_policy ().value
    (ACE_Event_Handler::Reference_Counting_Policy::ENABLED);

  ACE_DEBUG ((LM_DEBUG,
              "Reference count in Event_Handler() is %d\n",
              this->reference_count_.value ()));

  this->reactor (&reactor);

  int result =
    this->pipe_.open ();

  ACE_TEST_ASSERT (result == 0);
  ACE_UNUSED_ARG (result);

  this->reactor ()->register_handler (this->pipe_.read_handle (),
                                      this,
                                      ACE_Event_Handler::READ_MASK);
  ACE_TEST_ASSERT (result == 0);

  this->reactor ()->register_handler (this->pipe_.write_handle (),
                                      this,
                                      ACE_Event_Handler::READ_MASK);
  ACE_TEST_ASSERT (result == 0);
}

Event_Handler::~Event_Handler (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Reference count in ~Event_Handler() is %d\n",
              this->reference_count_.value ()));

  ++number_of_closes;
}

void
test (void)
{
  int result = 0;
  int i = 0;

  ACE_Reactor reactor (new ACE_WFMO_Reactor, 1);

  ACE_Event_Handler_var *safe_event_handlers =
    new ACE_Event_Handler_var[number_of_handlers];

  Event_Handler **event_handlers =
    new Event_Handler*[number_of_handlers];

  for (i = 0; i < number_of_handlers; ++i)
    {
      event_handlers[i] =
        new Event_Handler (reactor);

      safe_event_handlers[i] =
        event_handlers[i];
    }

  ACE_Time_Value timeout (0, 500 * 1000);

  result = reactor.run_reactor_event_loop (timeout);
  ACE_TEST_ASSERT (result != -1);

  for (i = 0; i < number_of_handlers; ++i)
    {
      if (i % 2 == 0)
        continue;

      result = reactor.suspend_handler (event_handlers[i]->pipe_.read_handle ());
      ACE_TEST_ASSERT (result == 0);

      result = reactor.suspend_handler (event_handlers[i]->pipe_.write_handle ());
      ACE_TEST_ASSERT (result == 0);
    }

  result = reactor.run_reactor_event_loop (timeout);
  ACE_TEST_ASSERT (result != -1);

  delete[] safe_event_handlers;
  delete[] event_handlers;
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("WFMO_Reactor_Test"));

  test ();

  ACE_TEST_ASSERT (number_of_closes == number_of_handlers);

  ACE_END_TEST;

  return 0;
}

#else /* ACE_WIN32 */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("WFMO_Reactor_Test"));

  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("WFMO_Reactor not supported on this platform\n")));

  ACE_END_TEST;

  return 0;
}

#endif /* ACE_WIN32 */
