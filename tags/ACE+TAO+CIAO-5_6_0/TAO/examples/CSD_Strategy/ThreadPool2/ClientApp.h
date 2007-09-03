// $Id$
#ifndef CLIENTAPP_H
#define CLIENTAPP_H

#include "ace/SString.h"
#include "ace/CORBA_macros.h"
#include "tao/Environment.h"


class ClientApp
{
  public:

    ClientApp();
    ~ClientApp();

    int run(int argc, char* argv[]);


  private:

    int parse_args(int argc, char* argv[]);
    void usage_statement();

    ACE_CString ior_;

    ACE_CString exe_name_;
};

#endif
