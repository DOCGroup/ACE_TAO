// $Id$
#ifndef SERVERAPP_H
#define SERVERAPP_H

#include "ace/SString.h"
#include "ace/CORBA_macros.h"
#include "tao/Environment.h"


class ServerApp
{
  public:

    ServerApp();
    ~ServerApp();

    int run(int argc, char* argv[]);


  private:

    int parse_args(int argc, char* argv[]);
    void usage_statement();

    ACE_CString exe_name_;
    ACE_CString ior_filename_;
    unsigned    num_clients_;
    unsigned    num_orb_threads_;
};

#endif
