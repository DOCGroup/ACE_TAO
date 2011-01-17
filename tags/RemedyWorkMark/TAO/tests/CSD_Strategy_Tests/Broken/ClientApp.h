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

    virtual int run_i(int argc, ACE_TCHAR* argv[]);


  private:

    // These are all called, in order, by the run_i() method.
    int init(int argc, ACE_TCHAR* argv[]);
    void poa_setup(void);
    void csd_setup(void);
    void client_setup(void);
    void poa_activate(void);
    void run_clients(void);
    void run_orb_event_loop(void);
    bool check_validity ();
    void cleanup();

    // Helper methods used by the methods above.
    int parse_args(int argc, ACE_TCHAR* argv[]);

    int set_arg(unsigned&   value,
                 const ACE_TCHAR* arg,
                 char        opt,
                 const char* name,
                 int         min = 0);

    void usage_statement();
    int arg_dependency_checks();

    PortableServer::POA_ptr create_poa(CORBA::ORB_ptr orb,
                                       const char* poa_name);


    typedef ServantList<Callback_i> ServantListType;

    CORBA::ORB_var               orb_;
    PortableServer::POA_var      poa_;
    TAO::CSD::TP_Strategy_Handle tp_strategy_;

    ServantListType servants_;

    ClientTask client_task_;

    ACE_TString exe_name_;

    unsigned num_servants_;
    unsigned num_csd_threads_;
    unsigned num_orb_threads_;

    ACE_TString         ior_;
    unsigned            client_kind_;
    unsigned            client_id_;
};

#endif
