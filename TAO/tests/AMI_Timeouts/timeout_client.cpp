// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/AMI_Timeouts
//
// = FILENAME
//    timeout_client.cpp
//
// = DESCRIPTION
//    Tests for proper handling of timeouts with AMI
//
// = AUTHOR
//    Michael Kircher <Michael.Kircher@mchp.siemens.de>
//
// ============================================================================


#include "timeout_client.h"

TimeoutClient::TimeoutClient (CORBA::ORB_ptr orb,
                              Timeout_ptr timeoutObject,
                              AMI_TimeoutHandler_ptr replyHandlerObject)
: orb_(CORBA::ORB::_duplicate (orb))
, timeoutObject_(Timeout::_duplicate (timeoutObject))
, replyHandlerObject_(AMI_TimeoutHandler::_duplicate (replyHandlerObject))
, INVOKE_SYNCH(false)
, INVOKE_ASYNCH(true)
{

};



TimeoutClient::~TimeoutClient ()
{

};


int
TimeoutClient::init ()
{
  ACE_TRY_NEW_ENV
    {
      CORBA::Object_var object =
        orb_->resolve_initial_references ("ORBPolicyManager",
                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      policy_manager_ =
        CORBA::PolicyManager::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Catched exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

void
TimeoutClient::send (bool async,
                     unsigned long msec)
{
  ACE_TRY_NEW_ENV
    {
      if (async)
        {
          timeoutObject_->sendc_sendTimeToWait (replyHandlerObject_,
                                                msec, 
                                                ACE_TRY_ENV);
        }
      else // synch
        {
          timeoutObject_->sendTimeToWait (msec, 
                                          ACE_TRY_ENV);
        }

      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::TIMEOUT, timeout)
    {
      // Trap this exception and continue...
      ACE_DEBUG ((LM_DEBUG,
                  "==> Trapped a TIMEOUT exception (expected)\n\n"));

      // @@Michael: I took over these comments from the timeout example..
      // Sleep so the server can send the reply...
      //ACE_Time_Value tv (0, (msec + 1)  * 1000);
      //orb_->run (tv);

    }
  ACE_ENDTRY;
}

int
TimeoutClient::svc ()
{
  this->init ();

  ACE_TRY_NEW_ENV
    {
      unsigned long msec = 10;
      
      TimeBase::TimeT timeout = 10000 * msec; 

      CORBA::Any any_orb;
      any_orb <<= timeout;

      CORBA::PolicyList policy_list (1);
      policy_list.length (1);
      policy_list[0] =
        orb_->create_policy (Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
                            any_orb,
                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      policy_manager_->set_policy_overrides (policy_list,
                                             CORBA::SET_OVERRIDE,
                                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "TimeoutClient::svc : Invoking sendTimeToWait synchronously with server delay\n\n"));

      this->send (INVOKE_SYNCH,
                  msec);
      // @@ Michael: If the reply to this request times out the replies to the asynch invocations
      //             do not get dispatched to the reply handler. Why?


      ACE_DEBUG ((LM_DEBUG,
                  "TimeoutClient::svc : Invoking sendTimeToWait asynchronously without server delay\n\n"));

      this->send (INVOKE_ASYNCH,
                  0);


      ACE_DEBUG ((LM_DEBUG,
                  "TimeoutClient::svc : Invoking sendTimeToWait asynchronously with server delay\n\n"));

      this->send (INVOKE_ASYNCH,
                  msec);

      ACE_Time_Value tv (1, 0); // wait 1s to give the responses enough time.
      ACE_OS::sleep (tv);

      // shut down ORB 
      //timeoutObject_->shutdown (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      policy_list[0]->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Catched exception:");
      return 1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "TimeoutClient::svc: Done\n\n"));

  return 0;
};

