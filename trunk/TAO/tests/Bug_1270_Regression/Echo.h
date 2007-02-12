
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

#include "TestS.h"

class Echo : public POA_Test::Echo
{
public:
  Echo(CORBA::ORB_ptr orb,
       int abort_counter);

  virtual void echo_payload (Test::Payload const & data);

private:
  CORBA::ORB_var orb_;

  int abort_counter_;
};

#endif /* TAO_TESTS_BUG1270_ECHO_H*/
