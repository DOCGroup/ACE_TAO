// $Id$

#include "Messenger_i.h"
#include "common.h"
#include <iostream>
#include <fstream>
#include <fstream>
#include "tao/RTCORBA/RTCORBA.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("Messenger.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    if (parse_args (argc, argv) != 0)
        return 1;

    // Get the RTORB.
    CORBA::Object_var obj = orb->resolve_initial_references("RTORB");
    RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow(obj.in());

    // PolicyCurrent.
    obj = orb->resolve_initial_references("PolicyCurrent");
    CORBA::PolicyCurrent_var policy_current =
              CORBA::PolicyCurrent::_narrow(obj.in());
    if (CORBA::is_nil(policy_current.in())) {
      std::cerr << "Unable to narrow the PolicyCurrent" << std::endl;
      return 1;
    }

    //Get reference to Root POA
    obj = orb->resolve_initial_references( "RootPOA" );
    PortableServer::POA_var poa = PortableServer::POA::_narrow( obj.in() );

    // Activate POA Manager
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    mgr->activate();

    // Create the thread-pool
    const CORBA::Short increment = get_increment();
    RTCORBA::ThreadpoolLanes lanes(get_total_lanes());
    lanes.length(get_total_lanes());
    std::cout << "Creating " << get_total_lanes() << " lane"
              << (get_total_lanes() == 1 ? "" : "s") << std::endl;
    for(CORBA::ULong i = 0; i < get_total_lanes(); i++) {
      lanes[i].static_threads = 1;
      lanes[i].dynamic_threads = 0;

      lanes[i].lane_priority = i * increment;
      std::cout << "  Priority: " << lanes[i].lane_priority << std::endl;
    }

    RTCORBA::ThreadpoolId threadpool_id =
      rt_orb->create_threadpool_with_lanes (0,  // Stack Size
                                            lanes,
                                            false,  // Allow borrowing
                                            false,  // Allow request buffering
                                            0,  // Max buffered requests
                                            0); // Max request buffer size

    CORBA::PolicyList poa_policy_list(2);
    poa_policy_list.length (2);

    poa_policy_list[0] =
      rt_orb->create_priority_model_policy(RTCORBA::CLIENT_PROPAGATED, 0);
    poa_policy_list[1] =
      rt_orb->create_threadpool_policy(threadpool_id);

    PortableServer::POA_var client_propagated_poa =
    poa->create_POA ("client_propagated_poa",
                     mgr.in (),
                     poa_policy_list);

    // Create an object
    PortableServer::Servant_var<Messenger_i> messenger_servant
      = new Messenger_i(orb.in());

    // Register the servant with the RootPOA, obtain its object
    // reference, stringify it, and write it to a file.
    PortableServer::ObjectId_var oid =
      client_propagated_poa->activate_object( messenger_servant.in() );
    CORBA::Object_var messenger_obj =
      client_propagated_poa->id_to_reference( oid.in() );
    CORBA::String_var str = orb->object_to_string( messenger_obj.in() );
    std::ofstream iorFile(ACE_TEXT_ALWAYS_CHAR(ior_output_file));
    iorFile << str.in() << std::endl;
    iorFile.close();
    std::cout << "IOR written to file " << ACE_TEXT_ALWAYS_CHAR(ior_output_file) << std::endl;

    // Accept requests
    orb->run();
    orb->destroy();
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "Caught CORBA exception: " << ex << std::endl;
    return 1;
  }

  return 0;
}
