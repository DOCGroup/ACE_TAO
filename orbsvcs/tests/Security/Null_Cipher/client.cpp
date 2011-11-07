// -*- C++ -*-
//
// $Id$
//

#include "ace/Get_Opt.h"

#include "FooC.h"
#include "ace/SString.h"
#include "orbsvcs/SecurityC.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
const char *cert_file = "cacert.pem";

void
null_cipher_test (CORBA::ORB_ptr orb,
                  CORBA::Object_ptr obj)
{
  Security::QOP qop = Security::SecQOPIntegrity;
  //  Security::EstablishTrust establish_trust = {0,1};

  CORBA::Any protection;
  //CORBA::Any trust;

  protection <<= qop;
  //trust <<= establish_trust;

  CORBA::PolicyList policy_list (2);
  policy_list.length (1);
  policy_list[0] =
    orb->create_policy (Security::SecQOPPolicy, protection);
//   policy_list[1] =
//     orb->create_policy (Security::SecEstablishTrustPolicy, trust);

  // Create an object reference that uses plain IIOP (i.e. no
  // protection).
  CORBA::Object_var object =
    obj->_set_policy_overrides (policy_list,
                                CORBA::SET_OVERRIDE);

  Foo_var server =
    Foo::_narrow (object.in ());

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
      server->shutdown ();
    }
  catch (const CORBA::NO_PERMISSION&)
    {
      ACE_DEBUG ((LM_INFO,
                  "(%P|%t) Received CORBA::NO_PERMISSION from "
                  "server\n"));

    }
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
  // Indicates successful parsing of the command line
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

      null_cipher_test (orb.in (), object.in ());

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
