// $Id$

#include "Echo_Caller.h"
#include "tao/Messaging/Messaging.h"
#include "tao/Utils/Servant_Var.h"
#include "tao/ORB_Core.h"
#include "ace/Get_Opt.h"
#include "Server_Thread_Pool.h"
#include "ORB_Task.h"

ACE_RCSID(Bug_1270_Regression, server, "$Id$")

const ACE_TCHAR *ior_output_file = ACE_TEXT("test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[]);

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
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

      CORBA::Object_var object =
        orb->resolve_initial_references ("PolicyCurrent");

      if (parse_args (argc, argv) != 0)
        return 1;

      ACE_Thread_Manager mymanager;
      Thread_Pool callback_pool (&mymanager, 10);

      TAO::Utils::Servant_Var<Echo_Caller> impl;
      {
        Echo_Caller * tmp;
        // ACE_NEW_RETURN is the worst possible way to handle
        // exceptions (think: what if the constructor allocates memory
        // and fails?), but I'm not in the mood to fight for a more
        // reasonable way to handle allocation errors in ACE.
        ACE_NEW_RETURN (tmp,
                        Echo_Caller(orb.in(), &callback_pool),
                        1);
        impl = tmp;
      }

      PortableServer::ObjectId_var id =
        root_poa->activate_object (impl.in ());

      CORBA::Object_var object_act = root_poa->id_to_reference (id.in ());

      Test::Echo_Caller_var server =
        Test::Echo_Caller::_narrow (object_act.in ());

      CORBA::String_var ior =
        orb->object_to_string (server.in ());

      // If the ior_output_file exists, output the ior to it
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                              1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate ();

      ORB_Task worker (orb.in ());
      worker.activate (THR_NEW_LWP | THR_JOINABLE,
                       4);

      orb->run();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      callback_pool.shutdown ();
      mymanager.wait ();
      worker.wait ();

      root_poa->destroy (1,
                         1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

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
                           "-o <iorfile> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}
