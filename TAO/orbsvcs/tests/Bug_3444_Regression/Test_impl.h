#ifndef TEST_IMPL_HPP
#define TEST_IMPL_HPP

#include "TestS.h"

class Server_impl : public virtual POA_Test::Server
{
public:
  Server_impl (CORBA::ORB_ptr orb);

  Server_impl (Server_impl const &) = delete;
  Server_impl (Server_impl &&) = delete;
  Server_impl &operator= (Server_impl const &) = delete;
  Server_impl &operator= (Server_impl &&) = delete;

  void shutdown () override;

private:
  CORBA::ORB_var orb_;
};

#endif /* TEST_IMPL_HPP */
