//=============================================================================
/**
 *  @file    Suspended_Removals.cpp
 *
 *  $Id$
 *
 *
 *  Tests the Reactor's ability to handle removal of suspended
 *  handles.
 *
 *
 *  @author Irfan Pyarali
 */
//=============================================================================


#include "ace/OS_main.h"

#if defined (ACE_WIN32)

#include "ace/Reactor.h"
#include "ace/WFMO_Reactor.h"



class Event_Handler : public ACE_Event_Handler
{
public:

  ACE_HANDLE get_handle (void) const
  {
    return this->event_.handle ();
  }

  ACE_Event event_;
};

class ACE_WFMO_Reactor_Test
{
public:
  static void check_for_valid_state (ACE_WFMO_Reactor &wfmo_reactor,
                                     size_t handles_to_be_added,
                                     size_t handles_to_be_suspended,
                                     size_t handles_to_be_resumed,
                                     size_t handles_to_be_deleted)
  {
    ACE_TEST_ASSERT (wfmo_reactor.handler_rep_.handles_to_be_added_ == handles_to_be_added);
    ACE_TEST_ASSERT (wfmo_reactor.handler_rep_.handles_to_be_suspended_ == handles_to_be_suspended);
    ACE_TEST_ASSERT (wfmo_reactor.handler_rep_.handles_to_be_resumed_ == handles_to_be_resumed);
    ACE_TEST_ASSERT (wfmo_reactor.handler_rep_.handles_to_be_deleted_ == handles_to_be_deleted);
  }
};

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  Event_Handler handler;
  ACE_WFMO_Reactor reactor;
  ACE_Reactor base_reactor (&reactor);

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_Time_Value time (1);
  //FUZZ: enable check_for_lack_ACE_OS

  int result =
    reactor.register_handler (&handler);
  ACE_TEST_ASSERT (result == 0);

  ACE_WFMO_Reactor_Test::check_for_valid_state (reactor,
                                                1, 0, 0, 0);

  result =
    reactor.remove_handler (&handler,
                            ACE_Event_Handler::DONT_CALL |
                            ACE_Event_Handler::ALL_EVENTS_MASK);
  ACE_TEST_ASSERT (result == 0);

  ACE_WFMO_Reactor_Test::check_for_valid_state (reactor,
                                                1, 0, 0, 1);

  result = base_reactor.run_reactor_event_loop (time);
  ACE_TEST_ASSERT (result != -1);

  ACE_WFMO_Reactor_Test::check_for_valid_state (reactor,
                                                0, 0, 0, 0);

  result =
    reactor.register_handler (&handler);
  ACE_TEST_ASSERT (result == 0);

  ACE_WFMO_Reactor_Test::check_for_valid_state (reactor,
                                                1, 0, 0, 0);

  result = base_reactor.run_reactor_event_loop (time);
  ACE_TEST_ASSERT (result != -1);

  ACE_WFMO_Reactor_Test::check_for_valid_state (reactor,
                                                0, 0, 0, 0);

  result =
    reactor.suspend_handler (&handler);
  ACE_TEST_ASSERT (result == 0);

  ACE_WFMO_Reactor_Test::check_for_valid_state (reactor,
                                                0, 1, 0, 0);

  result =
    reactor.remove_handler (&handler,
                            ACE_Event_Handler::DONT_CALL |
                            ACE_Event_Handler::ALL_EVENTS_MASK);
  ACE_TEST_ASSERT (result == 0);

  ACE_WFMO_Reactor_Test::check_for_valid_state (reactor,
                                                0, 0, 0, 1);

  result = base_reactor.run_reactor_event_loop (time);
  ACE_TEST_ASSERT (result != -1);

  ACE_WFMO_Reactor_Test::check_for_valid_state (reactor,
                                                0, 0, 0, 0);

  result =
    reactor.register_handler (&handler);
  ACE_TEST_ASSERT (result == 0);

  ACE_WFMO_Reactor_Test::check_for_valid_state (reactor,
                                                1, 0, 0, 0);

  result =
    reactor.suspend_handler (&handler);
  ACE_TEST_ASSERT (result == 0);

  ACE_WFMO_Reactor_Test::check_for_valid_state (reactor,
                                                1, 1, 0, 0);

  result = base_reactor.run_reactor_event_loop (time);
  ACE_TEST_ASSERT (result != -1);

  ACE_WFMO_Reactor_Test::check_for_valid_state (reactor,
                                                0, 0, 0, 0);

  result =
    reactor.resume_handler (&handler);
  ACE_TEST_ASSERT (result == 0);

  ACE_WFMO_Reactor_Test::check_for_valid_state (reactor,
                                                0, 0, 1, 0);

  result =
    reactor.remove_handler (&handler,
                            ACE_Event_Handler::DONT_CALL |
                            ACE_Event_Handler::ALL_EVENTS_MASK);
  ACE_TEST_ASSERT (result == 0);

  ACE_WFMO_Reactor_Test::check_for_valid_state (reactor,
                                                0, 0, 0, 1);

  result = base_reactor.run_reactor_event_loop (time);
  ACE_TEST_ASSERT (result != -1);

  ACE_WFMO_Reactor_Test::check_for_valid_state (reactor,
                                                0, 0, 0, 0);

  return 0;
}
#else /* !ACE_WIN32 */
int
ACE_TMAIN (int , ACE_TCHAR *[])
{
  return 0;
}
#endif /* ACE_WIN32 */
