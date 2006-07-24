// $Id$
#ifndef CLIENTAPP_H
#define CLIENTAPP_H

#include "TestAppBase.h"
#include "ClientEngine.h"
#include "tao/ORB.h"
#include "ace/SString.h"


class ClientApp : public TestAppBase
{
  public:

    ClientApp();
    virtual ~ClientApp();

  protected:

    virtual int run_i(int argc, char* argv[] ACE_ENV_ARG_DECL);

  private:

    // These are all called, in order, by the run_i() method.
    int init(int argc, char* argv[] ACE_ENV_ARG_DECL);
    void client_setup(ACE_ENV_SINGLE_ARG_DECL);
    int  run_engine(ACE_ENV_SINGLE_ARG_DECL);
    void cleanup();

    // Helper methods used by the methods above.
    int parse_args(int argc, char* argv[]);

    int set_arg(unsigned&   value,
                 const char* arg,
                 char        opt,
                 const char* name,
                 int         min = 0);

    void usage_statement();
    int arg_dependency_checks();


    CORBA::ORB_var      orb_;
    ACE_CString         exe_name_;
    ACE_CString         ior_;
    unsigned            client_kind_;
    ClientEngine_Handle engine_;
    unsigned            client_id_;
};

#endif
