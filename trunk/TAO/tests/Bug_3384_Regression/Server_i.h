
#include "TestS.h"

class Server : public POA_Test::Server
{
public:
  Server (CORBA::ORB_ptr orb)
    : POA_Test::Server(),
      orb_ (CORBA::ORB::_duplicate (orb)) {}
  virtual ~Server ();

  virtual void setup (
    ::Test::Client_ptr cli);

  virtual void request (
    ::CORBA::Long sel);

  virtual void shutdown (
    void);

private:
  CORBA::ORB_var orb_;
  ::Test::Client_var cli_;
};


