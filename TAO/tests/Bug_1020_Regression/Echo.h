#ifndef Echo__h_
#define Echo__h_
/**
 * @file Echo.h
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#include "TestS.h"

class Echo : public POA_Test::Echo
{
public:
  Echo (CORBA::ORB_ptr orb);
  Echo (Echo const &) = delete;
  Echo &operator= (Echo const &) = delete;
  Echo (Echo &&) = delete;
  Echo &operator= (Echo &&) = delete;

  Test::Payload *echo_payload (Test::Payload const &data) override;

private:
  CORBA::ORB_var orb_;
};

#endif /* Echo__h_ */
