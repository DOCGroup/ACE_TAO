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

class Echo_Caller : public POA_Test::Echo_Caller
{
public:
  Echo_Caller(CORBA::ORB_ptr orb);

  virtual void start_task(Test::Echo_ptr client);

  virtual void shutdown(void);

private:
  CORBA::ORB_var orb_;
};

#endif /* Echo_Caller__h_ */
