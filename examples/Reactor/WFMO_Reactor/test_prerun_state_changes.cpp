// $Id$
//
// ============================================================================
//
// = LIBRARY
//    examples
// 
// = FILENAME
//    test_prerun_state_changes.cpp
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

ACE_RCSID(ReactorEx, test_prerun_state_changes, "$Id$")

class Event_Handler : public ACE_Event_Handler
// = TITLE
//    Generic Event Handler.
//
{
public:
  virtual int handle_close (ACE_HANDLE handle, ACE_Reactor_Mask mask)
    {
      ACE_DEBUG ((LM_DEBUG, 
                  "event handler %d closed.\n", 
                  (int) handle));
      delete this;
      return 0;
    }
};

int
main (int argc, char *argv[])
{
  ACE_HANDLE handle = (ACE_HANDLE) ::socket (PF_INET, SOCK_STREAM, 0);

  Event_Handler *event_handler = new Event_Handler;
  
  ACE_ASSERT (ACE_Reactor::instance ()->register_handler (handle, 
                                                          event_handler, 
                                                          ACE_Event_Handler::READ_MASK) == 0);
  
  ACE_ASSERT (ACE_Reactor::instance ()->register_handler (handle, 
                                                          event_handler, 
                                                          ACE_Event_Handler::WRITE_MASK | ACE_Event_Handler::QOS_MASK) == 0);
  
  ACE_ASSERT (ACE_Reactor::instance ()->remove_handler (handle, 
                                                        ACE_Event_Handler::READ_MASK) == 0);

  return 0;
}
