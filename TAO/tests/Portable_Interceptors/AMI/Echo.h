/**
 * @file Echo.h
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#ifndef Echo__h_
#define Echo__h_

#include "TestS.h"
#include "tao/PortableServer/PortableServer.h"

class Echo
  : public POA_Test::Echo
{
public:
  explicit Echo (CORBA::ORB_ptr orb);
  Echo (Echo const &) = delete;
  Echo (Echo &&) = delete;
  Echo &operator= (Echo const &) = delete;
  Echo &operator= (Echo &&) = delete;

  char * echo_operation (char const *the_input) override;
  void shutdown () override;

private:
  CORBA::ORB_var orb_;
};

#endif /* Echo__h_ */
