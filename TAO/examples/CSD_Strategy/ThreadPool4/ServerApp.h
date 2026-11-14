#ifndef SERVERAPP_H
#define SERVERAPP_H

#include "FooC.h"
#include "tao/PortableServer/PortableServer.h"
#include "ace/SString.h"

class ServerApp
{
public:
  ServerApp () = default;
  ~ServerApp () = default;

  int run (int argc, ACE_TCHAR *argv[]);

private:
  int parse_args (int argc, ACE_TCHAR *argv[]);
  void usage_statement () const;

  Foo_ptr create_foo (PortableServer::POA_ptr poa, char const *servant_name);

  Callback_ptr create_callback (PortableServer::POA_ptr poa,
                                char const *servant_name);

  ACE_TString exe_name_ {};

  CORBA::ORB_var orb_ {};

  bool synch_with_server_ {};
};

#endif
