// $Id$

#include "testS.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "tao/ORB_Core.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "../check_supported_priorities.cpp"

class Test_i : public POA_Test
{
  // = TITLE
  //   An implementation for the Test interface in test.idl
  //
public:
  Test_i (CORBA::ORB_ptr orb);
  // ctor

  // = The Test methods.
  void test_method (CORBA::Short priority
                    ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
  // The ORB
};

Test_i::Test_i (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Test_i::test_method (CORBA::Short priority
                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Use RTCurrent to find out the CORBA priority of the current
  // thread.

  CORBA::Object_var obj =
    this->orb_->resolve_initial_references ("RTCurrent" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  RTCORBA::Current_var current =
    RTCORBA::Current::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (obj.in ()))
    ACE_THROW (CORBA::INTERNAL ());

  CORBA::Short servant_thread_priority =
    current->the_priority (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

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
Test_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}

//*************************************************************************

const char *ior_output_file = "test.ior";

// Parse command-line arguments.
int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
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
  ACE_TRY_NEW_ENV
    {
      CORBA::Object_var object =
        this->orb_->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: Panic <RootPOA> is nil\n"),
                          -1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      object = this->orb_->resolve_initial_references ("RTORB" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (object.in ()
                                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      object =
        this->orb_->resolve_initial_references ("RTCurrent" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      RTCORBA::Current_var current =
        RTCORBA::Current::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create POA with CLIENT_PROPAGATED PriorityModelPolicy,
      // and register Test object with it.
      CORBA::PolicyList poa_policy_list;
      poa_policy_list.length (1);
      poa_policy_list[0] =
        rt_orb->create_priority_model_policy (RTCORBA::CLIENT_PROPAGATED,
                                              0
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var child_poa =
        root_poa->create_POA ("Child_POA",
                              poa_manager.in (),
                              poa_policy_list
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test_i server_impl (this->orb_.in ());

      PortableServer::ObjectId_var id =
        child_poa->activate_object (&server_impl ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var server =
        child_poa->id_to_reference (id.in ()
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Print Object IOR.
      CORBA::String_var ior =
        this->orb_->object_to_string (server.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n\n", ior.in ()));

      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ior_output_file),
                              -1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      // Get the initial priority of the current thread.
      CORBA::Short initial_thread_priority =
        current->the_priority (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Run ORB Event loop.
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Server ORB event loop finished\n"));

      // Get the final priority of the current thread.
      CORBA::Short final_thread_priority =
        current->the_priority (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Standard initialization:
      // parse arguments and get all the references (ORB,
      // RootPOA, RTORB, RTCurrent, POAManager).
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return -1;

      // Make sure we can support multiple priorities that are required
      // for this test.
      check_supported_priorities (orb.in());

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
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
