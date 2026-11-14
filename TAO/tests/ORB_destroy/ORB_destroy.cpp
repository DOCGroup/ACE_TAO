//=============================================================================
/**
 *  @file     ORB_destroy.cpp
 *
 *   Simple ORB destruction test.
 *
 *  @author  Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================

#include <array>

#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/Utils/ORB_Manager.h"
#include "tao/Utils/Server_Main.h"

#include "ace/Log_Msg.h"

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
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, options.orb_name);
      CORBA::Object_var const obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in ());

      if (options.activate_poa_manager)
        {
          PortableServer::POAManager_var poa_manager =
            root_poa->the_POAManager ();
          poa_manager->activate ();
        }

      if (options.destroy_poa)
        root_poa->destroy (true, true);

      if (options.destroy_orb)
        orb->destroy ();
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

  for (Destroy_Options const &options : test_cases)
    if (run_destroy_case (argc, argv, options) != 0)
      return 1;

  TAO_ORB_Manager orb_manager;
  // Check that Server_Main can be constructed with a null name.
  TAO::Utils::Server_Main<PortableServer::ServantBase> const null_name_server (nullptr);

  ACE_TCHAR orb_name[] = ACE_TEXT ("ORB_Manager_fini");
  ACE_TCHAR *manager_argv[] = {orb_name, nullptr};
  int manager_argc = 1;
  try
    {
      if (orb_manager.fini () != 0 || orb_manager.fini () != 0
          || orb_manager.init (manager_argc, manager_argv,
                               "ORB_Manager_fini") != 0
          || orb_manager.fini () != 0
          || orb_manager.fini () != 0)
        return 1;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception raised");
      return 1;
    }

  return 0;
}
