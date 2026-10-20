/**
 * @file Server_i.h
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#ifndef BUG_1020_REGRESSION_SERVER_I_H
#define BUG_1020_REGRESSION_SERVER_I_H

#include "TestS.h"

class Server : public POA_Test::Server
{
public:
  Server (CORBA::ORB_ptr orb);
  Server (Server const &) = delete;
  Server &operator= (Server const &) = delete;
  Server (Server &&) = delete;
  Server &operator= (Server &&) = delete;

  void start_task (Test::Echo_ptr client) override;

private:
  CORBA::ORB_var orb_;
};

#endif /* BUG_1020_REGRESSION_SERVER_I_H*/
