// $Id$

#include "testC.h"
#include "ace/Get_Opt.h"
#include "tao/RTCORBA/RTCORBA.h"

#include "tao/Strategies/advanced_resource.h"

const char *ior1 = "file://test1.ior";
const char *ior2 = "file://test2.ior";
CORBA::ULong protocol_type = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "s:c:p:");
  int c, result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 's':
        ior1 = get_opts.opt_arg ();
        break;
      case 'c':
        ior2 = get_opts.opt_arg ();
        break;
      case 'p':
        result = ::sscanf (get_opts.opt_arg (),
                           "%u",
                           &protocol_type);
        if (result == 0 || result == EOF)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to process <-p> option"),
                            -1);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-s <ior> "
                           "-c <ior> "
                           "-p <protocol_type> "
                           "\n",
                           argv [0]),
                          -1);
      }

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

void
exception_test (Test_ptr server,
                const char *msg
                ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      server->test_method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
     }
  ACE_CATCH (CORBA::INV_POLICY, ex)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "INV_POLICY exception is caught as expected.\n"));
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG, msg));
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
      // Initialize the ORB, resolve references and parse arguments.

      // ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Parse arguments.
      if (parse_args (argc, argv) != 0)
        return 1;

      // RTORB.
      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (object.in ()
                                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (check_for_nil (rt_orb.in (), "RTORB") == -1)
        return 1;

      // PolicyManager.
      object = orb->resolve_initial_references ("ORBPolicyManager"
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA::PolicyManager_var policy_manager =
        CORBA::PolicyManager::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (check_for_nil (policy_manager.in (), "PolicyManager")
          == -1)
        return 1;

      // PolicyCurrent.
      object = orb->resolve_initial_references ("PolicyCurrent"
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA::PolicyCurrent_var policy_current =
        CORBA::PolicyCurrent::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (check_for_nil (policy_current.in (), "PolicyCurrent")
          == -1)
        return 1;

      // Test object 1 (ClientProtocolPolicy set on server).
      object = orb->string_to_object (ior1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      Test_var server1 = Test::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (check_for_nil (server1.in (), "server1") == -1)
        return 1;

      // Test object 2 (no client-exposed ClientProtocolPolicy).
      object = orb->string_to_object (ior2 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      Test_var server2 = Test::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (check_for_nil (server2.in (), "server2") == -1)
        return 1;

      // Do testing.

      // Test 1: Invoke operation on the object that has
      // ClientProtocolPolicy set on the server side.
      ACE_DEBUG ((LM_DEBUG,
                  "\n     Test 1\n"));
      server1->test_method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Test 2: Set the ORB-level ClientProtocolPolicy override, and
      // attempt the same invocation again.  Should get
      // CORBA::INV_POLICY exception since the policy is set on both
      // client and server sides.
      ACE_DEBUG ((LM_DEBUG,
                  "\n     Test 2\n"));
      RTCORBA::ProtocolList protocols;
      protocols.length (1);
      protocols[0].protocol_type = protocol_type;
      protocols[0].transport_protocol_properties =
        RTCORBA::ProtocolProperties::_nil ();
      protocols[0].orb_protocol_properties =
        RTCORBA::ProtocolProperties::_nil ();

      CORBA::PolicyList policy_list;
      policy_list.length (1);
      policy_list[0] =
        rt_orb->create_client_protocol_policy (protocols
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policy_manager->set_policy_overrides (policy_list,
                                            CORBA::SET_OVERRIDE
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      exception_test (server1.in (),
                      "ERROR: Test 2 failed\n" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Test 3: Attempt the invocation on the second object reference
      // (the one that didn't have ClientProtocolPolicy set on the
      // server side).  This should succeed since there are no
      // conflicts.
      ACE_DEBUG ((LM_DEBUG,
                  "\n     Test 3\n"));
      server2->test_method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Test 4: Override ClientProtocolPolicy on the Current level.
      // For the override value, use the sequence of protocols, none
      // of which are available in the server ORB.  Attempt an
      // invocation on the second object.  Should get
      // CORBA::INV_POLICY exception since none of the protocols
      // specified in the policy are available.
      ACE_DEBUG ((LM_DEBUG,
                  "\n     Test 4\n"));
      // Hardcode a sequence of nonexistent protocols.
      protocols.length (3);
      protocols[0].protocol_type = 3;
      protocols[1].protocol_type = 4;
      protocols[2].protocol_type = 5;
      policy_list[0] =
        rt_orb->create_client_protocol_policy (protocols
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policy_current->set_policy_overrides (policy_list,
                                            CORBA::SET_OVERRIDE
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      exception_test (server2.in (),
                      "ERROR: Test 4 failed\n" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Test 5: Override ClientProtocolPolicy on the Current level
      // again.  This time use the sequence in which the first
      // protocol isn't available and the second one is.  The
      // invocation should succeed through using the second protocol.
      ACE_DEBUG ((LM_DEBUG,
                  "\n     Test 5\n"));
      protocols.length (2);
      protocols[0].protocol_type = 3;
      protocols[1].protocol_type = protocol_type;
      policy_list[0] =
        rt_orb->create_client_protocol_policy (protocols
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policy_current->set_policy_overrides (policy_list,
                                            CORBA::SET_OVERRIDE
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      server2->test_method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Testing over.  Shut down server ORB.
      ACE_DEBUG ((LM_DEBUG,
                  "\n     Testing over\n"));
      server2->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
      "Unexpected exception caught in ClientProtocolPolicy test client:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
