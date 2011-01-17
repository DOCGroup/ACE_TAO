// -*- C++ -*-
// $Id$

#include <ace/SString.h>
#include <ace/ARGV.h>

#include <orbsvcs/SecurityLevel2C.h>
#include <orbsvcs/SecurityLevel3C.h>

#include "Foo_i.h"
#include "constants.h"

/*
  This tests the ability to use the SecurityManager/AccessDecision tools
  to permit unsecured access to specific object references, while all other
  references must be accessed via secured means.  (Note that, for the purposes
  of this conversation, "secured access" refers to the transport type, and
  an SSLIOP transport satisfies the requirements for "secured access.")

  In order to test this, we present a modified version of the
  Secure_Invocation test which creates two references of the same type
  (using the same type tests that access is granted on a per-object
  rather than per-type basis) and uses the AccessDecision tools to
  permit unsecured access to one of them, while leaving the other
  restricted only to secured access.  The server then publishes each
  of these IORs in different files.

  The other half is the client side.  The client will operate the same
  as it currently does, just performing the invocations on both IORs.
  When invoking on the secured reference, the client's insecure
  invocation should receive a NO_PERMISSION.  However, when invoking
  on the permitted reference, the insecure invocation AND the secure
  invocation should both succeed.

  IMPORTANT: SSLIOP is configured without "-SSLNoProtection" in the
  service configurator service listing.
 */

// Init/setup exception
struct RirFailedException
{
  RirFailedException (const char* token = 0) : token_(token) { }
  const char* token_;
};

template <typename REALTYPE>
typename REALTYPE::_ptr_type
rir(CORBA::ORB_ptr orb, const char* token)
{
  if (CORBA::is_nil (orb) || token == 0)
    return REALTYPE::_nil();

  CORBA::Object_var o = orb->resolve_initial_references (token);
  if (CORBA::is_nil (o.in()))
    throw RirFailedException (token);

  typename REALTYPE::_var_type r = REALTYPE::_narrow (o.in());
  return r._retn();
}

void
init_and_setup (int& argc,
                ACE_TCHAR* argv[],
                CORBA::ORB_var& orb,
                PortableServer::POA_var& rootpoa,
                PortableServer::POAManager_var& poamgr,
                SecurityLevel3::SecurityCurrent_var& sl3current,
                TAO::SL2::AccessDecision_var& sl2ad)
{
  orb = CORBA::ORB_init (argc, argv);
  rootpoa = rir<PortableServer::POA> (orb.in (), "RootPOA");
  poamgr = rootpoa->the_POAManager();

  sl3current =
    rir<SecurityLevel3::SecurityCurrent>(orb.in (), "SecurityLevel3:SecurityCurrent");

  SecurityLevel2::SecurityManager_var sl2sm =
    rir<SecurityLevel2::SecurityManager> (orb.in (),
                                          "SecurityLevel2:SecurityManager");
  SecurityLevel2::AccessDecision_var ad = sl2sm->access_decision ();

  sl2ad = TAO::SL2::AccessDecision::_narrow (ad.in ());
  if (CORBA::is_nil (sl2ad.in ()))
    throw "non-tao accessdecision";
}

struct IORPublicationException
{
  static const unsigned int OK = 0;
  static const unsigned int BAD_PARAM = 1;
  static const unsigned int IO_FAILURE = 2;

  IORPublicationException (unsigned why, int errnoval = 0)
    : why_(why), errnoval_(errnoval) { }

  unsigned int why_; // one of the const values above
  int errnoval_; // if set, the relevant value of errno at failure
};

