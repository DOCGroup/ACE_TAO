// $Id$

#include "test_i.h"
#include "ace/Get_Opt.h"
#include "tao/ORB_Core.h"
#include "ace/Task.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "../check_supported_priorities.cpp"

static const ACE_TCHAR *ior_output_file = ACE_TEXT("ior");
static int ior_count = 1;
static CORBA::ULong static_threads = 1;
static CORBA::ULong dynamic_threads = 0;
static RTCORBA::Priority low_priority;
static RTCORBA::Priority high_priority;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:s:d:t:"));
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

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s\n"
                           "\t-o <iorfile> (defaults to %s)\n"
                           "\t-s <static_threads> (defaults to %d)\n"
                           "\t-d <dynamic_threads> (defaults to %d)\n"
                           "\n",
                           argv [0],
                           ior_output_file,
                           static_threads,
                           dynamic_threads),
                          -1);
      }

  // Indicates successful parsing of the command line
  return 0;
}

int
write_ior_to_file (CORBA::ORB_ptr orb,
                   test_ptr test)
{
  CORBA::String_var ior =
    orb->object_to_string (test);

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
                                 RTCORBA::RTORB_ptr rt_orb)
{
  // Policies for the firstPOA to be created.
  CORBA::PolicyList policies (3); policies.length (3);

  // Implicit_activation policy.
  policies[0] =
    root_poa->create_implicit_activation_policy
    (PortableServer::IMPLICIT_ACTIVATION);

  // Thread pool policy.
  policies[1] =
    CORBA::Policy::_duplicate (threadpool_policy);

  // Priority Model policy.
  policies[2] =
    rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                          0);

  // Create the POA under the RootPOA.
  PortableServer::POA_var poa =
    root_poa->create_POA (poa_name,
                          poa_manager,
                          policies);

  // Creation of POAs is over. Destroy the Policy objects.
  for (CORBA::ULong i = 0;
       i < policies.length ();
       ++i)
    {
      policies[i]->destroy ();
    }

  test_i *servant =
    new test_i (orb,
                poa.in (),
                low_priority,
                high_priority);

  PortableServer::ServantBase_var safe_servant (servant);
  ACE_UNUSED_ARG (safe_servant);

  PortableServer::ObjectId_var id =
    poa->activate_object (servant);

  CORBA::Object_var object = poa->id_to_reference (id.in ());

  test_var test =
    test::_narrow (object.in ());

  int const result =
    write_ior_to_file (orb,
                       test.in ());

  return result;
}

class Task : public ACE_Task_Base
{
public:

  Task (ACE_Thread_Manager &thread_manager,
        CORBA::ORB_ptr orb);

  int svc (void);

  CORBA::ORB_var orb_;

};

Task::Task (ACE_Thread_Manager &thread_manager,
            CORBA::ORB_ptr orb)
  : ACE_Task_Base (&thread_manager),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Task::svc (void)
{
  try
    {
      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      object =
        this->orb_->resolve_initial_references ("RTORB");

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in ());

      object =
        this->orb_->resolve_initial_references ("RTCurrent");

      RTCORBA::Current_var current =
        RTCORBA::Current::_narrow (object.in ());

      RTCORBA::Priority default_thread_priority =
        get_implicit_thread_CORBA_priority (this->orb_.in ());

      low_priority = default_thread_priority;
      high_priority = default_thread_priority + 1;

      poa_manager->activate ();

      CORBA::ULong stacksize = 0;
      CORBA::Boolean allow_request_buffering = 0;
      CORBA::ULong max_buffered_requests = 0;
      CORBA::ULong max_request_buffer_size = 0;

      RTCORBA::ThreadpoolId threadpool_id_1 =
        rt_orb->create_threadpool (stacksize,
                                   static_threads,
                                   dynamic_threads,
                                   low_priority,
                                   allow_request_buffering,
                                   max_buffered_requests,
                                   max_request_buffer_size);

      CORBA::Policy_var threadpool_policy_1 =
        rt_orb->create_threadpool_policy (threadpool_id_1);

      CORBA::Boolean allow_borrowing = 0;
      RTCORBA::ThreadpoolLanes lanes (2);
      lanes.length (2);

      lanes[0].lane_priority = low_priority;
      lanes[0].static_threads = static_threads;
      lanes[0].dynamic_threads = dynamic_threads;

      lanes[1].lane_priority = high_priority;
      lanes[1].static_threads = static_threads;
      lanes[1].dynamic_threads = dynamic_threads;

      RTCORBA::ThreadpoolId threadpool_id_2 =
        rt_orb->create_threadpool_with_lanes (stacksize,
                                              lanes,
                                              allow_borrowing,
                                              allow_request_buffering,
                                              max_buffered_requests,
                                              max_request_buffer_size);

      CORBA::Policy_var threadpool_policy_2 =
        rt_orb->create_threadpool_policy (threadpool_id_2);

      int result =
        create_POA_and_register_servant (threadpool_policy_1.in (),
                                         "first_poa",
                                         poa_manager.in (),
                                         root_poa.in (),
                                         this->orb_.in (),
                                         rt_orb.in ());
      if (result != 0)
        return result;

      result =
        create_POA_and_register_servant (threadpool_policy_2.in (),
                                         "second_poa",
                                         poa_manager.in (),
                                         root_poa.in (),
                                         this->orb_.in (),
                                         rt_orb.in ());
      if (result != 0)
        return result;

      this->orb_->run ();

      this->orb_->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv);

      int result =
        parse_args (argc, argv);
      if (result != 0)
        return result;

      // Make sure we can support multiple priorities that are required
      // for this test.
      if (!check_supported_priorities (orb.in ()))
        return 2;

      // Thread Manager for managing task.
      ACE_Thread_Manager thread_manager;

      // Create task.
      Task task (thread_manager,
                 orb.in ());

      // Task activation flags.
      long flags =
        THR_NEW_LWP |
        THR_JOINABLE |
        orb->orb_core ()->orb_params ()->thread_creation_flags ();

      // Activate task.
      result =
        task.activate (flags);
      if (result == -1)
        {
          if (errno == EPERM)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Cannot create thread with scheduling policy %s\n"
                                 "because the user does not have the appropriate privileges, terminating program....\n"
                                 "Check svc.conf options and/or run as root\n",
                                 sched_policy_name (orb->orb_core ()->orb_params ()->ace_sched_policy ())),
                                2);
            }
          else
            // Unexpected error.
            ACE_ASSERT (0);
        }

      // Wait for task to exit.
      result =
        thread_manager.wait ();
      ACE_ASSERT (result != -1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
