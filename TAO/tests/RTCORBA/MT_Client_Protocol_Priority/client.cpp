#include "testC.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/Barrier.h"
#include "tao/ORB_Core.h"
#include "tao/Policy_ManagerC.h"
#include "tao/Policy_CurrentC.h"
#include "../check_supported_priorities.cpp"
#include "tao/Strategies/advanced_resource.h"

#include <cstdio>

class Worker_Thread : public ACE_Task_Base
{
  // = TITLE
  //   Runs one client worker thread.
  //
  // = DESCRIPTION
  //   Sets ClientProtocolPolicy override to the specified value on
  //   the Current level, sets thread priority to the specified value
  //   and waits on barrier for other worker threads.  Then loops
  //   making invocations on the specified server object.
  //
public:
  Worker_Thread (CORBA::ORB_ptr orb,
                 Test_ptr server,
                 CORBA::ULong protocol_type,
                 int iterations,
                 ACE_Barrier *barrier);
  Worker_Thread (Worker_Thread const &) = delete;
  Worker_Thread (Worker_Thread &&) = delete;
  Worker_Thread &operator= (Worker_Thread const &) = delete;
  Worker_Thread &operator= (Worker_Thread &&) = delete;

  int svc () override;
  // Do work.

private:
  CORBA::ORB_ptr const orb_;
  // Borrowed ORB reference. The owning Task outlives this worker.

  Test_ptr const server_;
  // Borrowed server reference. The owning Task outlives this worker.

  CORBA::ULong protocol_type_;
  // Protocol for ClientProtocolPolicy.

  int iterations_;
  // Number of server invocations to make.

  ACE_Barrier * const synchronizer_;
  // Mechanism for synchronization with other worker threads.
};

// ****************************************************************
struct Client_Options
{
  ACE_TCHAR const *ior { ACE_TEXT("file://test.ior") };
  int iterations { 100 };
  CORBA::Short priority1 { -1 };
  CORBA::Short priority2 { -1 };
  CORBA::ULong protocol1 { 1413566210 };
  CORBA::ULong protocol2 {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:a:b:e:f:n:"));
  int c {};
  int result {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'o':
        options.ior = get_opts.opt_arg ();
        break;
      case 'a':
        result = std::sscanf (ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ()),
                              "%hd",
                              &options.priority1);
        if (result == 0 || result == EOF)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to process <-a> option"),
                            -1);
        break;
      case 'b':
        result = std::sscanf (ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ()),
                              "%hd",
                              &options.priority2);
        if (result == 0 || result == EOF)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to process <-b> option"),
                            -1);
        break;
      case 'e':
        result = std::sscanf (ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ()),
                              "%u",
                              &options.protocol1);
        if (result == 0 || result == EOF)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to process <-e> option"),
                            -1);
        break;
      case 'f':
        result = std::sscanf (ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ()),
                              "%u",
                              &options.protocol2);
        if (result == 0 || result == EOF)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to process <-f> option"),
                            -1);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <ior> "
                           "-a <priority1> "
                           "-b <priority2> "
                           "-e <protocol_type1> "
                           "-f <protocol_type2> "
                           "-n <number_of_iterations> "
                           "\n",
                           argv [0]),
                          -1);
      }

  if (options.priority1 < 0
      || options.priority2 < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Valid thread priorities must be specified.\n"
                       "See README for command-line options.\n"),
                      -1);
  return 0;
}

int
check_for_nil (CORBA::Object_ptr obj, char const *msg)
{
  if (CORBA::is_nil (obj))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ERROR: Object reference <%C> is nil\n",
                       msg),
                      -1);

  return 0;
}

class Task : public ACE_Task_Base
{
public:
  Task (ACE_Thread_Manager &thread_manager,
        CORBA::ORB_ptr orb,
        Client_Options const &options);
  Task (Task const &) = delete;
  Task (Task &&) = delete;
  Task &operator= (Task const &) = delete;
  Task &operator= (Task &&) = delete;

  int svc () override;

private:
  CORBA::ORB_var const orb_;
  Client_Options const options_;
};

