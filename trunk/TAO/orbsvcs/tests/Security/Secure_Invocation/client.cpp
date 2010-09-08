// -*- C++ -*-
// $Id$

#include "ace/Get_Opt.h"

#include "FooC.h"
#include "orbsvcs/SecurityC.h"
#include "ace/SString.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
const char *cert_file = "cacert.pem";

void
insecure_invocation_test (CORBA::ORB_ptr orb,
                          CORBA::Object_ptr obj)
{
  // Disable protection for this insecure invocation test.

  Security::QOP qop = Security::SecQOPNoProtection;

  CORBA::Any no_protection;
  no_protection <<= qop;

  // Create the Security::QOPPolicy.
  CORBA::Policy_var policy =
    orb->create_policy (Security::SecQOPPolicy,
                        no_protection);

  CORBA::PolicyList policy_list (1);
  policy_list.length (1);
  policy_list[0] = CORBA::Policy::_duplicate (policy.in ());

  // Create an object reference that uses plain IIOP (i.e. no
  // protection).
  CORBA::Object_var object =
    obj->_set_policy_overrides (policy_list,
                                CORBA::SET_OVERRIDE);

  Foo::Bar_var server =
    Foo::Bar::_narrow (object.in ());

  if (CORBA::is_nil (server.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) ERROR: Object reference <%s> is "
                  "nil.\n",
                  ior));

      throw CORBA::INTERNAL ();
    }

  try
    {
      // This invocation should result in a CORBA::NO_PERMISSION
      // exception.
      server->baz ();
    }
  catch (const CORBA::NO_PERMISSION&)
    {
      ACE_DEBUG ((LM_INFO,
                  "(%P|%t) Received CORBA::NO_PERMISSION from "
                  "server, as expected.\n"));

      return;
    }

  ACE_ERROR ((LM_ERROR,
              "(%P|%t) ERROR: CORBA::NO_PERMISSION was not thrown.\n"
              "(%P|%t) ERROR: It should have been thrown.\n"));

  throw CORBA::INTERNAL ();
}

void
secure_invocation_test (CORBA::Object_ptr object)
{
  Foo::Bar_var server =
    Foo::Bar::_narrow (object);

  if (CORBA::is_nil (server.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) ERROR: Object reference <%s> is "
                  "nil.\n",
                  ior));

      throw CORBA::INTERNAL ();
    }

  // This invocation should return successfully.
  server->baz ();

  server->shutdown ();
}

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
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
                           "Usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior);

      // This test sets creates a Security::QOPPolicy with the
      // Quality-of-Protection set to "no protection."  It then
      // invokes a method on the server (insecurely), which should
      // then result in a CORBA::NO_PERMISSION exception.
      //
      // The server is not shutdown by this test.
      insecure_invocation_test (orb.in (), object.in ());

      // This test uses the default secure SSLIOP settings to securely
      // invoke a method on the server.  No exception should occur.
      //
      // The server *is* shutdown by this test.
      secure_invocation_test (object.in ());

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "\n"
              "Secure_Invocation test passed.\n"));

  return 0;
}
