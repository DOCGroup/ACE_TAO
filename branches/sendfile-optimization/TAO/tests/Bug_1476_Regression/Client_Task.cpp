//
// $Id$
//

#include "Client_Task.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/OS_NS_unistd.h"

ACE_RCSID(Bug_1476_Regression, Client_Task, "$Id$")

Client_Task::Client_Task (CORBA::ORB_ptr orb,
                          Test::Sender_ptr reply_gen,
                          ACE_Thread_Manager *thr_mgr,
                          int number_of_oneways)
  : ACE_Task_Base (thr_mgr)
  , sender_(Test::Sender::_duplicate (reply_gen))
  , orb_ (CORBA::ORB::_duplicate (orb))
  , number_ (number_of_oneways)
{
}

int
Client_Task::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting client task\n"));

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Apply sync_none policy
      CORBA::Object_var object =
        orb_->resolve_initial_references ("PolicyCurrent" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyCurrent_var policy_current =
        CORBA::PolicyCurrent::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (policy_current.in ()))
        {
          ACE_ERROR ((LM_ERROR, "ERROR: Nil policy current\n"));
          return 1;
        }
      CORBA::Any scope_as_any;
      scope_as_any <<= Messaging::SYNC_NONE;

      CORBA::PolicyList policies (1); policies.length (1);
      policies[0] =
        orb_->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                            scope_as_any
                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      policy_current->set_policy_overrides (policies, CORBA::ADD_OVERRIDE
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policies[0]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      for (int i = 0; i != number_; ++i)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) sending oneway invocation %d...\n", i));

          this->sender_->send_ready_message (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Do it slowly.
          ACE_OS::sleep(ACE_Time_Value(0,250000));
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
