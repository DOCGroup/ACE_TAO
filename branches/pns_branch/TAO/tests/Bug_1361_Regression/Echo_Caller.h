/**
 * @file Echo_Caller.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */
#ifndef Echo_Caller__h_
#define Echo_Caller__h_

#include "TestS.h"

class Thread_Pool;

class Echo_Caller : public POA_Test::Echo_Caller
{
public:
  Echo_Caller(CORBA::ORB_ptr orb, Thread_Pool *pool_);

  virtual void start_task(Test::Echo_ptr client
                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC((CORBA::SystemException));

  virtual void shutdown(ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
  Thread_Pool *pool_;
};

#endif /* Echo_Caller__h_ */
