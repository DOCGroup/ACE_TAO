// $Id$

#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"

#include "tao/Strategies/advanced_resource.h"

#if (TAO_HAS_RT_CORBA == 1)

const char *ior1 = "file://test1.ior";
const char *ior2 = "file://test2.ior";
CORBA::Short client_priority1 = -1;
CORBA::Short client_priority2 = -1;
CORBA::Short client_priority3 = -1;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "n:o:a:b:c:");
  int c, result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':
        ior1 = get_opts.optarg;
        break;
      case 'o':
        ior2 = get_opts.optarg;
        break;
      case 'a':
        result = ::sscanf (get_opts.optarg,
                           "%hd",
                           &client_priority1);
        if (result == 0 || result == EOF)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to process <-a> option"),
                            -1);
        break;

      case 'b':
        result = ::sscanf (get_opts.optarg,
                           "%hd",
                           &client_priority2);
        if (result == 0 || result == EOF)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to process <-b> option"),
                            -1);
        break;

      case 'c':
        result = ::sscanf (get_opts.optarg,
                           "%hd",
                           &client_priority3);
        if (result == 0 || result == EOF)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to process <-c> option"),
                            -1);
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-n <ior> "
                           "-o <ior> "
                           "-a <client_priority1>"
                           "-b <client_priority2>"
                           "-c <client_priority3>"
                           "\n",
                           argv [0]),
                          -1);
      }

  if (client_priority1 < 0 
      || client_priority2 < 0
      || client_priority3 < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Valid client priorities must be"
                       " specified.\nSee README file for more info\n"),
                      -1);

    return 0;
}

int
check_for_nil (CORBA::Object_ptr obj, const char *msg)
{
  if (CORBA::is_nil (obj))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ERROR: Object reference <%s> is nil\n",
                       msg),
                      -1);
  else
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

  if (check_for_nil (priority_policy.in (), "PriorityModelPolicy") == -1)
    return -1;

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
      obj->test_method (priority,
                        ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Test Succeeded: no exception caught\n"));
    }
  ACE_CATCH (CORBA::INV_POLICY, ex)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "INV_POLICY exception is caught as expected.\n"));
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG, "Test failed: unexpected exception caught\n"));
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

      // Initialize the ORB, resolve references and parse arguments.

      // ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Parse arguments.
      if (parse_args (argc, argv) != 0)
        return 1;

      // RTORB.
      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB", ACE_TRY_ENV);
      ACE_TRY_CHECK;
      RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (object.in (),
                                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (check_for_nil (rt_orb.in (), "RTORB") == -1)
        return 1;

      // RTCurrent.
      object =
        orb->resolve_initial_references ("RTCurrent", ACE_TRY_ENV);
      ACE_TRY_CHECK;
      RTCORBA::Current_var current =
        RTCORBA::Current::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Test object 1 (with CLIENT_PROPAGATED priority model).
      object =
        orb->string_to_object (ior1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Test_var client_propagated_obj = 
        Test::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (check_for_nil (client_propagated_obj.in (), 
                         "client_propagated_obj") 
          == -1)
        return 1;

      // Test object 2 (with SERVER_DECLARED priority model).
      object = orb->string_to_object (ior2, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Test_var server_declared_obj = 
        Test::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (check_for_nil (server_declared_obj.in (), 
                         "server_declared_obj") 
          == -1)
        return 1;

      // Set client thread's priority.
      current->the_priority (client_priority1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Test 4: Attempt to set priority bands that do not match
      // server resource configuration on the <client_propagated_obj>.
      // Should get INV_POLICY exception.
      ACE_DEBUG ((LM_DEBUG,
                  "\n     Test 4\n"));
      
      RTCORBA::PriorityBands false_bands;
      false_bands.length (2);
      false_bands[0].low = 10000;
      false_bands[0].high = 10005;
      false_bands[1].low = 26;
      false_bands[1].high = 30;
      CORBA::PolicyList policies;
      policies.length (1);
      policies[0] =
        rt_orb->create_priority_banded_connection_policy 
        (false_bands,
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
      if (check_for_nil (client_propagated_obj.in (), 
                         "client_propagated_obj") 
          == -1)
        return 1;

      invocation_exception_test (client_propagated_obj.in (),
                                 client_priority1,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Now set the priority bands that match server resource
      // configuration on the <client_propagated_obj>.
      CORBA::Policy_var policy =
        server_declared_obj->_get_policy (RTCORBA::PRIORITY_BANDED_CONNECTION_POLICY_TYPE,
                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      policies[0] = policy;
      
      object =
        client_propagated_obj->_set_policy_overrides (policies,
                                                      CORBA::SET_OVERRIDE,
                                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      client_propagated_obj = 
        Test::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (check_for_nil (client_propagated_obj.in (), 
                         "client_propagated_obj") 
          == -1)
        return 1;

      // Test 5: Make invocations on the <client_propagated_obj>,
      // changing the priority of the invoking thread.
      ACE_DEBUG ((LM_DEBUG,
                  "\n     Test 5\n"));

      client_propagated_obj->test_method (client_priority1,
                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      current->the_priority (client_priority2, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      client_propagated_obj->test_method (client_priority2,
                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Test 6: Attempt invocation on <client_propagated_obj> with
      // client thread priority not matching any of the bands.  Should
      // get INV_POLICY exception.
      ACE_DEBUG ((LM_DEBUG,
                  "\n     Test 6\n"));

      current->the_priority (client_priority3, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      invocation_exception_test (client_propagated_obj.in (),
                                 client_priority3,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Test 7: Attempt invocation with the same thread priority, but
      // now on the <server_declared_obj>.  This should succeed.
      ACE_DEBUG ((LM_DEBUG,
                  "\n     Test 7\n"));

      CORBA::Short server_priority =
        get_server_priority (server_declared_obj.in (),
                             ACE_TRY_ENV);
      ACE_TRY_CHECK;
        
      server_declared_obj->test_method (server_priority,
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

#else /* TAO_HAS_RT_CORBA == 1 */

int
main (int argc, char *argv[])
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_ERROR_RETURN ((LM_ERROR,
                     "\nRTCORBA must be enabled to run this test!\n"),
                    1);
}

#endif /* TAO_HAS_RT_CORBA == 1 */
