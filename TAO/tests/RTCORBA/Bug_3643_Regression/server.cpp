#include "test_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "tao/ORB_Core.h"
#include "ace/Task.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "ace/OS_NS_stdio.h"
#include "../check_supported_priorities.cpp"

#include <memory>

struct Server_Options
{
  ACE_TCHAR const *ior_output_file {ACE_TEXT ("ior")};
  CORBA::ULong static_threads {1};
  CORBA::ULong dynamic_threads {25};
  long nap_time {1000};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:s:d:t:h"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;

      case 's':
        options.static_threads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'd':
        options.dynamic_threads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 't':
        options.nap_time = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'h':
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

  // Indicates successful parsing of the command line
  return 0;
}

int
write_ior_to_file (CORBA::ORB_ptr orb,
                   test_ptr test,
                   ACE_TCHAR const *ior_output_file,
                   int &ior_count)
{
  CORBA::String_var const ior =
    orb->object_to_string (test);

  ACE_TCHAR filename[BUFSIZ] {};
  ACE_OS::sprintf (filename,
                   ACE_TEXT ("%s_%d"),
                   ior_output_file,
                   ior_count++);

  std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
    ACE_OS::fopen (filename, ACE_TEXT ("w")), ACE_OS::fclose);
  if (!output_file)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                       filename),
                      -1);

  ACE_OS::fprintf (output_file.get (), "%s", ior.in ());

  return 0;
}

int
create_POA_and_register_servant (CORBA::Policy_ptr threadpool_policy,
                                 const char *poa_name,
                                 PortableServer::POAManager_ptr poa_manager,
                                 PortableServer::POA_ptr root_poa,
                                 CORBA::ORB_ptr orb,
                                 RTCORBA::RTORB_ptr rt_orb,
                                 Server_Options const &options,
                                 int &ior_count)
{
  // Policies for the firstPOA to be created.
  CORBA::PolicyList policies;
  policies.length (3);

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
  PortableServer::POA_var const poa =
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
                options.nap_time);

  PortableServer::ServantBase_var safe_servant (servant);
  ACE_UNUSED_ARG (safe_servant);

  PortableServer::ObjectId_var const id =
    poa->activate_object (servant);

  CORBA::Object_var const object = poa->id_to_reference (id.in ());

  test_var const test =
    test::_narrow (object.in ());

  int result =
    write_ior_to_file (orb,
                       test.in (),
                       options.ior_output_file,
                       ior_count);

  return result;
}

class Task : public ACE_Task_Base
{
public:
  Task (ACE_Thread_Manager &thread_manager,
        CORBA::ORB_ptr orb,
        Server_Options const &options);
  Task (Task const &) = delete;
  Task (Task &&) = delete;
  Task &operator= (Task const &) = delete;
  Task &operator= (Task &&) = delete;

  int svc () override;

private:
  CORBA::ORB_var const orb_;
  Server_Options const options_;
  int ior_count_ {1};
};

Task::Task (ACE_Thread_Manager &thread_manager,
            CORBA::ORB_ptr orb,
            Server_Options const &options)
  : ACE_Task_Base (&thread_manager),
    orb_ (CORBA::ORB::_duplicate (orb)),
    options_ (options)
{
}

int
Task::svc ()
{
  try
    {
      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      object =
        this->orb_->resolve_initial_references ("RTORB");

      RTCORBA::RTORB_var const rt_orb =
        RTCORBA::RTORB::_narrow (object.in ());

      RTCORBA::Priority const default_thread_priority =
        get_implicit_thread_CORBA_priority (this->orb_.in ());

      test_i servant (this->orb_.in (), root_poa.in (), this->options_.nap_time);
      PortableServer::ObjectId_var const id =
        root_poa->activate_object (&servant);

      CORBA::Object_var const object_act = root_poa->id_to_reference (id.in ());

      test_var const test =
        test::_narrow (object_act.in ());

      int result =
        write_ior_to_file (this->orb_.in (),
                           test.in (),
                           this->options_.ior_output_file,
                           this->ior_count_);

      if (result != 0)
        return result;

      poa_manager->activate ();

      CORBA::ULong const stacksize {};
      bool const allow_request_buffering {};
      CORBA::ULong const max_buffered_requests {};
      CORBA::ULong const max_request_buffer_size {};

      RTCORBA::ThreadpoolId const threadpool_id_1 =
        rt_orb->create_threadpool (stacksize,
                                   this->options_.static_threads,
                                   this->options_.dynamic_threads,
                                   default_thread_priority,
                                   allow_request_buffering,
                                   max_buffered_requests,
                                   max_request_buffer_size);

      CORBA::Policy_var const threadpool_policy_1 =
        rt_orb->create_threadpool_policy (threadpool_id_1);

      bool const allow_borrowing {};
      RTCORBA::ThreadpoolLanes lanes (1);
      lanes.length (1);

      lanes[0].lane_priority = default_thread_priority;
      lanes[0].static_threads = this->options_.static_threads;
      lanes[0].dynamic_threads = this->options_.dynamic_threads;

      RTCORBA::ThreadpoolId const threadpool_id_2 =
        rt_orb->create_threadpool_with_lanes (stacksize,
                                              lanes,
                                              allow_borrowing,
                                              allow_request_buffering,
                                              max_buffered_requests,
                                              max_request_buffer_size);

      CORBA::Policy_var const threadpool_policy_2 =
        rt_orb->create_threadpool_policy (threadpool_id_2);

      result =
        create_POA_and_register_servant (threadpool_policy_1.in (),
                                         "first_poa",
                                         poa_manager.in (),
                                         root_poa.in (),
                                         this->orb_.in (),
                                         rt_orb.in (),
                                         this->options_,
                                         this->ior_count_);
      if (result != 0)
        return result;

      result =
        create_POA_and_register_servant (threadpool_policy_2.in (),
                                         "second_poa",
                                         poa_manager.in (),
                                         root_poa.in (),
                                         this->orb_.in (),
                                         rt_orb.in (),
                                         this->options_,
                                         this->ior_count_);
      if (result != 0)
        return result;

      this->orb_->run ();

      this->orb_->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      Server_Options options;
      int result = parse_args (argc, argv, options);
      if (result != 0)
        return result;

      // Make sure we can support multiple priorities that are required
      // for this test.
      if (!check_supported_priorities (orb.in ()))
        return 2;

      // Thread Manager for managing task.
      ACE_Thread_Manager thread_manager;

      // Create task.
      Task task (thread_manager, orb.in (), options);

      // Task activation flags.
      long const flags = THR_NEW_LWP |
                         THR_JOINABLE |
                         orb->orb_core ()->orb_params ()->thread_creation_flags ();

      // Activate task.
      result =
        task.activate (flags);
      if (result == -1)
        {
          if (errno == EPERM)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot create thread with scheduling policy %s\n"
                               "because the user does not have the appropriate privileges, terminating program....\n"
                               "Check svc.conf options and/or run as root\n",
                               sched_policy_name (orb->orb_core ()->orb_params ()->ace_sched_policy ())),
                              2);
          else
            // Unexpected error.
            ACE_ASSERT (0);
        }

      // Wait for task to exit.
      result =
        thread_manager.wait ();
      ACE_ASSERT (result != -1);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
