// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    ServerApp.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================
#ifndef SERVERAPP_H
#define SERVERAPP_H

#include "TestInf/TestAppBase.h"
#include "TestInf/ServantList_T.h"
#include "TestInf/ClientTask.h"
#include "TestServant/Foo_i.h"
#include "TestServant/Foo_Statistics.h"
#include "tao/CSD_ThreadPool/CSD_TP_Strategy.h"
#include "ace/SString.h"


class ServerApp : public TestAppBase
{
  public:

    ServerApp();
    virtual ~ServerApp();


  protected:

    virtual int run_i(int argc, char* argv[]);


  private:

    // These are all called, in order, by the run_i() method.
    int init(int argc, char* argv[]);
    void poa_setup(void);
    void csd_setup(void);
    void servant_setup(void);
    void collocated_setup();
    void poa_activate(void);
    void run_collocated_clients(void);
    void run_orb_event_loop(void);
    bool check_results();
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
                                       const char* poa_name);

    typedef ServantList<Foo_i> ServantListType;

    CORBA::ORB_var               orb_;
    PortableServer::POA_var      poa_;
    TAO::CSD::TP_Strategy_Handle tp_strategy_;

    ServantListType servants_;

    ClientTask collocated_client_task_;

    ACE_CString exe_name_;
    ACE_CString ior_filename_prefix_;
    unsigned    num_servants_;
    unsigned    num_csd_threads_;
    unsigned    num_orb_threads_;
    unsigned    num_remote_clients_;
    unsigned    num_collocated_clients_;
    unsigned    num_loops_;
    unsigned    use_csd_;

    ACE_CString scenario_id_;
    unsigned    trial_id_;

    Foo_Statistics stats_;
};

#endif