Task::Task (ACE_Thread_Manager &thread_manager,
            CORBA::ORB_ptr orb,
            Client_Options const &options)
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
      // Priority Mapping Manager.
      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("PriorityMappingManager");
      RTCORBA::PriorityMappingManager_var const mapping_manager =
        RTCORBA::PriorityMappingManager::_narrow (object.in ());
      if (check_for_nil (mapping_manager.in (), "Mapping Manager") == -1)
        return -1;

      // The mapping manager owns this mapping object.
      RTCORBA::PriorityMapping * const pm =
        mapping_manager->mapping ();

      // RTCurrent.
      object =
        this->orb_->resolve_initial_references ("RTCurrent");
      RTCORBA::Current_var const current =
        RTCORBA::Current::_narrow (object.in ());
      if (check_for_nil (current.in (), "RTCurrent") == -1)
        return -1;

      // Obtain Test object reference.
      object =
        this->orb_->string_to_object (this->options_.ior);
      Test_var const server = Test::_narrow (object.in ());
      if (check_for_nil (server.in (), "Test object") == -1)
        return -1;

      // Check that test object is configured with CLIENT_PROPAGATED
      // PriorityModelPolicy.
      CORBA::Policy_var policy =
        server->_get_policy (RTCORBA::PRIORITY_MODEL_POLICY_TYPE);

      RTCORBA::PriorityModelPolicy_var const priority_policy =
        RTCORBA::PriorityModelPolicy::_narrow (policy.in ());

      if (check_for_nil (priority_policy.in (), "PriorityModelPolicy") == -1)
        return -1;

      RTCORBA::PriorityModel const priority_model =
        priority_policy->priority_model ();
      if (priority_model != RTCORBA::CLIENT_PROPAGATED)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: priority_model != "
                           "RTCORBA::CLIENT_PROPAGATED!\n"),
                          -1);

      // Spawn two worker threads.
      ACE_Barrier thread_barrier (2);
      long const flags =
        THR_NEW_LWP |
        THR_JOINABLE |
        this->orb_->orb_core ()->orb_params ()->thread_creation_flags ();

      // Worker 1.
      Worker_Thread worker1 (this->orb_.in (),
                             server.in (),
                             this->options_.protocol1,
                             this->options_.iterations,
                             &thread_barrier);

      CORBA::Short native_priority1 {};
      if (pm->to_native (this->options_.priority1, native_priority1) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot convert corba priority %d to native priority\n",
                           this->options_.priority1),
                          -1);

      if (worker1.activate (flags,
                            1, 0,
                            native_priority1) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate first client worker threads\n"),
                          -1);

      // Worker 2.
      Worker_Thread worker2 (this->orb_.in (),
                             server.in (),
                             this->options_.protocol2,
                             this->options_.iterations,
                             &thread_barrier);

      CORBA::Short native_priority2 {};
      if (pm->to_native (this->options_.priority2, native_priority2) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot convert corba priority %d to native priority\n",
                           this->options_.priority2),
                          -1);

      if (worker2.activate (flags,
                            1, 0,
                            native_priority2) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate second client worker threads\n"),
                          -1);

      // Wait for worker threads to finish.
      ACE_Thread_Manager::instance ()->wait ();

      // Testing over.  Shut down the server.
      ACE_DEBUG ((LM_DEBUG, "Client threads finished\n"));
      current->the_priority (this->options_.priority1);
      server->shutdown ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (
        "Unexpected exception in MT_Client_Protocol_Priority test client:");
      return -1;
    }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      Client_Options options;

      // Initialize the ORB, resolve references and parse arguments.

      // ORB.
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      // Parse arguments.
      if (parse_args (argc, argv, options) != 0)
        return -1;

      // Make sure we can support multiple priorities that are required
      // for this test.
      if (!check_supported_priorities (orb.in ()))
        return 2;

      // Thread Manager for managing task.
      ACE_Thread_Manager thread_manager;

      // Create task.
      Task task (thread_manager,
                 orb.in (),
                 options);

      // Task activation flags.
      long const flags =
        THR_NEW_LWP |
        THR_JOINABLE |
        orb->orb_core ()->orb_params ()->thread_creation_flags ();

      // Activate task.
      int const activate_result =
        task.activate (flags);
      if (activate_result == -1)
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
      int const wait_result =
        thread_manager.wait ();
      ACE_ASSERT (wait_result != -1);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (
        "Unexpected exception in MT_Client_Protocol_Priority test client:");
      return -1;
    }

  return 0;
}

// ****************************************************************

Worker_Thread::Worker_Thread (CORBA::ORB_ptr orb,
                              Test_ptr server,
                              CORBA::ULong protocol_type,
                              int iterations,
                              ACE_Barrier *thread_barrier)
  : orb_ (orb),
    server_ (server),
    protocol_type_ (protocol_type),
    iterations_ (iterations),
    synchronizer_ (thread_barrier)
{
}

int
Worker_Thread::svc ()
{
  try
    {
      // RTORB.
      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RTORB");
      RTCORBA::RTORB_var const rt_orb = RTCORBA::RTORB::_narrow (object.in ());
      if (check_for_nil (rt_orb.in (), "RTORB") == -1)
        return 0;

      // PolicyCurrent.
      object =
        this->orb_->resolve_initial_references ("PolicyCurrent");
      CORBA::PolicyCurrent_var const policy_current =
        CORBA::PolicyCurrent::_narrow (object.in ());
      if (check_for_nil (policy_current.in (), "PolicyCurrent")
          == -1)
        return 0;

      object =
        this->orb_->resolve_initial_references ("RTCurrent");

      RTCORBA::Current_var const current =
        RTCORBA::Current::_narrow (object.in ());

      // We need to set the client thread CORBA priority
      current->the_priority (get_implicit_thread_CORBA_priority (this->orb_));

      // Set ClientProtocolPolicy override on the Current.
      RTCORBA::ProtocolList protocols;
      protocols.length (1);
      protocols[0].protocol_type = this->protocol_type_;
      protocols[0].transport_protocol_properties =
        RTCORBA::ProtocolProperties::_nil ();
      protocols[0].orb_protocol_properties =
        RTCORBA::ProtocolProperties::_nil ();

      CORBA::PolicyList policy_list;
      policy_list.length (1);
      policy_list[0] =
        rt_orb->create_client_protocol_policy (protocols);

      policy_current->set_policy_overrides (policy_list,
                                            CORBA::SET_OVERRIDE);

      // Wait for other threads.
      this->synchronizer_->wait ();

      for (int i = 0; i < this->iterations_; ++i)
        {
          // Invoke method.
          this->server_->test_method ();
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Worker Thread exception:");
    }
  return 0;
}
