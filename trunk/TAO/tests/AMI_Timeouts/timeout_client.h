// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/AMI_Timeouts
//
// = FILENAME
//    timeout_client.h
//
// = DESCRIPTION
//    Tests timeouts with AMI.
//
// = AUTHOR
//    Michael Kircher <Michael.Kircher@mchp.siemens.de>
//
// ============================================================================


#include "ace/Task.h"
#include "tao/corba.h"

#include "timeoutC.h"
#include "timeout_i.h"

class TimeoutClient 
: public ACE_Task_Base
{
public:
  TimeoutClient (CORBA::ORB_ptr orb,
                 Timeout_ptr timeoutObject,
                 AMI_TimeoutHandler_ptr replyHandlerObject,
                 TimeoutHandler_i *timeoutHandler_i,
                 unsigned long timeToWait);

  ~TimeoutClient ();


private:
  // Initialize the context of this class.
  int init ();

  // Run in a separate thread.
  virtual int svc (void );

  // Wrapps complex invocations logic.
  void send (bool async,
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
  Timeout_var timeoutObject_;

  // A CORBA object reference to the reply handler
  AMI_TimeoutHandler_var replyHandlerObject_;

  // A pointer to the actual C++ reply handler implementation
  TimeoutHandler_i *timeoutHandler_i_;

  // Remember the policy manager.
  CORBA::PolicyManager_var policy_manager_;

  // Count test local exceptions
  unsigned short local_reply_excep_counter_;

  const bool INVOKE_SYNCH;

  const bool INVOKE_ASYNCH;

  unsigned int timeToWait_;
};
