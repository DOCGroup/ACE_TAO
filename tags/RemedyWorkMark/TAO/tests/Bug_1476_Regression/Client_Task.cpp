//
// $Id$
//

#include "Client_Task.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/OS_NS_unistd.h"

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

  try
    {
      // Apply sync_none policy
      CORBA::Object_var object =
        orb_->resolve_initial_references ("PolicyCurrent");

      CORBA::PolicyCurrent_var policy_current =
        CORBA::PolicyCurrent::_narrow (object.in ());

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
                            scope_as_any);
      policy_current->set_policy_overrides (policies, CORBA::ADD_OVERRIDE);

      policies[0]->destroy ();

      for (int i = 0; i != number_; ++i)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) sending oneway invocation %d...\n", i));

          this->sender_->send_ready_message ();

          // Do it slowly.
          ACE_OS::sleep(ACE_Time_Value(0,250000));
         }
       }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught Exception");
      return -1;
    }
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Client task finished\n"));
  return 0;
}
