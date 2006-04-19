// $Id$
#ifndef CLIENTAPP_H
#define CLIENTAPP_H

#include "TestAppBase.h"
#include "ClientEngine.h"
#include "ServantList_T.h"
#include "Callback_i.h"
#include "ClientTask.h"
#include "tao/CSD_ThreadPool/CSD_TP_Strategy.h"
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
    void poa_setup(ACE_ENV_SINGLE_ARG_DECL);
    void csd_setup(ACE_ENV_SINGLE_ARG_DECL);
    void client_setup(ACE_ENV_SINGLE_ARG_DECL);
    void poa_activate(ACE_ENV_SINGLE_ARG_DECL);
    void run_clients(ACE_ENV_SINGLE_ARG_DECL);
    void run_orb_event_loop(ACE_ENV_SINGLE_ARG_DECL);
    bool check_validity ();
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

    PortableServer::POA_ptr create_poa(CORBA::ORB_ptr orb,
                                       const char* poa_name
                                       ACE_ENV_ARG_DECL);


    typedef ServantList<Callback_i> ServantListType;

    CORBA::ORB_var               orb_;
    PortableServer::POA_var      poa_;
    TAO::CSD::TP_Strategy_Handle tp_strategy_;

    ServantListType servants_;

    ClientTask client_task_;

    ACE_CString exe_name_;

    unsigned num_servants_;
    unsigned num_csd_threads_;
    unsigned num_orb_threads_;

    ACE_CString         ior_;
    unsigned            client_kind_;
    unsigned            client_id_;
};

#endif
