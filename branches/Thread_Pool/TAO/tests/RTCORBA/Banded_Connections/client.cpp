// $Id$

#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "tao/RTCORBA/RTCORBA.h"

const char *ior1 = "file://test1.ior";
const char *ior2 = "file://test2.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "n:o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':
        ior1 = get_opts.optarg;
        break;
      case 'o':
        ior2 = get_opts.optarg;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-n <ior> "
                           "-o <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

CORBA::Short
get_server_priority (Test_ptr server,
                     CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Policy_var policy =
    server->_get_policy (RTCORBA::PRIORITY_MODEL_POLICY_TYPE,
                         ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  RTCORBA::PriorityModelPolicy_var priority_policy =
    RTCORBA::PriorityModelPolicy::_narrow (policy.in (), ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  RTCORBA::PriorityModel priority_model =
    priority_policy->priority_model (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  if (priority_model != RTCORBA::SERVER_DECLARED)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ERROR: priority_model != "
                       "RTCORBA::SERVER_DECLARED!\n"),
                      -1);

  return priority_policy->server_priority (ACE_TRY_ENV);
}

void
invocation_exception_test (Test_ptr obj,
                           CORBA::Short priority,
                           CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      obj->test_method (1,
                        priority,
                        ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "ERROR: no exception caught\n"));
    }
  ACE_CATCH (CORBA::INV_POLICY, ex)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "INV_POLICY exception is caught as expected.\n"));
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG, "Error: unexpected exception caught\n"));
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // First check that we have sufficient priority range to run the
      // test, i.e., more than 1 priority level.
      int max_priority =
        ACE_Sched_Params::priority_max (ACE_SCHED_OTHER);
      int min_priority =
        ACE_Sched_Params::priority_min (ACE_SCHED_OTHER);

      if (max_priority == min_priority)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Not enough priority levels on this platform"
                      "to run the test, aborting\n"));
          return 0;
        }

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         "",
                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Parse arguments.
      int result =
        parse_args (argc,
                    argv);
      if (result != 0)
        return result;

      // RTORB.
      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB",
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      RTCORBA::RTORB_var rt_orb =
        RTCORBA::RTORB::_narrow (object.in (),
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // RTCurrent.
      object =
        orb->resolve_initial_references ("RTCurrent",
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      RTCORBA::Current_var current =
        RTCORBA::Current::_narrow (object.in (),
                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Test object 1 (with CLIENT_PROPAGATED priority model).
      object =
        orb->string_to_object (ior1,
                               ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Test_var client_propagated_obj =
        Test::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Test object 2 (with SERVER_DECLARED priority model).
      object = orb->string_to_object (ior2,
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Test_var server_declared_obj =
        Test::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Test: Attempt to set priority bands that do not match server
      // resource configuration on the <client_propagated_obj>.
      // Should get INV_POLICY exception.
      ACE_DEBUG ((LM_DEBUG,
                  "\n<---Test--->: Client bands do not match server lanes\n\n"));

      RTCORBA::PriorityBands false_bands;
      false_bands.length (2);
      false_bands[0].low = 10000;
      false_bands[0].high = 10005;
      false_bands[1].low = 26000;
      false_bands[1].high = 30000;
      CORBA::PolicyList policies;
      policies.length (1);
      policies[0] =
        rt_orb->create_priority_banded_connection_policy (false_bands,
                                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      object =
        client_propagated_obj->_set_policy_overrides (policies,
                                                      CORBA::SET_OVERRIDE,
                                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      client_propagated_obj =
        Test::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      invocation_exception_test (client_propagated_obj.in (),
                                 0,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Now set the priority bands that match server resource
      // configuration on the <client_propagated_obj>.
      policies[0] =
        server_declared_obj->_get_policy (RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE,
                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      RTCORBA::PriorityBandedConnectionPolicy_var bands_policy =
        RTCORBA::PriorityBandedConnectionPolicy::_narrow (policies[0]);

      RTCORBA::PriorityBands_var bands =
        bands_policy->priority_bands ();

      object =
        client_propagated_obj->_set_policy_overrides (policies,
                                                      CORBA::SET_OVERRIDE,
                                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      client_propagated_obj =
        Test::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Test: Attempt invocation on <client_propagated_obj> with
      // client thread priority not matching any of the bands.  Should
      // get INV_POLICY exception.
      ACE_DEBUG ((LM_DEBUG,
                  "\n<---Test--->: Client threads does not match band\n\n"));

      CORBA::Short client_priority =
        bands[bands->length () - 1].high + 1;
      current->the_priority (client_priority,
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      invocation_exception_test (client_propagated_obj.in (),
                                 client_priority,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Test: Make invocations on the <client_propagated_obj>.
      ACE_DEBUG ((LM_DEBUG,
                  "\n<---Test--->: Invoking on client propagated object\n\n"));

      for (CORBA::ULong i = 0;
           i < bands->length ();
           ++i)
        {
          CORBA::Short client_priority =
            (bands[i].low + bands[i].high) / 2;

          // Set client thread's priority.
          current->the_priority (client_priority,
                                 ACE_TRY_ENV);
          ACE_TRY_CHECK;

          client_propagated_obj->test_method (1,
                                              client_priority,
                                              ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }

      // Test: Attempt invocation with the same thread priority, but
      // now on the <server_declared_obj>.  This should succeed.
      ACE_DEBUG ((LM_DEBUG,
                  "\n<---Test--->: Invoking on server declared object\n\n"));

      CORBA::Short server_priority =
        get_server_priority (server_declared_obj.in (),
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      server_declared_obj->test_method (0,
                                        server_priority,
                                        ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Testing over. Shut down Server ORB.
      server_declared_obj->shutdown (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
       "Unexpected exception in Banded_Connections test client:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
