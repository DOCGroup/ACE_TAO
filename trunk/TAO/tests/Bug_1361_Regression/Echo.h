
/**
 * @file Echo.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */
#ifndef TAO_TESTS_BUG1270_ECHO_H
#define TAO_TESTS_BUG1270_ECHO_H

#include "ace/Atomic_Op.h"
#include "TestS.h"

class Echo : public POA_Test::Echo
{
public:
  Echo(CORBA::ORB_ptr orb,
       int abort_counter);

  virtual void echo_payload (Test::Payload const & data);

  virtual void echo_payload_out (
        Test::Payload_out data
      );

private:
  CORBA::ORB_var orb_;

  ACE_Atomic_Op<TAO_SYNCH_MUTEX, int> abort_counter_;
};

#endif /* TAO_TESTS_BUG1270_ECHO_H*/
