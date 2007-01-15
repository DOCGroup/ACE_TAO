#ifndef Echo__h_
#define Echo__h_
/**
 * @file Echo.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */
#include "TestS.h"

class Echo : public POA_Test::Echo
{
public:
  Echo(CORBA::ORB_ptr orb,
       int abort_counter);

  virtual void echo_payload (Test::Payload const & data
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;

  int abort_counter_;
};

#endif /* Echo__h_ */
