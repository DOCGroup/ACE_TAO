#ifndef TAO_NO_SERVER_MT_CLIENT_TEST_I_H
#define TAO_NO_SERVER_MT_CLIENT_TEST_I_H

#include "testS.h"

/// Implements the `Simple_Server` interface declared in `test.idl`.
class Simple_Server_i : public POA_Simple_Server
{
public:
  explicit Simple_Server_i (CORBA::ORB_ptr orb);
  Simple_Server_i (Simple_Server_i const &) = delete;
  Simple_Server_i &operator= (Simple_Server_i const &) = delete;
  Simple_Server_i (Simple_Server_i &&) = delete;
  Simple_Server_i &operator= (Simple_Server_i &&) = delete;

  void test_method () override;

  void shutdown () override;

private:
  CORBA::ORB_var const orb_;
};

#endif /* TAO_MT_CLIENT_TEST_I_H */
