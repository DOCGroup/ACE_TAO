// -*- C++ -*-
#if !defined (SERVER_I_H)
#define SERVER_I_H

#include "testS.h"

#include <memory>
#include "ace/SString.h"

class Server_i
{
public:
  Server_i ();
  ~Server_i ();

  int init (int argc, ACE_TCHAR **argv);

  int run ();

private:
  int parse_args (int argc, ACE_TCHAR* argv[]);

  CORBA::ORB_var orb_;
  PortableServer::POA_var root_poa_;

  ACE_CString server_name_;
  int count_;
};

#endif /* SERVER_I_H */
