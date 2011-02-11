// $Id$

#include "bidir_giop_pch.h"

#include "simple_i.h"
#include "callbackC.h"

#include "ace/Get_Opt.h"
#include "tao/BiDir_GIOP/BiDirGIOP.h"
#include <iostream>
#include <fstream>

ACE_TString ior_output_file;
int callback_count = 10;

int
parse_args(int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts(argc, argv, ACE_TEXT("o:i:"));
  int c;

  while((c = get_opts()) != -1)
    switch(c)
  {
      case 'o':
        ior_output_file = get_opts.optarg;
        break;
      case 'i':
        callback_count = ACE_OS::atoi(get_opts.optarg);
        break;
      case '?':
      default:
        std::cerr << "usage: " << argv[0] << "-o <iorfile> -i <no_iterations>" << std::endl;
        return -1;
        break;
  }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
  {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    if (parse_args(argc, argv) != 0) {
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

    PortableServer::Servant_var<Simple_i> svt = new Simple_i(orb.in(), callback_count);

    // Register and activate Simple servant
    PortableServer::ObjectId_var id = poa->activate_object(svt.in());
    obj = poa->id_to_reference(id.in());
    Simple_var server = Simple::_narrow(obj.in());

    CORBA::String_var ior = orb->object_to_string(server.in());
    if (ior_output_file != ACE_TEXT("")) {
      std::ofstream outfile(ACE_TEXT_ALWAYS_CHAR(ior_output_file.c_str()));
      outfile << ior.in();
    }
    std::cout << "Activated as " << ior.in() << std::endl;

    // Our own special orb->run() that knows how to callback clients
    while (true) {

      // returns 1 as soon as it has successfully called back.
      if (svt->call_client()) {
        break;
      }

      // We don't want to check for work pending, because we really want
      // to simulate a normal orb->run() while adding the ability to call
      // our routine which calls back to the client.
      orb->perform_work();
    }

    std::cout << "Event loop finished." << std::endl;

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
