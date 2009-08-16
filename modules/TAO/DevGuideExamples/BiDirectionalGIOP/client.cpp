// $Id$

#include "bidir_giop_pch.h"

#include "callback_i.h"
#include "simpleC.h"

#include "ace/Get_Opt.h"
#include "tao/BiDir_GIOP/BiDirGIOP.h"

#include <iostream>

ACE_TString ior = ACE_TEXT ("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
  {
      case 'k':
        ior = get_opts.optarg;
        break;

      case '?':
      default:
        std::cerr << "usage: " << argv[0] << " -k <ior>" << std::endl;
        return -1;
        break;
  }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  try {
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    if (parse_args (argc, argv) != 0) {
      return 1;
    }

    // Create a bidirectional POA
    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow(obj.in());
    PortableServer::POAManager_var poa_manager = root_poa->the_POAManager();
    // Policies for the childPOA to be created.
    CORBA::PolicyList policies(1);
    policies.length(1);
    CORBA::Any pol;
    pol <<= BiDirPolicy::BOTH;
    policies[0] =
      orb->create_policy(BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE, pol);
    // Create POA as child of RootPOA with the above policies.  This POA
    // will receive request in the same connection in which it sent
    // the request
    PortableServer::POA_var poa = root_poa->create_POA("bidirPOA", poa_manager.in(), policies);
    // Creation of bidirPOA is over. Destroy the Policy objects.
    for (CORBA::ULong i = 0; i < policies.length (); ++i) {
      policies[i]->destroy ();
    }
    poa_manager->activate ();

    // get server object
    obj = orb->string_to_object(ior.c_str());
    Simple_var server = Simple::_narrow (obj.in());

    PortableServer::Servant_var<Callback_i> callback_svt = new Callback_i(orb.in());

    // Register and activate callback servant
    PortableServer::ObjectId_var id = poa->activate_object(callback_svt.in());
    obj = poa->id_to_reference(id.in());
    Callback_var callback = Callback::_narrow(obj.in());

    server->callback_object (callback.in());

    CORBA::Long r = server->test_method(1); // Tell the server to call us back.
    if (r != 0) {
      std::cout << "unexpected result = " << r << std::endl;
    }

    orb->run();

    CORBA::Boolean etherealize = true, wait = true;
    poa->destroy(etherealize, wait);
    orb->destroy();

    return 0;
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "Caught CORBA::Exception: " << std::endl << ex << std::endl;
  }

  return 1;
}
