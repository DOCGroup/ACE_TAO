//=============================================================================
/**
 *  @file    timeout_client.cpp
 *
 *  $Id$
 *
 *  Tests for proper handling of timeouts with AMI
 *
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================

#include "timeout_client.h"
#include "tao/Messaging.h"

TimeoutClient::TimeoutClient (CORBA::ORB_ptr orb,
                              TimeoutObj_ptr timeoutObject,
                              AMI_TimeoutObjHandler_ptr replyHandlerObject,
                              TimeoutHandler_i *timeoutHandler_i,
                              unsigned long timeToWait)
: orb_ (CORBA::ORB::_duplicate (orb))
  , timeoutObject_ (TimeoutObj::_duplicate (timeoutObject))
  , replyHandlerObject_ (AMI_TimeoutObjHandler::_duplicate (replyHandlerObject))
  , timeoutHandler_i_ (timeoutHandler_i)
  , local_reply_excep_counter_ (0)
  , INVOKE_SYNCH (0)
  , INVOKE_ASYNCH (1)
  , timeToWait_ (timeToWait)
{

}


TimeoutClient::~TimeoutClient ()
{

}



int
TimeoutClient::svc ()
{
  this->initialize ();

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {

      // Tests timeouts for synchronous
      this->synch_test ();

      // Tests AMI timeouts for influences on non-timeout calls
      this->none_test ();

      // Tests AMI timeouts for accuracy
      this->accuracy_test ();

      // Tests AMI timeouts for influences on non-timeout calls
      this->none_test ();

      // shut down remote ORB
      timeoutObject_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_Time_Value tv (0, 20); // wait for the ORB to deliver the shutdonw
      ACE_OS::sleep (tv);

      // shut down local ORB
      orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Caught exception:");
      return 1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "TimeoutClient::svc: Done\n\n"));

  return 0;
}

int
TimeoutClient::initialize ()
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::Object_var object =
        orb_->resolve_initial_references ("ORBPolicyManager"
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policy_manager_ =
        CORBA::PolicyManager::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Caught exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}


void
TimeoutClient::send (CORBA::Boolean async,
                     unsigned long local_timeout,
                     unsigned long remote_sleep)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Invoking "));
  if (async)
    ACE_DEBUG ((LM_DEBUG,
                "asynch "));
  else
    ACE_DEBUG ((LM_DEBUG,
                "synch "));

  ACE_DEBUG ((LM_DEBUG,
              "local: %dms remote: %dms  ... ",
              local_timeout,
              remote_sleep));

  CORBA::PolicyList policy_list (1);

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY_EX (normal)
    {
      if (local_timeout != 0)
        {
          TimeBase::TimeT timeout = 10000 * local_timeout;

          CORBA::Any any_orb;
          any_orb <<= timeout;

          policy_list.length (1);
          policy_list[0] =
            orb_->create_policy (Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
                                 any_orb
                                 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK_EX (normal);

          policy_manager_->set_policy_overrides (policy_list,
                                                 CORBA::SET_OVERRIDE
                                                 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK_EX (normal);
        }
      else
        {
          policy_list.length (0);
          policy_manager_->set_policy_overrides (policy_list,
                                                 CORBA::SET_OVERRIDE
                                                 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK_EX (normal);
        }


      // Start time measurement at the reply handler
      timeoutHandler_i_->start ();

      if (async)
        {
          timeoutObject_->sendc_sendTimeToWait (replyHandlerObject_.in (),
                                                remote_sleep
                                                ACE_ENV_ARG_PARAMETER);
        }
      else // synch
        {
          timeoutObject_->sendTimeToWait (remote_sleep
                                          ACE_ENV_ARG_PARAMETER);
        }
      ACE_TRY_CHECK_EX (normal);
    }
  ACE_CATCH (CORBA::TIMEOUT, timeout)
    {
      local_reply_excep_counter_++;

      // Trap this exception and continue...
      ACE_DEBUG ((LM_DEBUG,
                  " timeout "));

    }
  ACE_ENDTRY;
  ACE_CHECK;

  // get rid of the policy, you created before.
  ACE_TRY_EX (cleanup)
    {
      if (local_timeout != 0)
        {
          policy_list[0]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK_EX (cleanup);
        }
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Error: Unexpected exception\n\n"));
    }
  ACE_ENDTRY;
  ACE_CHECK;

  // wait for responses
  ACE_Time_Value tv (0, (local_timeout + remote_sleep)*2000 + 4000);
  ACE_OS::sleep (tv);

  ACE_Time_Value &elapsed_time = timeoutHandler_i_->elapsed_time ();

  if (async)
    ACE_DEBUG ((LM_DEBUG,
                " .. needed %dms\n",
                elapsed_time.msec()));
  else
    ACE_DEBUG ((LM_DEBUG,
                " .. done\n"));
}


int
TimeoutClient::synch_test ()
{
  ACE_DEBUG ((LM_DEBUG,
              "TimeoutClient::synch_test - Begin\n"));
  timeoutHandler_i_->reset_reply_counter ();
  timeoutHandler_i_->reset_reply_excep_counter ();
  local_reply_excep_counter_ = 0;

  this->send (INVOKE_SYNCH,
              0,    // local
              0);   // remote

  this->send (INVOKE_SYNCH,
              timeToWait_,    // local
              timeToWait_*2); // remote

  this->send (INVOKE_SYNCH,
              0,    // local
              0);   // remote

  if (timeoutHandler_i_->reply_counter () != 0
   || timeoutHandler_i_->reply_excep_counter () != 0
   || local_reply_excep_counter_ != 1)
    ACE_DEBUG ((LM_DEBUG,
                "**** Error in replies %d %d %d.\n\n",
                timeoutHandler_i_->reply_counter (),
                timeoutHandler_i_->reply_excep_counter (),
                local_reply_excep_counter_));

  ACE_DEBUG ((LM_DEBUG,
              "TimeoutClient::synch_test - End\n\n"));

  return 0;
}

int
TimeoutClient::accuracy_test ()
{
  ACE_DEBUG ((LM_DEBUG,
              "TimeoutClient::accuracy_test - Begin\n\n"));

  timeoutHandler_i_->reset_reply_counter ();
  timeoutHandler_i_->reset_reply_excep_counter ();
  local_reply_excep_counter_ = 0;

  this->send (INVOKE_ASYNCH,
              timeToWait_,
              (unsigned long)(timeToWait_*1.5));

  this->send (INVOKE_ASYNCH,
              timeToWait_,
              (unsigned long)(timeToWait_*0.5));

  if (timeoutHandler_i_->reply_counter () != 1
   || timeoutHandler_i_->reply_excep_counter () != 1
   || local_reply_excep_counter_ != 0)
    ACE_DEBUG ((LM_DEBUG,
                "**** Error in replies %d %d %d.\n\n",
                timeoutHandler_i_->reply_counter (),
                timeoutHandler_i_->reply_excep_counter (),
                local_reply_excep_counter_));

  ACE_DEBUG ((LM_DEBUG,
              "TimeoutClient::accuracy_test - End\n\n"));

  return 0;
}

int
TimeoutClient::none_test ()
{
  ACE_DEBUG ((LM_DEBUG,
              "TimeoutClient::none_test - Begin\n\n"));

  timeoutHandler_i_->reset_reply_counter ();
  timeoutHandler_i_->reset_reply_excep_counter ();
  local_reply_excep_counter_ = 0;

  this->send (INVOKE_ASYNCH,
              0,
              0);

  this->send (INVOKE_ASYNCH,
              timeToWait_,
              timeToWait_+10);  // trigger a timeout

  this->send (INVOKE_ASYNCH,
              timeToWait_,
              0);

  if (timeoutHandler_i_->reply_counter () != 2
   || timeoutHandler_i_->reply_excep_counter () != 1
   || local_reply_excep_counter_ != 0)
    ACE_DEBUG ((LM_DEBUG,
                "**** Error in replies %d %d %d.\n\n",
                timeoutHandler_i_->reply_counter (),
                timeoutHandler_i_->reply_excep_counter (),
                local_reply_excep_counter_));

  ACE_DEBUG ((LM_DEBUG,
              "TimeoutClient::none_test - End\n\n"));

  return 0;
}
