// $Id$
#ifndef SERVERAPP_H
#define SERVERAPP_H

#include "TestAppBase.h"
#include "ace/SString.h"


class ServerApp : public TestAppBase
{
  public:

    ServerApp();
    virtual ~ServerApp();


  protected:

    virtual int run_i(int argc, char* argv[]);


  private:

    int parse_args(int argc, char* argv[]);

    ACE_CString ior_filename_;
    unsigned    num_clients_;
};

#endif
