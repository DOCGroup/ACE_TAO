// -*- C++ -*-

#if !defined (SERVER_I_H)
#define SERVER_I_H

#include "testS.h"

#include "ace/Auto_Ptr.h"
#include "ace/SString.h"

class Server_i
{
public:
  Server_i (void);
  ~Server_i (void);

  int init (int argc, ACE_TCHAR **argv);

  int run (void);

private:
  int parse_args (int argc, ACE_TCHAR* argv[]);

  CORBA::ORB_var orb_;
  PortableServer::POA_var root_poa_;

  ACE_CString server_name_;
  ACE_CString second_name_;
};

#endif /* SERVER_I_H */
