//
// $Id$
//

#include "Client_Task.h"

ACE_RCSID(Muxing, Client_Task, "$Id$")

Client_Task::Client_Task (Test::Sender_ptr reply_gen,
                          Test::Receiver_ptr us,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
  , sender_(Test::Sender::_duplicate (reply_gen))
  , us_ (Test::Receiver::_duplicate (us))

{
}

int
Client_Task::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting client task\n"));

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      for (int i = 0; i != 1; ++i)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) sending oneways...\n"));

          this->sender_->send_ready_message (this->us_.in ()
                                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                            "Caught Exception");
      return -1;
    }
  ACE_ENDTRY;
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Client task finished\n"));
  return 0;
}
