//=============================================================================
/**
 *  @file    ClientApp.h
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================
#ifndef CLIENTAPP_H
#define CLIENTAPP_H

#include "TestInf/TestAppBase.h"
#include "TestInf/ClientEngine.h"
#include "tao/ORB.h"
#include "ace/SString.h"

class ClientApp : public TestAppBase
{
public:
  ClientApp ();
  ~ClientApp () override;

  ClientApp (ClientApp const &) = delete;
  ClientApp (ClientApp &&) = delete;
  ClientApp &operator= (ClientApp const &) = delete;
  ClientApp &operator= (ClientApp &&) = delete;

protected:
  int run_i (int argc, ACE_TCHAR *argv[]) override;

private:
  // These are all called, in order, by the run_i() method.
  int init (int argc, ACE_TCHAR *argv[]);
  void client_setup ();
  int run_engine ();

  // Helper methods used by the methods above.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  int set_arg (unsigned &value,
               ACE_TCHAR const *arg,
               char opt,
               char const *name,
               int min = 0);

  void usage_statement ();
  int arg_dependency_checks ();

  CORBA::ORB_var orb_;
  ACE_TString exe_name_;
  ACE_TString ior_ {ACE_TEXT("Not Set")};
  ClientEngine_Handle engine_;
  unsigned client_id_ {};
  unsigned num_loops_ {1};
};

#endif
