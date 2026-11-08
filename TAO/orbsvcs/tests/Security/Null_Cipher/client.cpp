// -*- C++ -*-

#include "ace/Get_Opt.h"
#include "FooC.h"
#include "orbsvcs/SecurityC.h"

struct Client_Options
{
  ACE_TCHAR const *ior {ACE_TEXT ("file://test.ior")};
};

void
null_cipher_test (CORBA::ORB_ptr orb,
                  CORBA::Object_ptr obj,
                  ACE_TCHAR const *ior)
{
  Security::QOP const qop = Security::SecQOPIntegrity;
  CORBA::Any protection;
  protection <<= qop;

  CORBA::PolicyList policy_list (1);
  policy_list.length (1);
  policy_list[0] = orb->create_policy (Security::SecQOPPolicy, protection);

  // Create an object reference that uses plain IIOP (i.e. no protection).
  CORBA::Object_var object = obj->_set_policy_overrides (policy_list,
                                                         CORBA::SET_OVERRIDE);
  Foo_var server = Foo::_narrow (object.in ());
  if (CORBA::is_nil (server.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) ERROR: Object reference <%s> is nil.\n"),
                  ior));
      throw CORBA::INTERNAL ();
    }

  try
    {
      // This invocation should result in a CORBA::NO_PERMISSION exception.
      server->shutdown ();
    }
  catch (CORBA::NO_PERMISSION const &)
    {
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("(%P|%t) Received CORBA::NO_PERMISSION from server\n")));
    }
}

int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c {};
  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Usage: %s -k <ior>\n"),
                           argv[0]),
                          -1);
      }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      Client_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;
      CORBA::Object_var object = orb->string_to_object (options.ior);
      null_cipher_test (orb.in (), object.in (), options.ior);
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\nSecure_Invocation test passed.\n")));
  return 0;
}