void
publish_ior (CORBA::ORB_ptr orb, CORBA::Object_ptr o, const ACE_TCHAR* filename)
{
  if (filename == 0 || CORBA::is_nil (orb) || CORBA::is_nil (o))
    throw IORPublicationException (IORPublicationException::BAD_PARAM);

  CORBA::String_var ior = orb->object_to_string (o);

  FILE *output_file = ACE_OS::fopen (filename, "w");
  if (output_file == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Cannot open output file %s for writing IOR\n",
                  filename));
      throw IORPublicationException (IORPublicationException::IO_FAILURE,
                                     errno);
    }

  ACE_OS::fprintf (output_file, "%s", ior.in() );
  ACE_OS::fclose (output_file);
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  // Necessary nonsense to make up for deficencies doing this
  // in run_test.pl on Windows platforms.
  ACE_CString env ("SSL_CERT_FILE=");
  env += TAO_Mixed_Security_Test::cert_file;
  ACE_OS::putenv (env.c_str ());

  CORBA::ORB_var orb;
  Foo_i *server1 = 0;
  Foo_i *server2 = 0;

  try
    {
      // 1. ORB setup: init,
      //    grab refs for RootPOA, SL3::SecCurrent, poamgr, AccessDecision, etc.
      PortableServer::POA_var rootpoa;
      PortableServer::POAManager_var poamgr;
      SecurityLevel3::SecurityCurrent_var sl3current;
      TAO::SL2::AccessDecision_var sl2ad;

      // throws exception if it can't work
      init_and_setup (argc, argv, orb, rootpoa, poamgr, sl3current, sl2ad);
      ACE_DEBUG ((LM_DEBUG, "mixed_security/server: "
                  "init and setup complete\n"));

      // 2. Create servant #1 of Foo_i, and its associated Object
      ACE_NEW_RETURN (server1, Foo_i (orb.in(), sl3current.in()), 1);
      Foo::Bar_var server1_obj = server1->_this ();
      ACE_DEBUG ((LM_DEBUG, "mixed_security/server: "
                  "created servant/object #1\n"));

      // 3. Create servant #2 of Foo_i, and its associated Object
      ACE_NEW_RETURN (server2, Foo_i (orb.in(), sl3current.in()), 1);

      Foo::Bar_var server2_obj = server2->_this();
      ACE_DEBUG ((LM_DEBUG, "mixed_security/server: "
                  "created servant/object #2\n"));

      // 4. add servant #2's Object reference to the "permitted" list.
      PortableServer::ObjectId_var oid = rootpoa->servant_to_id (server2);
      CORBA::OctetSeq_var poaid = rootpoa->id();
      CORBA::String_var orbid = orb->id();
      sl2ad->add_object (orbid.in(), poaid.in(), oid.in(), true);
      ACE_DEBUG ((LM_DEBUG, "mixed_security/server: "
                  "added object #2 as a permitted reference for "
                  "non-secure invocations\n"));

      // 5. publish references to #1 and #2 to distinct files
      publish_ior (orb.in (), server1_obj.in (),
                   TAO_Mixed_Security_Test::restricted_ior_file);
      publish_ior (orb.in (), server2_obj.in (),
                   TAO_Mixed_Security_Test::permitted_ior_file);
      ACE_DEBUG ((LM_DEBUG, "mixed_security/server: "
                  "published IORs for objects\n"));

      // 6. activate the POA manager
      poamgr->activate ();

      // 7. run the orb.
      ACE_DEBUG ((LM_DEBUG, "mixed_security/server: "
                  "running the orb\n"));
      orb->run ();
    }
  catch (const RirFailedException& e)
    {
      ACE_ERROR ((LM_ERROR,
                  "mixed_security/server: resolve_initial_references"
                  " failed for %s\n",
                  e.token_));
      return 1;
    }
  catch (const IORPublicationException& e)
    {
      ACE_ERROR ((LM_ERROR,
                  "mixed_security/server: failed to publish IOR (%s)\n",
                  (e.why_ == IORPublicationException::BAD_PARAM) ? "BAD_PARAM" :
                  (e.why_ == IORPublicationException::IO_FAILURE) ? "IO_FAILURE":
                  "<unknown>"));
    }
  catch (CORBA::Exception& ex)
    {
      ex._tao_print_exception
        ("mized_security/server: caught unexpected exception ");
      return 1;
    }

  // Do I need to delete server1 and server2, or, since they're refcounted,
  // will they get automatically cleaned up on POA destruction?
  orb->destroy ();

  return 0;
}
