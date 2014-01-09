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

    int run(int argc, ACE_TCHAR* argv[]);


  private:

    int parse_args(int argc, ACE_TCHAR* argv[]);
    void usage_statement();

    ACE_TString exe_name_;
    ACE_TString ior_filename_;
    unsigned    num_servants_;
    unsigned    num_clients_;
};

#endif
