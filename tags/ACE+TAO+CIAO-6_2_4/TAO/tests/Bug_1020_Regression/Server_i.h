/**
 * @file Server_i.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */
#ifndef BUG_1020_REGRESSION_SERVER_I_H
#define BUG_1020_REGRESSION_SERVER_I_H

#include "TestS.h"

class Server : public POA_Test::Server
{
public:
  Server(CORBA::ORB_ptr orb);

  virtual void start_task(Test::Echo_ptr client);

private:
  CORBA::ORB_var orb_;
};

#endif /* BUG_1020_REGRESSION_SERVER_I_H*/
