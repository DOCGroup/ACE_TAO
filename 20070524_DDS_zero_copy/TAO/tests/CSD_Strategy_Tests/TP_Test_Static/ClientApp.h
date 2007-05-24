// $Id$
#ifndef CLIENTAPP_H
#define CLIENTAPP_H

#include "TestAppBase.h"
#include "ace/SString.h"


class ClientApp : public TestAppBase
{
  public:

    ClientApp();
    virtual ~ClientApp();

  protected:

    virtual int run_i(int argc, char* argv[]);


  private:

    int parse_args(int argc, char* argv[]);

    ACE_CString ior_;
};

#endif
