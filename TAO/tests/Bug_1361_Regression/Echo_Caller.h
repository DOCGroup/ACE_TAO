/**
 * @file Echo_Caller.h
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#ifndef Echo_Caller__h_
#define Echo_Caller__h_

#include "TestS.h"

class Thread_Pool;

class Echo_Caller : public POA_Test::Echo_Caller
{
public:
  Echo_Caller (Thread_Pool *pool_);
  Echo_Caller (Echo_Caller const &) = delete;
  Echo_Caller &operator= (Echo_Caller const &) = delete;
  Echo_Caller (Echo_Caller &&) = delete;
  Echo_Caller &operator= (Echo_Caller &&) = delete;

  void start_task (Test::Echo_ptr client) override;

  void shutdown () override;

private:
  Thread_Pool *pool_;
};

#endif /* Echo_Caller__h_ */
