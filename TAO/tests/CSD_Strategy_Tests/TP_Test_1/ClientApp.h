#ifndef CLIENTAPP_H
#define CLIENTAPP_H

#include "TestAppBase.h"
#include "ace/SString.h"


class ClientApp : public TestAppBase
{
  public:
    ClientApp();
    ~ClientApp() override;

  protected:
    int run_i(int argc, ACE_TCHAR* argv[]) override;

    ClientApp (ClientApp const &) = delete;
    ClientApp (ClientApp &&) = delete;
    ClientApp & operator= (ClientApp const &) = delete;
    ClientApp & operator= (ClientApp &&) = delete;


  private:
    int parse_args(int argc, ACE_TCHAR* argv[]);

    ACE_TString ior_;
};

#endif
