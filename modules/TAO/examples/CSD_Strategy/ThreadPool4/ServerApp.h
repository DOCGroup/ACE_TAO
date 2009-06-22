// $Id$
#ifndef SERVERAPP_H
#define SERVERAPP_H

#include "FooC.h"
#include "tao/PortableServer/PortableServer.h"
#include "ace/SString.h"

class ServerApp
{
  public:

    ServerApp();
    ~ServerApp();

    int run(int argc, ACE_TCHAR* argv[]);


  private:

    int parse_args(int argc, ACE_TCHAR* argv[]);
    void usage_statement();

    Foo_ptr
    create_foo(
      PortableServer::POA_ptr poa,
      const char* servant_name);

    Callback_ptr
    create_callback(
      PortableServer::POA_ptr poa,
      const char* servant_name);


    ACE_TString exe_name_;

    CORBA::ORB_var orb_;

    int synch_with_server_;
};

#endif
