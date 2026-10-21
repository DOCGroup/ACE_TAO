#include "TestS.h"

class Server : public POA_Test::Server
{
public:
  Server (CORBA::ORB_ptr orb)
    : POA_Test::Server (),
      orb_ (CORBA::ORB::_duplicate (orb)) {}
  ~Server () override = default;

  Server (Server const &) = delete;
  Server (Server &&) = delete;
  Server &operator= (Server const &) = delete;
  Server &operator= (Server &&) = delete;

  void setup (::Test::Client_ptr cli) override;

  void request (::CORBA::Long sel) override;

  void shutdown () override;

private:
  CORBA::ORB_var orb_;
  ::Test::Client_var cli_;
};


