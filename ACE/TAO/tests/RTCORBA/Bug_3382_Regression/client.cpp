// $Id$

#include "testC.h"
#include "ace/Task.h"
#include "tao/ORB_Core.h"
#include "Client_ORBInitializer.h"
#include "tao/ORBInitializer_Registry.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "ace/Get_Opt.h"
#include "../check_supported_priorities.cpp"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
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

  int result = 0;
  try
    {
      CORBA::Object_var object =
        this->orb_->string_to_object (ior);

      Test_var server =
        Test::_narrow (object.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "ERROR: Object reference <%s> is nil\n",
                             ior),
                            -1);
        }

      // Check that the object is configured with CLIENT_PROPAGATED
      // PriorityModelPolicy.
      CORBA::Policy_var policy =
        server->_get_policy (RTCORBA::PRIORITY_MODEL_POLICY_TYPE);

      RTCORBA::PriorityModelPolicy_var priority_policy =
        RTCORBA::PriorityModelPolicy::_narrow (policy.in ());

      if (CORBA::is_nil (priority_policy.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: Priority Model Policy not exposed!\n"),
                          -1);

      RTCORBA::PriorityModel priority_model =
        priority_policy->priority_model ();

      if (priority_model != RTCORBA::CLIENT_PROPAGATED)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: priority_model != "
                           "RTCORBA::CLIENT_PROPAGATED!\n"),
                          -1);

      // Make several invocations, changing the priority of this thread
      // for each.
      object =
        this->orb_->resolve_initial_references ("RTCurrent");

      RTCORBA::Current_var current =
        RTCORBA::Current::_narrow (object.in ());

      object = this->orb_->resolve_initial_references ("PriorityMappingManager");

      RTCORBA::PriorityMappingManager_var mapping_manager =
        RTCORBA::PriorityMappingManager::_narrow (object.in ());

      RTCORBA::PriorityMapping *pm =
        mapping_manager->mapping ();

      int sched_policy =
        this->orb_->orb_core ()->orb_params ()->ace_sched_policy ();

      int max_priority =
        ACE_Sched_Params::priority_max (sched_policy);
      int min_priority =
        ACE_Sched_Params::priority_min (sched_policy);

      CORBA::Short native_priority =
        (max_priority - min_priority) / 2;

      CORBA::Short desired_priority = 0;

      for (int i = 0; i < 3; ++i)
        {
          if (pm->to_CORBA (native_priority, desired_priority) == 0)
            {
              ACE_ERROR ((LM_ERROR,
                           "ERROR: Cannot convert native priority %d to corba priority\n",
                           native_priority));
              result = -1;
              break;
            }

          current->the_priority (desired_priority);

          CORBA::Short priority =
            current->the_priority ();

          if (desired_priority != priority)
            {
              ACE_ERROR ((LM_ERROR,
                               "ERROR: No exception setting the priority but mismatch between requested and returned value from Current. "
                               "Set to %d but Current::the_priority returns %d\n", desired_priority, priority));
              result = -1;
            }


          server->test_method (priority);

          native_priority++;
        }

      // Shut down Server ORB.
      server->shutdown ();
    }
  catch (const CORBA::DATA_CONVERSION& ex)
    {
      ex._tao_print_exception (
                          "Most likely, this is due to the in-ability "
                          "to set the thread priority.");
      return -1;
    }
  catch (const CORBA::Exception & ae)
    {
      ae._tao_print_exception (
                           "Caught exception:");
      return -1;
    }

  return result;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int result = 0;
  try
    {
      // Register the interceptors to check for the RTCORBA
      // service contexts in the reply messages.
      PortableInterceptor::ORBInitializer_ptr temp_initializer;

      ACE_NEW_RETURN (temp_initializer,
                      Client_ORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (initializer.in ());

      // Initialize and obtain reference to the Test object.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return -1;

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
            ACE_ERROR_RETURN ((LM_ERROR,
                                 "ERROR: Cannot create thread. errno = %d\n",
                                 ACE_ERRNO_GET),
                                -1);
        }

      // Wait for task to exit.
      result =
        thread_manager.wait ();
    }
  catch (const CORBA::Exception & ae)
    {
      ae._tao_print_exception (
                           "Caught exception:");
      return -1;
    }

  return result;
}
