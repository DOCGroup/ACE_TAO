//
// $Id$
//

#include "Client_Task.h"

ACE_RCSID(Bug_1XXX_Regression, Client_Task, "$Id$")

Client_Task::Client_Task (Test::Hello_ptr receiver,
                          CORBA::ORB_ptr orb,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
  , receiver_ (Test::Hello::_duplicate (receiver))
  , handler_ (new Reply_Handler(receiver, orb))
  , handler_var_ (handler_->_this())
  , orb_ (CORBA::ORB::_duplicate (orb))
{
}

Client_Task::~Client_Task (void)
{
}

int
Client_Task::svc (void)
{
  //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting client task\n"));

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Start 25 separate concurrent request streams
      for (CORBA::Short i = 0; i != 25; ++i)
      {
        this->receiver_->sendc_short_sleep (this->handler_var_.in ()
            ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }

      ACE_Time_Value tv(10, 0);
      orb_->run(tv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "(%P|%t) Client_Task - caught exception:");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
