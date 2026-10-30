/**
 * @file Echo_Caller.h
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#ifndef Echo_Caller__h_
#define Echo_Caller__h_

#include "TestS.h"

class Echo_Caller : public POA_Test::Echo_Caller
{
public:
  explicit Echo_Caller (CORBA::ORB_ptr orb);
  Echo_Caller (Echo_Caller const &) = delete;
  Echo_Caller &operator= (Echo_Caller const &) = delete;
  Echo_Caller (Echo_Caller &&) = delete;
  Echo_Caller &operator= (Echo_Caller &&) = delete;

  void start_task (Test::Echo_ptr client) override;

  void shutdown () override;

private:
  CORBA::ORB_var orb_;
};

#endif /* Echo_Caller__h_ */
