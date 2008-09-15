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

    virtual int run_i (int argc, ACE_TCHAR *argv[]);

  private:

    int parse_args (int argc, ACE_TCHAR *argv[]);

    ACE_CString ior_filename_;
    unsigned    num_clients_;
};

#endif
