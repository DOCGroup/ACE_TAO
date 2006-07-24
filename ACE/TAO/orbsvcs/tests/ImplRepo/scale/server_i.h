// -*- C++ -*-
// $Id$

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

  int init (int argc, char **argv ACE_ENV_ARG_DECL);

  int run (ACE_ENV_SINGLE_ARG_DECL);

private:
  int parse_args (int argc, char* argv[]);

  CORBA::ORB_var orb_;
  PortableServer::POA_var root_poa_;

  ACE_CString server_name_;
  int count_;
};

#endif /* SERVER_I_H */
