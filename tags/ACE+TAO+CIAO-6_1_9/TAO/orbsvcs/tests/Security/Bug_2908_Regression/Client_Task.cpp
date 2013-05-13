//
// $Id$
//

#include "Client_Task.h"
#include "MessengerC.h"



Client_Task::Client_Task (
        CORBA::ORB_ptr corb,
        Messenger_ptr messenger,
        ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
    , result_ (0)
    , corb_ (CORBA::ORB::_duplicate (corb))
    , messenger_ (Messenger::_duplicate (messenger))
{
}

int
Client_Task::svc (void)
{
  try
    {
      // call message
      messenger_->call_message("user");
    }
  catch(const CORBA::NO_PERMISSION ex)
    {
      result_ = 1;
      ex._tao_print_exception ("ERROR: Caught CORBA::NO_PERMISSION exception");
    }
  catch(const CORBA::Exception &ex)
    {
      result_ = 1;
      ex._tao_print_exception ("Caught exception:");
    }

  return 0;
}
