// -*- C++ -*-

#include "ace/Get_Opt.h"

#include "FooC.h"
#include "orbsvcs/SecurityC.h"
#include "ace/SString.h"

ACE_RCSID (Secure_Invocation,
           client,
           "$Id$")

const char *ior = "file://test.ior";
const char *cert_file = "cacert.pem";

int disable_protection = 0;
int test_failure_expected = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:d:f:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'd':
        disable_protection = (get_opts.opt_arg () [0] == '1');
        break;
      case 'f':
        test_failure_expected = (get_opts.opt_arg () [0] == '1');
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s "
                           "-k <ior> "
                           "[-d 0|1] "
                           "[-f 0|1] "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}


void
insecure_invocation_test (CORBA::ORB_ptr orb,
                          Foo::Bar_ptr obj
                          ACE_ENV_ARG_DECL)
{
  // Disable protection for this insecure invocation test.

  Security::QOP qop = Security::SecQOPNoProtection;

  CORBA::Any no_protection;
  no_protection <<= qop;

  // Create the Security::QOPPolicy.
  CORBA::Policy_var policy =
    orb->create_policy (Security::SecQOPPolicy,
                        no_protection
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::PolicyList policy_list (1);
  policy_list.length (1);
  policy_list[0] = CORBA::Policy::_duplicate (policy.in ());

  // Create an object reference that uses plain IIOP (i.e. no
  // protection).
  CORBA::Object_var object =
    obj->_set_policy_overrides (policy_list,
                                CORBA::SET_OVERRIDE
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  Foo::Bar_var server =
    Foo::Bar::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (server.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) ERROR: Object reference <%s> is "
                  "nil.\n",
                  ior));

      ACE_THROW (CORBA::INTERNAL ());
    }


  server->baz (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_TRY_CHECK;

  return;
}

void
invocation_test (Foo::Bar_ptr server
                 ACE_ENV_ARG_DECL)
{
  // This invocation should return successfully.
  server->baz (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

int
main (int argc, char *argv[])
{
  int test_aborting = 1;

  ACE_TRY_NEW_ENV
    {
      ACE_TString env ("SSL_CERT_FILE=");
      env += cert_file;
      ACE_OS::putenv (env.c_str ());

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return test_aborting;

      CORBA::Object_var object =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Foo::Bar_var server =
        Foo::Bar::_narrow (object ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (CORBA::is_nil (server.in ()))
      {
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) ERROR: Object reference <%s> is "
                    "nil.\n",
                    ior));

        ACE_THROW (CORBA::INTERNAL ());
      }

      // This test sets creates a Security::QOPPolicy with the
      // Quality-of-Protection set to "no protection."  It then
      // invokes a method on the server (insecurely), which should
      // then result in a CORBA::NO_PERMISSION exception.
      if (disable_protection)
      {
        insecure_invocation_test (orb.in (), server.in () ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
      else
      {
        // This test uses the default secure SSLIOP settings to securely
        // invoke a method on the server.  No exception should occur.
        invocation_test (server.in () ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }

      server->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_aborting = 0;
    }
  ACE_CATCHANY
    {
      if (test_failure_expected)
      {
        ACE_DEBUG ((LM_WARNING,
                    "An expected, client-side exception was caught."));
      }
      else
      {
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                             "Caught client-side exception:");
      }
    }
  ACE_ENDTRY;

  if ((test_aborting && test_failure_expected)
      || (!test_aborting && !test_failure_expected))
  {
    ACE_DEBUG ((LM_DEBUG,
                "\n"
                "Policies test passed.\n"));

    return 0;
  }
  else
  {
    ACE_DEBUG ((LM_DEBUG,
                "\n"
                "Policies test failed.\n"));
    return 1;
  }
}
