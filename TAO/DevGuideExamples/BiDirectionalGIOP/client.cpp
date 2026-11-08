#include "bidir_giop_pch.h"

#include "callback_i.h"
#include "simpleC.h"

#include "ace/Get_Opt.h"
#include "tao/BiDir_GIOP/BiDirGIOP.h"

#include <iostream>

struct Client_Options
{
  ACE_TString ior {ACE_TEXT ("file://test.ior")};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int option;

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        std::cerr << "usage: " << argv[0] << " -k <ior>" << std::endl;
        return -1;
      }
  return 0;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb (CORBA::ORB_init (argc, argv));
      Client_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var object (orb->resolve_initial_references ("RootPOA"));
      PortableServer::POA_var const root_poa (
        PortableServer::POA::_narrow (object.in ()));
      PortableServer::POAManager_var const poa_manager (
        root_poa->the_POAManager ());

      CORBA::PolicyList policies (1);
      policies.length (1);
      CORBA::Any policy_value;
      policy_value <<= BiDirPolicy::BOTH;
      policies[0] = orb->create_policy (
        BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE, policy_value);
      PortableServer::POA_var const poa (
        root_poa->create_POA ("bidirPOA", poa_manager.in (), policies));
      policies[0]->destroy ();
      poa_manager->activate ();

      object = orb->string_to_object (options.ior.c_str ());
      Simple_var const server (Simple::_narrow (object.in ()));
      PortableServer::Servant_var<Callback_i> const callback_servant (
        new Callback_i (orb.in ()));
      PortableServer::ObjectId_var const id (
        poa->activate_object (callback_servant.in ()));
      object = poa->id_to_reference (id.in ());
      Callback_var const callback (Callback::_narrow (object.in ()));

      server->callback_object (callback.in ());
      CORBA::Long const result = server->test_method (true);
      if (result != 0)
        std::cout << "unexpected result = " << result << std::endl;

      orb->run ();
      CORBA::Boolean const etherealize = true;
      CORBA::Boolean const wait_for_completion = true;
      poa->destroy (etherealize, wait_for_completion);
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "Caught CORBA::Exception: " << std::endl << ex << std::endl;
      return 1;
    }

  return 0;
}
