// -*- C++ -*-
// $Id$

#include "ace/Get_Opt.h"

#include "FooC.h"
#include "orbsvcs/SecurityC.h"
#include "ace/SString.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
const char *cert_file = "cacert.pem";

int
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

      return 1;
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

      return 0;
    }

  ACE_ERROR ((LM_ERROR,
              "(%P|%t) ERROR: CORBA::NO_PERMISSION was not thrown.\n"
              "(%P|%t) ERROR: It should have been thrown.\n"));

  return 1;
}

int
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

      return 1;
    }

  // This invocation should return successfully.
  server->baz ();

  server->shutdown ();

  return 0;
}

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("nk:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'n':
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s "
                           "-k <ior>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int status = 0;
  bool set_cert_file = true;
  try
    {
      // This has to be done before calling CORBA::ORB_init() and
      // parse_args() has to be called after CORBA::ORB_init(), so we
      // will look at argv manually.
      for(int i = 0; i < argc; i++)
        {
          if (ACE_OS::strcmp(argv[i], ACE_TEXT("-n")) == 0)
            {
              set_cert_file = false;
              break;
            }
        }

      ACE_CString env ("SSL_CERT_FILE=");
      env += cert_file;
      if (set_cert_file)
        {
          ACE_OS::putenv (env.c_str ());
        }

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var object =
        orb->string_to_object (ior);

      if (set_cert_file)
        {
          // This test sets creates a Security::QOPPolicy with the
          // Quality-of-Protection set to "no protection."  It then
          // invokes a method on the server (insecurely), which should
          // then result in a CORBA::NO_PERMISSION exception.
          //
          // The server is not shutdown by this test.
          status = insecure_invocation_test (orb.in (), object.in ());
        }

      // This test uses the default secure SSLIOP settings to securely
      // invoke a method on the server.  No exception should occur.
      //
      // The server *is* shutdown by this test.
      try
        {
          status = secure_invocation_test (object.in ());
        }
      catch (CORBA::Exception const &ex)
        {
          if (set_cert_file)
            {
              ex._tao_print_exception ("Caught unexpected exception "
                                       "(probable failure):");
              status = 1;
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Caught an exception as expected due "
                          "to the SSL_CERT_FILE environment "
                          "variable not being set.\n"));
            }
        }

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception in main:");
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "\n"
              "Bug_1107_Regression test passed.\n"));

  return 0;
}
