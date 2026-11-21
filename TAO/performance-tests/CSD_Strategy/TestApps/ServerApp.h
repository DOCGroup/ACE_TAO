//=============================================================================
/**
 *  @file    ServerApp.h
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
  ServerApp ();
  ~ServerApp () override;

  ServerApp (ServerApp const &) = delete;
  ServerApp (ServerApp &&) = delete;
  ServerApp &operator= (ServerApp const &) = delete;
  ServerApp &operator= (ServerApp &&) = delete;

protected:
  int run_i (int argc, ACE_TCHAR *argv[]) override;

private:
  // These are all called, in order, by the run_i() method.
  int init (int argc, ACE_TCHAR *argv[]);
  void poa_setup ();
  void csd_setup ();
  void servant_setup ();
  void collocated_setup ();
  void poa_activate ();
  void run_collocated_clients ();
  void run_orb_event_loop ();
  bool check_results ();

  // Helper methods used by the methods above.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  int set_arg (unsigned &value,
               ACE_TCHAR const *arg,
               char opt,
               char const *name,
               int min = 0);

  void usage_statement ();
  int arg_dependency_checks ();

  PortableServer::POA_ptr create_poa (CORBA::ORB_ptr orb,
                                      char const *poa_name);

  using ServantListType = ServantList<Foo_i>;

  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
  TAO::CSD::TP_Strategy_Handle tp_strategy_;

  ServantListType servants_;
  ClientTask collocated_client_task_;

  ACE_TString exe_name_;
  ACE_TString ior_filename_prefix_ {ACE_TEXT("foo")};
  unsigned num_servants_ {1};
  unsigned num_csd_threads_ {1};
  unsigned num_orb_threads_ {1};
  unsigned num_remote_clients_ {1};
  unsigned num_collocated_clients_ {};
  unsigned num_loops_ {1};
  unsigned use_csd_ {1};

  ACE_CString scenario_id_ {"UnknownScenarioId"};
  unsigned trial_id_ {};

  Foo_Statistics stats_;
};

#endif
