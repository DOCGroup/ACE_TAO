#include "Test_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Thread_Manager.h"
#include <memory>

#include "tao/ORB_Core_TSS_Resources.h"
#include "tao/ORB_Core.h"
#include "tao/Dynamic_TP/DTP_Thread_Pool.h"

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TString &ior_output_file,
            ACE_TString &upstream_ior)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case 'k':
        upstream_ior = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "-k <upstream_ior>"
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

      ACE_TString ior_output_file (ACE_TEXT ("middle.ior"));
      ACE_TString upstream_ior (ACE_TEXT ("file://server.ior"));
      if (parse_args (argc, argv, ior_output_file, upstream_ior) != 0)
        return 1;

      CORBA::Object_var obj =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Middle panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      obj = orb->string_to_object (upstream_ior.c_str ());
      Test::Sleeper_var const sleeper = Test::Sleeper::_narrow (obj.in ());

      if (CORBA::is_nil (sleeper.in()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Middle panic: nil sleeper reference\n"),
                          1);


      std::unique_ptr<Middle_i> test_impl =
        std::make_unique<Middle_i> (orb.in (), sleeper.in ());
      PortableServer::ServantBase_var owner_transfer (test_impl.release ());
      PortableServer::ObjectId_var id = root_poa->activate_object (owner_transfer.in ());
      obj = root_poa->id_to_reference (id.in ());

      Test::Middle_var const middle = Test::Middle::_narrow (obj.in ());
      CORBA::String_var const ior = orb->object_to_string (middle.in ());

      ACE_DEBUG ((LM_DEBUG,"Middle calling poa_manager::activate()\n"));
      poa_manager->activate ();

      // Output the IOR to the <ior_output_file>
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (ior_output_file.c_str (), "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file %s for writing IOR: %C",
                             ior_output_file.c_str (),
                             ior.in ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      TAO_ORB_Core_TSS_Resources &tss =
        *orb->orb_core ()->get_tss_resources ();

      // The API calls it a lane but DTP Thread Pools
      // are always a single lane.
      // The ORB core owns the lane and its thread pool.
      TAO_DTP_Thread_Pool * const pool =
        static_cast <TAO_DTP_Thread_Pool *> (tss.lane_);

      if (pool == nullptr)
        {
          ACE_DEBUG ((LM_DEBUG,"Middle calling orb->run()\n"));
          orb->run ();
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,"Middle calling pool->wait()\n"));
          pool->wait();
        }

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Middle - event loop finished\n"));
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Middle Exception caught:");
      return 1;
    }

  return 0;
}
