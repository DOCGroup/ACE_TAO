//=============================================================================
/**
 *  @file    Prerun_State_Changes.cpp
 *
 *  Tests the Reactor's ability to handle state changes before
 *  getting a chance to run.
 *
 *  @author Irfan Pyarali
 */
//=============================================================================


#include "ace/Reactor.h"
#include "ace/OS_main.h"
#include "ace/OS_NS_sys_socket.h"
#include "ace/Log_Msg.h"


/**
 * @class Event_Handler
 *
 * @brief Generic Event Handler.
 */
class Event_Handler : public ACE_Event_Handler
{
public:
  Event_Handler () = default;
  Event_Handler (Event_Handler const &) = delete;
  Event_Handler (Event_Handler &&) = delete;
  Event_Handler &operator= (Event_Handler const &) = delete;
  Event_Handler &operator= (Event_Handler &&) = delete;

  int handle_close (ACE_HANDLE handle, ACE_Reactor_Mask mask) override
    {
      ACE_UNUSED_ARG(mask);
      ACE_DEBUG ((LM_DEBUG,
                  "event handler %d closed.\n",
                  static_cast<size_t> (handle)));
      delete this;
      return 0;
    }
};

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_HANDLE const handle = ACE_OS::socket (PF_INET, SOCK_STREAM, 0);

  // The reactor may still own this registration during process teardown.
  Event_Handler *event_handler = new Event_Handler;

  int result = ACE_Reactor::instance ()->register_handler (handle,
                                                           event_handler,
                                                           ACE_Event_Handler::READ_MASK);
  ACE_TEST_ASSERT (result == 0);

  result = ACE_Reactor::instance ()->register_handler (handle,
                                                       event_handler,
                                                       ACE_Event_Handler::WRITE_MASK | ACE_Event_Handler::QOS_MASK);
  ACE_TEST_ASSERT (result == 0);

  result = ACE_Reactor::instance ()->remove_handler (handle,
                                                     ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);
  ACE_TEST_ASSERT (result == 0);

  return 0;
}
