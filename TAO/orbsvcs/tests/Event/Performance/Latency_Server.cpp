// $Id$

#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Default_Factory.h"

#include "tao/Messaging/Messaging.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/PortableServer/PortableServer.h"
#include "ace/High_Res_Timer.h"
#include "ace/Get_Opt.h"
#include "ace/Sample_History.h"
#include "ace/Sched_Params.h"
#include "ace/OS_NS_errno.h"



const ACE_TCHAR *ior_file_name = ACE_TEXT("ec.ior");

/// Parse the arguments.
static int parse_args (int argc, ACE_TCHAR *argv[]);

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  TAO_EC_Default_Factory::init_svcs ();

  int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
     + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
  priority = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
                                                  priority);
  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.

  if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                              priority,
                                              ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "server (%P|%t): user is not superuser, "
                      "test runs in time-shared class\n"));
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "server (%P|%t): sched_params failed\n"));
    }

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

#if (TAO_HAS_CORBA_MESSAGING == 1)
      CORBA::Object_var manager_object =
        orb->resolve_initial_references ("ORBPolicyManager");

      CORBA::PolicyManager_var policy_manager =
        CORBA::PolicyManager::_narrow (manager_object.in ());

      CORBA::Any sync_scope;
      sync_scope <<= Messaging::SYNC_WITH_TARGET;

      CORBA::PolicyList policy_list (1);
      policy_list.length (1);
      policy_list[0] =
        orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                            sync_scope);
      policy_manager->set_policy_overrides (policy_list,
                                            CORBA::SET_OVERRIDE);
#else
      ACE_DEBUG ((LM_DEBUG,
                  "CORBA Messaging disabled in this configuration,"
                  " test may not be optimally configured\n"));
#endif /* TAO_HAS_MESSAGING */

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      if (parse_args (argc, argv) != 0)
        return 1;

      TAO_EC_Event_Channel_Attributes attr (root_poa.in (),
                                            root_poa.in ());
      TAO_EC_Event_Channel ec_impl (attr);
      ec_impl.activate ();

      RtecEventChannelAdmin::EventChannel_var ec =
        ec_impl._this ();

      CORBA::String_var str =
        orb->object_to_string (ec.in ());

      if (ior_file_name != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_file_name, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ior_file_name),
                              1);
          ACE_OS::fprintf (output_file, "%s", str.in ());
          ACE_OS::fclose (output_file);
        }

      orb->run ();

      PortableServer::ObjectId_var id =
        root_poa->servant_to_id (&ec_impl);
      root_poa->deactivate_object (id.in ());

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (argv[0]);
    }
  return 0;
}

// ****************************************************************

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_file_name = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <ior_file_name>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}
