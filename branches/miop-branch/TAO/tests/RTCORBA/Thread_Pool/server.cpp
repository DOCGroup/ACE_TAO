// $Id$

#include "test_i.h"
#include "ace/Get_Opt.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "../check_supported_priorities.cpp"

ACE_RCSID(Thread_Pool, server, "$Id$")

const char *ior_output_file = "ior";
int ior_count = 1;
CORBA::ULong static_threads = 2;
CORBA::ULong dynamic_threads = 2;
long nap_time = 1000;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:s:d:t:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case 's':
        static_threads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'd':
        dynamic_threads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 't':
        nap_time = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile> "
                           "-s <static_threads> "
                           "-d <dynamic_threads> "
                           "-t <nap_time> "
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates sucessful parsing of the command line
  return 0;
}

int
write_ior_to_file (CORBA::ORB_ptr orb,
                   test_ptr test
                   TAO_ENV_ARG_DECL)
{
  CORBA::String_var ior =
    orb->object_to_string (test
                           TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  char filename[BUFSIZ];
  ACE_OS::sprintf (filename,
                   "%s_%d",
                   ior_output_file,
                   ior_count++);

  FILE *output_file =
    ACE_OS::fopen (filename,
                   "w");

  if (output_file == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Cannot open output file for writing IOR: %s",
                       filename),
                      -1);

  ACE_OS::fprintf (output_file,
                   "%s",
                   ior.in ());

  ACE_OS::fclose (output_file);

  return 0;
}

int
create_POA_and_register_servant (CORBA::Policy_ptr threadpool_policy,
                                 const char *poa_name,
                                 PortableServer::POAManager_ptr poa_manager,
                                 PortableServer::POA_ptr root_poa,
                                 CORBA::ORB_ptr orb,
                                 RTCORBA::RTORB_ptr rt_orb
                                 TAO_ENV_ARG_DECL)
{
  // Policies for the firstPOA to be created.
  CORBA::PolicyList policies (3); policies.length (3);

  // Implicit_activation policy.
  policies[0] =
    root_poa->create_implicit_activation_policy
      (PortableServer::IMPLICIT_ACTIVATION
       TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Thread pool policy.
  policies[1] =
    CORBA::Policy::_duplicate (threadpool_policy);

  // Priority Model policy.
  policies[2] =
    rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                          0
                                          TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Create the POA under the RootPOA.
  PortableServer::POA_var poa =
    root_poa->create_POA (poa_name,
                          poa_manager,
                          policies
                          TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Creation of POAs is over. Destroy the Policy objects.
  for (CORBA::ULong i = 0;
       i < policies.length ();
       ++i)
    {
      policies[i]->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }

  test_i *servant =
    new test_i (orb,
                poa.in (),
                nap_time);

  PortableServer::ServantBase_var safe_servant (servant);
  ACE_UNUSED_ARG (safe_servant);

  test_var test =
    servant->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  int result =
    write_ior_to_file (orb,
                       test.in ()
                       TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return result;
}

int
main (int argc, char *argv[])
{
  // Make sure we can support multiple priorities that are required
  // for this test.
  check_supported_priorities ();

  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         ""
                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA"
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ()
                                      TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      object =
        orb->resolve_initial_references ("RTORB"
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in ()
                                 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      object =
        orb->resolve_initial_references ("RTCurrent"
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTCORBA::Current_var current =
        RTCORBA::Current::_narrow (object.in ()
                                   TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTCORBA::Priority default_thread_priority =
        current->the_priority (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int result =
        parse_args (argc, argv);
      if (result != 0)
        return result;

      test_i servant (orb.in (),
                      root_poa.in (),
                      nap_time);
      test_var test =
        servant._this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      result =
        write_ior_to_file (orb.in (),
                           test.in ()
                           TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (result != 0)
        return result;

      poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::ULong stacksize = 0;
      CORBA::Boolean allow_request_buffering = 0;
      CORBA::ULong max_buffered_requests = 0;
      CORBA::ULong max_request_buffer_size = 0;

      RTCORBA::ThreadpoolId threadpool_id_1 =
        rt_orb->create_threadpool (stacksize,
                                   static_threads,
                                   dynamic_threads,
                                   default_thread_priority,
                                   allow_request_buffering,
                                   max_buffered_requests,
                                   max_request_buffer_size
                                   TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Policy_var threadpool_policy_1 =
        rt_orb->create_threadpool_policy (threadpool_id_1
                                          TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Boolean allow_borrowing = 0;
      RTCORBA::ThreadpoolLanes lanes (2);
      lanes.length (2);

      lanes[0].lane_priority = default_thread_priority;
      lanes[0].static_threads = static_threads;
      lanes[0].dynamic_threads = dynamic_threads;

      lanes[1].lane_priority = default_thread_priority;
      lanes[1].static_threads = static_threads * 2;
      lanes[1].dynamic_threads = dynamic_threads * 2;

      RTCORBA::ThreadpoolId threadpool_id_2 =
        rt_orb->create_threadpool_with_lanes (stacksize,
                                              lanes,
                                              allow_borrowing,
                                              allow_request_buffering,
                                              max_buffered_requests,
                                              max_request_buffer_size
                                              TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Policy_var threadpool_policy_2 =
        rt_orb->create_threadpool_policy (threadpool_id_2
                                          TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      result =
        create_POA_and_register_servant (threadpool_policy_1.in (),
                                         "first_poa",
                                         poa_manager.in (),
                                         root_poa.in (),
                                         orb.in (),
                                         rt_orb.in ()
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (result != 0)
        return result;

      result =
        create_POA_and_register_servant (threadpool_policy_2.in (),
                                         "second_poa",
                                         poa_manager.in (),
                                         root_poa.in (),
                                         orb.in (),
                                         rt_orb.in ()
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (result != 0)
        return result;

      orb->run (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
