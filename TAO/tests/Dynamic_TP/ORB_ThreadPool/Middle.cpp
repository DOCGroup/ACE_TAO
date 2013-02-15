// $Id$

#include "Test_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Thread_Manager.h"

#include "tao/ORB_Core_TSS_Resources.h"
#include "tao/ORB_Core.h"
#include "tao/Dynamic_TP/DTP_Thread_Pool.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("middle.ior");
const ACE_TCHAR *upstream_ior = ACE_TEXT ("file://server.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:o:"));
  int c;

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

      if (parse_args (argc, argv) != 0)
        return 1;

      obj = orb->string_to_object (upstream_ior);
      Test::Sleeper_var sleeper = Test::Sleeper::_narrow (obj.in ());

      if (CORBA::is_nil (sleeper.in()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Middle panic: nil sleeper reference\n"),
                          1);


      Middle_i *test_impl;
      ACE_NEW_RETURN (test_impl, Middle_i (orb.in (), sleeper.in()), 1);
      PortableServer::ServantBase_var owner_transfer(test_impl);
      PortableServer::ObjectId_var id = root_poa->activate_object (test_impl);
      obj = root_poa->id_to_reference (id.in ());

      Test::Middle_var middle = Test::Middle::_narrow (obj.in ());
      CORBA::String_var ior = orb->object_to_string (middle.in ());

      ACE_DEBUG ((LM_DEBUG,"Middle calling poa_manager::activate()\n"));
      poa_manager->activate ();

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

      TAO_ORB_Core_TSS_Resources &tss =
        *orb->orb_core ()->get_tss_resources ();

      // The API calls it a lane but DTP Thread Pools
      // are always a single lane.
      TAO_DTP_Thread_Pool *pool =
        static_cast <TAO_DTP_Thread_Pool *> (tss.lane_);

      if (pool == 0)
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
