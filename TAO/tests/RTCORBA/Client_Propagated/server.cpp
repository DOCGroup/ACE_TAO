#include "testS.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "tao/ORB_Core.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "../check_supported_priorities.cpp"

#include <cstdio>
#include <memory>

class Test_i : public POA_Test
{
  // = TITLE
  //   An implementation for the Test interface in test.idl
  //
public:
  explicit Test_i (CORBA::ORB_ptr orb);
  // ctor

  // = The Test methods.
  virtual void test_method (CORBA::Short priority);

  //FUZZ: disable check_for_lack_ACE_OS
  virtual void shutdown ();
  //FUZZ: enable check_for_lack_ACE_OS

private:
  Test_i (Test_i const &) = delete;
  Test_i &operator= (Test_i const &) = delete;
  Test_i (Test_i &&) = delete;
  Test_i &operator= (Test_i &&) = delete;

  CORBA::ORB_var orb_;
  // The ORB
};

Test_i::Test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Test_i::test_method (CORBA::Short priority)
{
  // Use RTCurrent to find out the CORBA priority of the current
  // thread.

  CORBA::Object_var const obj =
    this->orb_->resolve_initial_references ("RTCurrent");

  RTCORBA::Current_var const current =
    RTCORBA::Current::_narrow (obj.in ());

  if (CORBA::is_nil (obj.in ()))
    throw CORBA::INTERNAL ();

  CORBA::Short const servant_thread_priority =
    current->the_priority ();

  // Print out the info.
  if (servant_thread_priority != priority)
    ACE_DEBUG ((LM_DEBUG,
                "ERROR: servant thread priority is not equal "
                "to method argument.\n"));

  ACE_DEBUG ((LM_DEBUG,
              "Client priority: %d  "
              "Servant thread priority: %d\n",
              priority, servant_thread_priority));
}

void
Test_i::shutdown ()
{
  this->orb_->shutdown (false);
}

//*************************************************************************

struct Server_Options
{
  ACE_TCHAR const *ior_output_file {ACE_TEXT("test.ior")};
};

// Parse command-line arguments.
int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
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

  return 0;
}

class Task : public ACE_Task_Base
{
public:
  Task (ACE_Thread_Manager &thread_manager,
        CORBA::ORB_ptr orb,
        Server_Options const &options);

  int svc ();

private:
  CORBA::ORB_var orb_;
  Server_Options const options_;
  Task (Task const &) = delete;
  Task &operator= (Task const &) = delete;
  Task (Task &&) = delete;
  Task &operator= (Task &&) = delete;
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
        this->orb_->resolve_initial_references("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: Panic <RootPOA> is nil\n"),
                          -1);

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      object = this->orb_->resolve_initial_references ("RTORB");
      RTCORBA::RTORB_var const rt_orb = RTCORBA::RTORB::_narrow (object.in ());

      object =
        this->orb_->resolve_initial_references ("RTCurrent");
      RTCORBA::Current_var const current =
        RTCORBA::Current::_narrow (object.in ());

      // Create POA with CLIENT_PROPAGATED PriorityModelPolicy,
      // and register Test object with it.
      CORBA::PolicyList poa_policy_list;
      poa_policy_list.length (1);
      poa_policy_list[0] =
        rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                              0);

      PortableServer::POA_var const child_poa =
        root_poa->create_POA ("Child_POA",
                              poa_manager.in (),
                              poa_policy_list);

      Test_i server_impl (this->orb_.in ());

      PortableServer::ObjectId_var id =
        child_poa->activate_object (&server_impl);

      CORBA::Object_var server =
        child_poa->id_to_reference (id.in ());

      // Print Object IOR.
      CORBA::String_var ior =
        this->orb_->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n\n", ior.in ()));

      if (this->options_.ior_output_file != nullptr)
        {
          std::unique_ptr<FILE, int (*) (FILE *)> output_file (
            ACE_OS::fopen (this->options_.ior_output_file, "w"), ACE_OS::fclose);
          if (!output_file)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               this->options_.ior_output_file),
                              -1);
          ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
        }

      // Get the initial priority of the current thread.
      // Note that it is an error to access the priority via the current in a thread that it
      // where it hasn't been set following the fix for tao681.
      CORBA::Short const initial_thread_priority =
        get_implicit_thread_CORBA_priority (this->orb_.in ());
      current->the_priority (initial_thread_priority);

      // Run ORB Event loop.
      poa_manager->activate ();

      this->orb_->run ();

      ACE_DEBUG ((LM_DEBUG, "Server ORB event loop finished\n"));

      // Get the final priority of the current thread.
      CORBA::Short const final_thread_priority =
        current->the_priority ();

      if (final_thread_priority != initial_thread_priority)
        ACE_DEBUG ((LM_DEBUG,
                    "ERROR: Priority of the servant thread "
                    "has been permanently changed!\n"
                    "Initial priority: %d  Final priority: %d\n",
                    initial_thread_priority, final_thread_priority));
      else
        ACE_DEBUG ((LM_DEBUG,
                    "Final priority of the servant thread"
                    " = its initial priority\n"));
    }
  catch (CORBA::Exception const &ex)
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
      // Standard initialization:
      // parse arguments and get all the references (ORB,
      // RootPOA, RTORB, RTCurrent, POAManager).
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      Server_Options options;
      if (parse_args (argc, argv, options) != 0)
        return -1;

      // Make sure we can support multiple priorities that are required
      // for this test.
      if (!check_supported_priorities (orb.in ()))
        return 2;

      // Thread Manager for managing task.
      ACE_Thread_Manager thread_manager;

      // Create task.
      Task task (thread_manager, orb.in (), options);

      // Task activation flags.
      long const flags =
        THR_NEW_LWP |
        THR_JOINABLE |
        orb->orb_core ()->orb_params ()->thread_creation_flags ();

      // Activate task.
      int result =
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
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
