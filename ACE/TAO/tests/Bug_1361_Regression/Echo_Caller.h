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
  Echo_Caller(Thread_Pool *pool_);

  virtual void start_task(Test::Echo_ptr client);

  virtual void shutdown(void);

private:
  Thread_Pool *pool_;
};

#endif /* Echo_Caller__h_ */
