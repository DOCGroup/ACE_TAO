// -*- C++ -*-
// $Id$

#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include "orbsvcs/SecurityC.h"
#include "FooC.h"
#include "constants.h"

bool shutdown_server = true;

class ClientTest
{
public:
  struct Results
  {
    bool secure;
    bool non_secure;
    bool successful; // set by "run" if actual meets expected

    Results(bool sec = false, bool nsec = false)
      : secure(sec), non_secure(nsec), successful(false) { }
  };

  ClientTest (CORBA::ORB_ptr orb, const ACE_TCHAR* ior);

  /*!
   * Creates a Security::QOPPolicy with the Quality-of-Protection set
   * to "no protection."  It then invokes a method on @c obj_ via a
   * non-secured transport, which should then result in a @c
   * CORBA::NO_PERMISSION exception.
   */
  bool non_secure_invocation ();
  bool secure_invocation ();

  Results run(const Results& expected_results);

private:
  CORBA::ORB_var orb_;
  Foo::Bar_var obj_;
};

ClientTest::ClientTest (CORBA::ORB_ptr orb, const ACE_TCHAR* ior)
  : orb_(CORBA::ORB::_duplicate (orb))
{
  CORBA::Object_var o = this->orb_->string_to_object (ior);
  if (CORBA::is_nil (o.in()))
    throw CORBA::INTERNAL ();

  this->obj_ = Foo::Bar::_narrow (o.in ());
}

ClientTest::Results
ClientTest::run (const ClientTest::Results& expected)
{
  Results actual;
#if 1
  actual.non_secure = this->non_secure_invocation ();
#else
  actual.non_secure = true;
#endif
  actual.secure = this->secure_invocation ();

  actual.successful = (expected.secure == actual.secure
                       && expected.non_secure == actual.non_secure);

  return actual;
}

bool
ClientTest::non_secure_invocation ()
{
  ACE_DEBUG ((LM_DEBUG, "mixed_security/client: invoking via non-secured means\n"));
  // Disable protection for this insecure invocation test.

  Security::QOP qop = Security::SecQOPNoProtection;

  CORBA::Any no_protection;
  no_protection <<= qop;

  // Create the Security::QOPPolicy.
  CORBA::Policy_var policy =
    this->orb_->create_policy (Security::SecQOPPolicy,
                               no_protection);

  CORBA::PolicyList policy_list (1);
  policy_list.length (1);
  policy_list[0] = CORBA::Policy::_duplicate (policy.in ());

  // Create an object reference that uses plain IIOP (i.e. no
  // protection).
  CORBA::Object_var object =
    this->obj_->_set_policy_overrides (policy_list,
                                       CORBA::SET_OVERRIDE);

  Foo::Bar_var server =
    Foo::Bar::_narrow (object.in ());

  if (CORBA::is_nil (server.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) ERROR: Failed to narrow override reference to "
                  "Foo::Bar type.\n"));

      throw CORBA::INTERNAL ();
    }

  bool invocation_succeeded = true;
  try
    {
      // This invocation should result in a CORBA::NO_PERMISSION
      // exception.
      server->baz ();
      ACE_DEBUG ((LM_DEBUG, "mixed_security/client: non-secured invocation succeeded\n"));
    }
  catch (const CORBA::NO_PERMISSION& )
    {
      ACE_DEBUG ((LM_DEBUG,
                  "ClientTest::non_secure_invocation: got NO_PERMISSION\n"));
      invocation_succeeded = false;
    }

  return invocation_succeeded;
}

bool
ClientTest::secure_invocation ()
{
  ACE_DEBUG ((LM_DEBUG, "mixed_security/client: invoking via secure means\n"));

  // In this test, any NO_PERM exception is a failure.
  bool invocation_succeeded = true;
  try
    {
      // This invocation should return successfully.
      this->obj_->baz ();
      ACE_DEBUG ((LM_DEBUG, "mixed_security/client: secured invocation succeeded\n"));
    }
  catch (const CORBA::NO_PERMISSION&)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "ClientTest::secure_invocation: got NO_PERMISSION\n"));
      invocation_succeeded = false;
    }

  return invocation_succeeded;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  // Accomodate deficiencies on Windows that preclude doing this in
  // run_test.pl
  ACE_CString env ("SSL_CERT_FILE=");
  env += TAO_Mixed_Security_Test::cert_file;
  ACE_OS::putenv (env.c_str ());

  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      ClientTest restricted (orb.in (),
                             TAO_Mixed_Security_Test::restricted_ior);

      // Run the restricted test
      ClientTest::Results restricted_results =
        restricted.run (ClientTest::Results(true, false));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("===> Restricted test %C: secure=%d, non-secure=%d\n"),
                  restricted_results.successful ? "PASSED" : "FAILED",
                  restricted_results.secure,
                  restricted_results.non_secure));

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("mixed_security/client: set up permitted test\n")));
      // Run the permitted test
      ClientTest permitted (orb.in (),
                            TAO_Mixed_Security_Test::permitted_ior);

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("mixed_security/client: running permitted test\n")));
      ClientTest::Results permitted_results =
        permitted.run (ClientTest::Results(true, true));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("===> Permitted test %C: secure=%d, non-secure=%d\n"),
                  permitted_results.successful ? "PASSED" : "FAILED",
                  permitted_results.secure,
                  permitted_results.non_secure));

      // The server ORB *is* shutdown by this test, if explicitly requested
      // @@ at this point there's no way to specify this...but I should
      if (shutdown_server)
        {
          CORBA::Object_var o =
          orb->string_to_object (TAO_Mixed_Security_Test::permitted_ior);
          Foo::Bar_var foo = Foo::Bar::_narrow (o.in());
          foo->shutdown ();
        }

      orb->destroy ();
    }
  catch (CORBA::Exception& ex)
    {
      ex._tao_print_exception
        (ACE_TEXT ("mixed_security/client: caught unexpected exception "));
      return 1;
    }

  return 0;
}
