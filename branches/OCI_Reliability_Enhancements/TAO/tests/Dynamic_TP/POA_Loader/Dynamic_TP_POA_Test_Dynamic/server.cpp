// $Id$

#include "Hello.h"
#include "tao/Dynamic_TP/Dynamic_TP_POA_Strategy.h"
#include "tao/Dynamic_TP/Dynamic_TP_POA_StrategyImpl.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Thread_Manager.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("test.ior");

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
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

	 //   // Create the thread pool servant dispatching strategy object, and
  //// hold it in a (local) smart pointer variable.
	 // ACE_CString tp_config_name="POA1";
	 // TAO_Dynamic_TP_POA_StrategyImpl * dynamic_strategy = 0;
  //TAO_Intrusive_Ref_Count_Handle<TAO_Dynamic_TP_POA_Strategy> csd_strategy =
  //                                               new TAO_Dynamic_TP_POA_Strategy(tp_config_name,
		//										                                 dynamic_strategy, true);

  //// Tell the strategy to apply itself to the child poa.
  //if (csd_strategy->apply_to(root_poa.in()) == false)
  //  {
  //    ACE_ERROR((LM_ERROR,
  //               "Failed to apply CSD strategy to root poa.\n"));
  //    return -1;
  //  }

      Hello *hello_impl;
      ACE_NEW_RETURN (hello_impl,
                      Hello (orb.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer(hello_impl);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (hello_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Test::Hello_var hello =
        Test::Hello::_narrow (object.in ());

      CORBA::String_var ior =
        orb->object_to_string (hello.in ());

      // Output the IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file %s for writing IOR: %C",
                           ior_output_file,
                           ior.in ()),
                              1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (1, 1);

      orb->destroy ();

      // Wait for all CSD task threads exit.
      ACE_Thread_Manager::instance ()->wait ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
