// $Id$
//
// ============================================================================
//
// = LIBRARY
//    examples
//
// = FILENAME
//    Prerun_State_Changes.cpp
//
// = DESCRIPTION
//
//    Tests the Reactor's ability to handle state changes before
//    getting a chance to run.
//
// = AUTHOR
//
//    Irfan Pyarali
//
// ============================================================================

#include "ace/Reactor.h"
#include "ace/OS_main.h"
#include "ace/OS_NS_sys_socket.h"
#include "ace/Log_Msg.h"



class Event_Handler : public ACE_Event_Handler
// = TITLE
//    Generic Event Handler.
//
{
public:
  virtual int handle_close (ACE_HANDLE handle, ACE_Reactor_Mask mask)
    {
      ACE_UNUSED_ARG(mask);
      ACE_DEBUG ((LM_DEBUG,
                  "event handler %d closed.\n",
                  (size_t) handle));
      delete this;
      return 0;
    }
};

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_HANDLE handle = ACE_OS::socket (PF_INET, SOCK_STREAM, 0);

  Event_Handler *event_handler = new Event_Handler;

  int result = ACE_Reactor::instance ()->register_handler (handle,
                                                           event_handler,
                                                           ACE_Event_Handler::READ_MASK);
  ACE_ASSERT (result == 0);

  result = ACE_Reactor::instance ()->register_handler (handle,
                                                       event_handler,
                                                       ACE_Event_Handler::WRITE_MASK | ACE_Event_Handler::QOS_MASK);
  ACE_ASSERT (result == 0);

  result = ACE_Reactor::instance ()->remove_handler (handle,
                                                     ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);
  ACE_ASSERT (result == 0);

  return 0;
}
