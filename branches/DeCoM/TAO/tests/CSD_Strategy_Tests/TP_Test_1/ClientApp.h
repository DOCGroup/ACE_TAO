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

    virtual int run_i(int argc, ACE_TCHAR* argv[]);


  private:

    int parse_args(int argc, ACE_TCHAR* argv[]);

    ACE_TString ior_;
};

#endif
