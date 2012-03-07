
//=============================================================================
/**
 *  @file    timeout_client.h
 *
 *  $Id$
 *
 *  Tests timeouts with AMI.
 *
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================



#include "ace/Task.h"

#include "timeout_i.h"

class TimeoutClient
: public ACE_Task_Base
{
public:
  TimeoutClient (CORBA::ORB_ptr orb,
                 TimeoutObj_ptr timeoutObject,
                 AMI_TimeoutObjHandler_ptr replyHandlerObject,
                 TimeoutHandler_i *timeoutHandler_i,
                 unsigned long timeToWait);

  ~TimeoutClient ();


private:
  // Initialize the context of this class.
  int initialize ();

  // Run in a separate thread.
  virtual int svc (void );

  // Wrapps complex invocations logic.
  void send (CORBA::Boolean async,
             unsigned long local_timeout,
             unsigned long remote_sleep);

  // Test if the synchronous timeouts still work.
  int synch_test ();

  // Test the accuracy of the timeouts.
  int accuracy_test ();

  // Test if the timeout functionaltiy disturbs non-timeout invocations
  int none_test ();

private:
  CORBA::ORB_var orb_;

  // A CORBA object reference to the target object.
  TimeoutObj_var timeoutObject_;

  // A CORBA object reference to the reply handler
  AMI_TimeoutObjHandler_var replyHandlerObject_;

  // A pointer to the actual C++ reply handler implementation
  TimeoutHandler_i *timeoutHandler_i_;

  // Remember the policy manager.
  CORBA::PolicyManager_var policy_manager_;

  // Count test local exceptions
  unsigned short local_reply_excep_counter_;

  const CORBA::Boolean INVOKE_SYNCH;

  const CORBA::Boolean INVOKE_ASYNCH;

  unsigned long timeToWait_;
};
