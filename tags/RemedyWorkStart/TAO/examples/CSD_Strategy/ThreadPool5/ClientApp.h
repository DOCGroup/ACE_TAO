// $Id$
#ifndef CLIENTAPP_H
#define CLIENTAPP_H

#include "ace/SString.h"
#include "ace/CORBA_macros.h"
#include "tao/Environment.h"

class ClientTask;

class ClientApp
{
  public:

    ClientApp();
    ~ClientApp();

    int run(int argc, ACE_TCHAR* argv[]);


  private:

    int parse_args(int argc, ACE_TCHAR* argv[]);
    void usage_statement();

    ACE_TString ior_;

    ACE_TString exe_name_;
};

#endif
