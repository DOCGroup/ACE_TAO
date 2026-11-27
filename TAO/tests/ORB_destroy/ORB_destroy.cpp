/**
 *  @file     ORB_destroy.cpp
 *
 *   Simple ORB destruction test.
 *
 *  @author  Irfan Pyarali <irfan@cs.wustl.edu>
 */

#include <algorithm>
#include <array>

#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Servant_Base.h"
#include "tao/Utils/ORB_Manager.h"
#include "tao/Utils/Server_Main.h"

#include "ace/Log_Msg.h"

class Failing_Server : public PortableServer::ServantBase
{
public:
  int parse_args (int, ACE_TCHAR *[]) { return 0; }

  int init (CORBA::ORB_ptr orb)
  {
    CORBA::Object_var const object = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var const poa = PortableServer::POA::_narrow (object.in ());
    poa->activate_object (this);
    return -1;
  }

  int idle (int &) { return 1; }
  int fini () { return 0; }
  char const *identity () const { return "Failing_Server"; }

  void _dispatch (TAO_ServerRequest &, TAO::Portable_Server::Servant_Upcall *) override {}
  char const *_interface_repository_id () const override { return "IDL:Failing_Server:1.0"; }
};

struct Destroy_Options
{
  char const *orb_name;
  bool destroy_orb;
  bool destroy_poa;
  bool activate_poa_manager;
};

int
run_destroy_case (int argc,
                  ACE_TCHAR *argv[],
                  Destroy_Options const &options)
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv, options.orb_name);
      CORBA::Object_var const obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const root_poa = PortableServer::POA::_narrow (obj.in ());

      if (options.activate_poa_manager)
        {
          PortableServer::POAManager_var const poa_manager =
            root_poa->the_POAManager ();
          poa_manager->activate ();
        }

      if (options.destroy_poa)
      {
        root_poa->destroy (true, true);
      }

      if (options.destroy_orb)
      {
        orb->destroy ();
      }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception raised");
      return 1;
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  std::array<Destroy_Options, 8> const test_cases {{
    {"destroy_nothing_with_poa_manager", false, false, true},
    {"destroy_poa_with_poa_manager", false, true, true},
    {"destroy_orb_with_poa_manager", true, false, true},
    {"destroy_poa_and_orb_with_poa_manager", true, true, true},
    {"destroy_nothing_without_poa_manager", false, false, false},
    {"destroy_poa_without_poa_manager", false, true, false},
    {"destroy_orb_without_poa_manager", true, false, false},
    {"destroy_poa_and_orb_without_poa_manager", true, true, false}
  }};

  if (std::any_of (test_cases.begin (),
                   test_cases.end (),
                   [argc, argv] (Destroy_Options const &options)
                   {
                     return run_destroy_case (argc, argv, options) != 0;
                   }))
    {
      return 1;
    }

  TAO_ORB_Manager orb_manager;
  TAO::Utils::Server_Main<Failing_Server> const failing_server ("Failing_Server");
  ACE_TCHAR failing_name[] = ACE_TEXT ("Failing_Server");
  ACE_TCHAR *failing_argv[] = {failing_name, nullptr};
  if (failing_server.run (1, failing_argv) != -1)
    {
      return 1;
    }
  // Check that Server_Main can be constructed with a null name.
  TAO::Utils::Server_Main<PortableServer::ServantBase> const null_name_server (nullptr);

  ACE_TCHAR orb_name[] = ACE_TEXT ("ORB_Manager_fini");
  ACE_TCHAR *manager_argv[] = {orb_name, nullptr};
  try
    {
      int manager_argc = 1;
      if (orb_manager.fini () != 0 || orb_manager.fini () != 0
          || orb_manager.init (manager_argc, manager_argv,
                               "ORB_Manager_fini") != 0
          || orb_manager.fini () != 0
          || orb_manager.fini () != 0)
      {
        return 1;
      }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception raised");
      return 1;
    }

  return 0;
}
