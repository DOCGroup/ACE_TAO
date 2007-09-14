// $Id$

#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "tao/Policy_ManagerC.h"
#include "tao/Policy_CurrentC.h"

const char *ior = "file://test.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <ior> "
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

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB, parse arguments, and resolve references.

      // ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      // Parse arguments.
      if (parse_args (argc, argv) != 0)
        return -1;

      // RTORB.
      CORBA::Object_var object =
        orb->resolve_initial_references ("RTORB");
      RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (object.in ());
      if (check_for_nil (rt_orb.in (), "RTORB") == -1)
        return -1;

      // PolicyCurrent.
      object = orb->resolve_initial_references ("PolicyCurrent");
      CORBA::PolicyCurrent_var policy_current =
        CORBA::PolicyCurrent::_narrow (object.in ());
      if (check_for_nil (policy_current.in (), "PolicyCurrent")
          == -1)
        return -1;

      // Test object.
      object = orb->string_to_object (ior);
      Test_var server = Test::_narrow (object.in ());
      if (check_for_nil (server.in (), "server") == -1)
        return -1;

      // Test 1: Check that <validate_connection> establishes an
      // appropriate connection for the current set of effective
      // policies.
      // Set ClientProtocolPolicy override to SHMIOP, and invoke
      // <validate_connection> on the object.  This should succeed, and
      // SHMIOP connection should get established.
      RTCORBA::ProtocolList protocols;
      protocols.length (1);
      protocols[0].protocol_type = TAO_TAG_SHMEM_PROFILE;
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

      ACE_DEBUG ((LM_DEBUG,
                  "\n     Test 1\n"));

      CORBA::PolicyList_var pols;
      int status = server->_validate_connection (pols.out ());

      if (!status)
        ACE_DEBUG ((LM_DEBUG,
                    "ERROR: <validate_connection> returned FALSE\n"));

      // Test 2: Check that connection established with
      // <validate_connection> is used for subsequent invocations.
      // Invoke <test_method> on the object.  This should succeed and
      // NO new connections should get established.
      ACE_DEBUG ((LM_DEBUG,
                  "\n     Test 2\n"));
      server->test_method ();

      // Test 3: Check that <validate_connection> detects policy
      // misconfigurations and reports them through
      // <inconsistent_policies> argument.
      // Set ClientProtocolPolicy override to UIOP, call
      // <validate_connection>.   It should return FALSE, and
      // <inconsistent_policies> should contain the problematic
      // override.
      ACE_DEBUG ((LM_DEBUG,
                  "\n     Test 3\n"));

      protocols[0].protocol_type = TAO_TAG_UIOP_PROFILE;
      policy_list[0] =
        rt_orb->create_client_protocol_policy (protocols);

      policy_current->set_policy_overrides (policy_list,
                                            CORBA::SET_OVERRIDE);

      status = server->_validate_connection (pols.out ());

      if (status)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("<validate_connection> returned TRUE\n")));

      //
      // This portion of code has been temporarily disabled.
      //
      /*
        if (pols.ptr () != 0
        && pols->length () == 1
        && pols[0u]->policy_type () == RTCORBA::CLIENT_PROTOCOL_POLICY_TYPE)
        ACE_DEBUG ((LM_DEBUG,
        "Inconsistent policies contain "
        "ClientProtocolPolicy, as expected.\n"));
        else
        ACE_DEBUG ((LM_DEBUG,
        "ERROR: Inconsistent policies do not "
        "contain what's expected.\n"));
      */

      // Testing over.  Shut down Server ORB.
      protocols[0].protocol_type = TAO_TAG_SHMEM_PROFILE;
      policy_list[0] =
        rt_orb->create_client_protocol_policy (protocols);

      policy_current->set_policy_overrides (policy_list,
                                            CORBA::SET_OVERRIDE);

      ACE_DEBUG ((LM_DEBUG,
                  "\n  Testing over - shutting down\n"));
      ACE_OS::sleep (2);
      server->shutdown ();

      ACE_OS::sleep (2);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Unexpected exception caught in Explicit_Binding test client:");
      return -1;
    }

  return 0;
}
