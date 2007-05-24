/**
 * @file Echo_Caller.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */
#ifndef BUG_1020_BASIC_REGRESSION_ECHO_CALLER_H
#define BUG_1020_BASIC_REGRESSION_ECHO_CALLER_H
#include "TestS.h"

class Echo_Caller : public POA_Test::Echo_Caller
{
public:
  Echo_Caller (CORBA::ORB_ptr orb);

  virtual void start_task (Test::Echo_ptr client);

  virtual void shutdown (void);

private:
  CORBA::ORB_var orb_;
};

#endif /* BUG_1020_BASIC_REGRESSION_ECHO_CALLER_H*/
