// $Id$

#include "started_pch.h"

#include "Messenger_i.h"
#include "tao/Dynamic_TP/DTP_POA_Strategy.h"
#include <iostream>
#include <fstream>
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

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    // Initialize the ORB.
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    if (parse_args (argc, argv) != 0)
        return 1;

    //Get reference to the RootPOA.
    CORBA::Object_var obj = orb->resolve_initial_references( "RootPOA" );
    PortableServer::POA_var poa = PortableServer::POA::_narrow( obj.in() );

    // Create a configuration structure and set the values
    TAO_DTP_Definition tp_config;

    tp_config.min_threads_ = 1;                   // Set low water mark to 1 thread.
    tp_config.init_threads_ = 3;                  // Start 3 threads to start.
    tp_config.max_threads_ = -1;                  // Create threads as needed (no limit).
    tp_config.queue_depth_ = -1;                  // Allow infinite queue depth.
    tp_config.stack_size_ = (64 * 1024);          // Each thread with 64K stacksize.
    tp_config.timeout_ = ACE_Time_Value(30,0);    // Expire thread that is idle for 30 sec.

    // Create the dynamic thread pool servant dispatching strategy object, and
    // hold it in a (local) smart pointer variable.
    TAO_Intrusive_Ref_Count_Handle<TAO_DTP_POA_Strategy> dtp_strategy =
                            new TAO_DTP_POA_Strategy(&tp_config, false);

  // Tell the strategy to apply itself to the child poa.
    if (dtp_strategy->apply_to(poa.in()) == false)
      {
        ACE_ERROR((LM_ERROR,
                   "Failed to apply CSD strategy to root poa.\n"));
        return -1;
      }


    // Activate the POAManager.
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    mgr->activate();

    // Create a servant.
    PortableServer::Servant_var<Messenger_i> servant = new Messenger_i();

    // Register the servant with the RootPOA, obtain its object
    // reference, stringify it, and write it to a file.
    PortableServer::ObjectId_var oid = poa->activate_object( servant.in() );
    obj = poa->id_to_reference( oid.in() );
    CORBA::String_var str = orb->object_to_string( obj.in() );
    std::ofstream iorFile( ACE_TEXT_ALWAYS_CHAR(ior_output_file) );
    iorFile << str.in() << std::endl;
    iorFile.close();
    std::cout << "IOR written to file " << ACE_TEXT_ALWAYS_CHAR(ior_output_file) << std::endl;

    // Accept requests from clients.
    orb->run();
    orb->destroy();

    return 0;
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "CORBA exception: " << ex << std::endl;
  }

  return 1;
}
