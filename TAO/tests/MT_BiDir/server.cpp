// $Id$

#include "ace/Get_Opt.h"
#include "Sender_i.h"
#include "tao/BiDir_GIOP/BiDirGIOP.h"
#include "Server_Task.h"
#include "tao/ORB_Core.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Transport_Cache_Manager.h"

ACE_RCSID(BiDirectional, server, "server.cpp,v 1.5 2001/03/30 20:11:05 bala Exp")

const char *ior_output_file = 0;
static int no_iterations = 10;
static int no_clients = 2;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:i:c:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.optarg;
        break;
      case 'c':
        no_clients = ACE_OS::atoi (get_opts.optarg);
        break;
      case 'i':
        no_iterations = ACE_OS::atoi (get_opts.optarg);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "-c <no_clients>"
                           "-i <no_iterations>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}


int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Policies for the childPOA to be created.
      CORBA::PolicyList policies (1);
      policies.length (1);

      CORBA::Any pol;
      pol <<= BiDirPolicy::BOTH;
      policies[0] =
        orb->create_policy (BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE,
                            pol
                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create POA as child of RootPOA with the above policies.  This POA
      // will receive request in the same connection in which it sent
      // the request
      PortableServer::POA_var child_poa =
        root_poa->create_POA ("childPOA",
                              poa_manager.in (),
                              policies
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Creation of childPOA is over. Destroy the Policy objects.
      for (CORBA::ULong i = 0;
           i < policies.length ();
           ++i)
        {
          policies[i]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_Manual_Event manual_event;

      Sender_i *sender = 0;
      ACE_NEW_RETURN (sender,
                      Sender_i (no_clients,
                                manual_event),
                      -1);

      PortableServer::ServantBase_var owner_transfer (sender);

      PortableServer::ObjectId_var id =
        child_poa->activate_object (sender
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var obj =
        child_poa->id_to_reference (id.in ()
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        orb->object_to_string (obj.in ()
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }


      // Get the main thread id..
      ACE_thread_t thr_id = ACE_Thread::self ();

      Server_Task server_task (sender,
                               manual_event,
                               no_iterations,
                               orb.in (),
                               ACE_Thread_Manager::instance ());


      if (server_task.activate (THR_NEW_LWP | THR_JOINABLE, 4, 1) == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error activating server task\n"));
        }

      // Only the main thread gets to run the ORB
      if (thr_id == ACE_Thread::self ())
        {
          // run the ORB for at most 60 seconds...
          ACE_Time_Value tv (60, 0);

          // Call the ORB run from the main thread
          orb->run (tv ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_DEBUG ((LM_DEBUG, "(%P|%t) event loop finished\n"));
        }

      ACE_Thread_Manager::instance ()->wait ();

      root_poa->destroy (1,
                         1
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
