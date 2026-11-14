#include "testS.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "tao/ORB_Core.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "tao/Strategies/advanced_resource.h"
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
  Test_i (Test_i const &) = delete;
  Test_i (Test_i &&) = delete;
  Test_i &operator= (Test_i const &) = delete;
  Test_i &operator= (Test_i &&) = delete;
  // ctor

  void test_method () override;

  //FUZZ: disable check_for_lack_ACE_OS
  void shutdown () override;
  //FUZZ: enable check_for_lack_ACE_OS

private:
  CORBA::ORB_var const orb_;
  // The ORB
};

Test_i::Test_i (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Test_i::test_method ()
{
  ACE_DEBUG ((LM_DEBUG, "test_method invoked\n"));
}

void
Test_i::shutdown ()
{
  this->orb_->shutdown (false);
}

//*************************************************************************

struct Server_Options
{
  ACE_TCHAR const *ior_output_file { ACE_TEXT("test.ior") };
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
        Server_Options const &options);
  Task (Task const &) = delete;
  Task (Task &&) = delete;
  Task &operator= (Task const &) = delete;
  Task &operator= (Task &&) = delete;

  int svc () override;

private:
  CORBA::ORB_var const orb_;
  Server_Options const options_;
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
      // RTORB.
      CORBA::Object_var object =
        this->orb_->resolve_initial_references ("RTORB");
      RTCORBA::RTORB_var const rt_orb = RTCORBA::RTORB::_narrow (object.in ());
      if (check_for_nil (rt_orb.in (), "RTORB") == -1)
        return -1;

      // RootPOA.
      object =
        this->orb_->resolve_initial_references("RootPOA");
      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (object.in ());
      if (check_for_nil (root_poa.in (), "RootPOA") == -1)
        return -1;

      // POAManager.
      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      // Create POA with CLIENT_PROPAGATED PriorityModelPolicy.
      CORBA::PolicyList poa_policy_list;
      poa_policy_list.length (1);
      poa_policy_list[0] =
        rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                              0);

      PortableServer::POA_var const child_poa =
        root_poa->create_POA ("Child_POA",
                              poa_manager.in (),
                              poa_policy_list);

      // Create object.
      Test_i server_impl (this->orb_.in ());

      PortableServer::ObjectId_var const id =
        child_poa->activate_object (&server_impl);

      CORBA::Object_var const server =
        child_poa->id_to_reference (id.in ());

      // Print Object IOR.
      CORBA::String_var const ior =
        this->orb_->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n\n", ior.in ()));

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

      // Activate POA manager.
      poa_manager->activate ();

      // Start ORB event loop.
      this->orb_->run ();

      ACE_DEBUG ((LM_DEBUG, "Server ORB event loop finished\n\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (
        "Unexpected exception caught in MT_Client_Protocol_Priority test server:");
      return -1;
    }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      Server_Options options;

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
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
