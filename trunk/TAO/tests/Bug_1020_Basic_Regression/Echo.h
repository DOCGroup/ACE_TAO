
/**
 * @file Echo.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */
#ifndef BUG_1020_BASIC_REGRESSION_ECHO_H
#define BUG_1020_BASIC_REGRESSION_ECHO_H
#include "TestS.h"

class Echo : public POA_Test::Echo
{
public:

  Echo (CORBA::ORB_ptr orb,
        int abort_counter);

  virtual void echo_payload (Test::Payload & data
                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;

  int abort_counter_;

  ACE_SYNCH_MUTEX mutex_;
};

#endif /* BUG_1020_BASIC_REGRESSION_ECHO_H*/
