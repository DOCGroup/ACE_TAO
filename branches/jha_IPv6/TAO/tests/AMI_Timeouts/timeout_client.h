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

class TimeoutClient 
: public ACE_Task_Base
{
public:
  TimeoutClient (CORBA::ORB_ptr orb,
                 Timeout_ptr timeoutObject,
                 AMI_TimeoutHandler_ptr replyHandlerObject);

  ~TimeoutClient ();

  virtual int svc (void );

private:
  int init ();

  void send (bool async, unsigned long msec);

private:
  CORBA::ORB_var orb_;

  Timeout_var timeoutObject_;

  AMI_TimeoutHandler_var replyHandlerObject_;

  CORBA::PolicyManager_var policy_manager_;

  const bool INVOKE_SYNCH;

  const bool INVOKE_ASYNCH;
};
