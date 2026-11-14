// -*- C++ -*-
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdlib.h"

#include "FooC.h"
#include "orbsvcs/SecurityC.h"

#include <string>

struct Client_Options
{
  ACE_TCHAR const *ior {ACE_TEXT ("file://test.ior")};
  bool use_certificate {true};
};

int
insecure_invocation_test (CORBA::ORB_ptr orb,
                          CORBA::Object_ptr obj,
                          ACE_TCHAR const *ior)
{
  // Disable protection for this insecure invocation test.
  const Security::QOP qop = Security::SecQOPNoProtection;
  CORBA::Any no_protection;
  no_protection <<= qop;
  CORBA::Policy_var policy = orb->create_policy (Security::SecQOPPolicy,
                                                  no_protection);
  CORBA::PolicyList policy_list (1);
  policy_list.length (1);
  policy_list[0] = CORBA::Policy::_duplicate (policy.in ());

  // Create an object reference that uses plain IIOP (i.e. no protection).
  CORBA::Object_var object = obj->_set_policy_overrides (policy_list,
                                                          CORBA::SET_OVERRIDE);
  Foo::Bar_var server = Foo::Bar::_narrow (object.in ());
  if (CORBA::is_nil (server.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) ERROR: Object reference <%s> is nil.\n"),
                  ior));
      return 1;
    }

  try
    {
      // This invocation should result in a CORBA::NO_PERMISSION exception.
      server->baz ();
    }
  catch (const CORBA::NO_PERMISSION &)
    {
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("(%P|%t) Received CORBA::NO_PERMISSION from server, as expected.\n")));
      return 0;
    }

  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("(%P|%t) ERROR: CORBA::NO_PERMISSION was not thrown.\n")
              ACE_TEXT ("(%P|%t) ERROR: It should have been thrown.\n")));
  return 1;
}

int
secure_invocation_test (CORBA::Object_ptr object)
{
  Foo::Bar_var server = Foo::Bar::_narrow (object);
  if (CORBA::is_nil (server.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) ERROR: Object reference is nil.\n")));
      return 1;
    }

  // This invocation should return successfully.
  server->baz ();
  server->shutdown ();
  return 0;
}

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("nk:"));
  int c;
  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case 'n':
        options.use_certificate = false;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Usage: %s -k <ior> [-n]\n"),
                           argv[0]),
                          -1);
      }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Client_Options options;
  for (int c = 1; c < argc; ++c)
    if (ACE_OS::strcmp (argv[c], ACE_TEXT ("-n")) == 0)
      options.use_certificate = false;

  std::string cert_environment;
  if (options.use_certificate)
    {
      cert_environment = "SSL_CERT_FILE=cacert.pem";
      ACE_OS::putenv (cert_environment.data ());
    }

  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var object = orb->string_to_object (options.ior);
      int status = 0;
      if (options.use_certificate)
        {
          // The unprotected invocation should fail with CORBA::NO_PERMISSION.
          status = insecure_invocation_test (orb.in (),
                                             object.in (),
                                             options.ior);
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("insecure_invocation_test returned <%d>\n"),
                      status));
        }

      // The default secure SSLIOP invocation should succeed and stop the server.
      try
        {
          status = secure_invocation_test (object.in ());
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("secure_invocation_test returned <%d>\n"),
                      status));
        }
      catch (const CORBA::Exception &ex)
        {
          if (options.use_certificate)
            {
              ex._tao_print_exception ("Caught unexpected exception (probable failure):");
              status = 1;
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("Caught an exception as expected because SSL_CERT_FILE is unset.\n")));
            }
        }

      orb->destroy ();
      if (status == 0)
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nBug_1107_Regression test passed.\n")));
      return status;
    }
  catch (const CORBA::Exception &ex)
    {
      ex._tao_print_exception ("Exception in main:");
      return 1;
    }
}
