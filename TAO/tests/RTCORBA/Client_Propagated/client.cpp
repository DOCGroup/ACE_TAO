// $Id$

#include "testC.h"
#include "Client_ORBInitializer.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "ace/Get_Opt.h"
#include "../check_supported_priorities.cpp"

const char *ior = "file://test.ior";

int
parse_args (int argc, char *argv[])
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

int
main (int argc, char *argv[])
{
  // Make sure we can support multiple priorities that are required
  // for this test.
  check_supported_priorities ();

  ACE_TRY_NEW_ENV
    {
      // Register the interceptors to check for the RTCORBA
      // service contexts in the reply messages.
      PortableInterceptor::ORBInitializer_ptr temp_initializer;

      ACE_NEW_RETURN (temp_initializer,
                      Client_ORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (initializer.in ()
                                                     TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Initialize and obtain reference to the Test object.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test_var server =
        Test::_narrow (object.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "ERROR: Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      // Check that the object is configured with CLIENT_PROPAGATED
      // PriorityModelPolicy.
      CORBA::Policy_var policy =
        server->_get_policy (RTCORBA::PRIORITY_MODEL_POLICY_TYPE
                             TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTCORBA::PriorityModelPolicy_var priority_policy =
        RTCORBA::PriorityModelPolicy::_narrow (policy.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (priority_policy.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: Priority Model Policy not exposed!\n"),
                          1);

      RTCORBA::PriorityModel priority_model =
        priority_policy->priority_model (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (priority_model != RTCORBA::CLIENT_PROPAGATED)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: priority_model != "
                           "RTCORBA::CLIENT_PROPAGATED!\n"),
                          1);

      // Make several invocation, changing the priority of this thread
      // for each.
      object =
        orb->resolve_initial_references ("RTCurrent" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      RTCORBA::Current_var current =
        RTCORBA::Current::_narrow (object.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      object = orb->resolve_initial_references ("PriorityMappingManager"
                                                TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      RTCORBA::PriorityMappingManager_var mapping_manager =
        RTCORBA::PriorityMappingManager::_narrow (object.in ()
                                              TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTCORBA::PriorityMapping *pm =
        mapping_manager->mapping ();

      int max_priority =
        ACE_Sched_Params::priority_max (ACE_SCHED_OTHER);
      int min_priority =
        ACE_Sched_Params::priority_min (ACE_SCHED_OTHER);

      CORBA::Short native_priority =
        (max_priority + min_priority) / 2;

      CORBA::Short desired_priority = 0;

      if (pm->to_CORBA (native_priority, desired_priority) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot convert native priority %d to corba priority\n",
                           native_priority),
                          1);

      for (int i = 0; i < 3; ++i)
        {
          current->the_priority (desired_priority TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CORBA::Short priority =
            current->the_priority (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (desired_priority != priority)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "ERROR: Unable to set thread "
                               "priority to %d\n", desired_priority),
                              1);


          server->test_method (priority TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          desired_priority++;
        }

      // Shut down Server ORB.
      server->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::DATA_CONVERSION, ex)
    {
      ACE_PRINT_EXCEPTION(ex,
                          "Most likely, this is due to the in-ability "
                          "to set the thread priority.");
      return 1;
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
