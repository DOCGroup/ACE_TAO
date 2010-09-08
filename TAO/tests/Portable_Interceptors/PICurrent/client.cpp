// -*- C++ -*-
// $Id$

#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

#include "testC.h"
#include "ClientORBInitializer.h"

#include "tao/ORBInitializer_Registry.h"
#include "tao/PI/PI.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

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
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage: %s "
                           "-k IOR "
                           "\n",
                           argv[0]),
                          -1);
      }
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      PortableInterceptor::ORBInitializer_ptr temp_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      ACE_NEW_RETURN (temp_initializer,
                      ClientORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         "client_orb");

      if (parse_args (argc, argv) != 0)
        return 1;

      // Get the PICurrent object.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("PICurrent");

      PortableInterceptor::Current_var pi_current =
        PortableInterceptor::Current::_narrow (obj.in ());

      if (CORBA::is_nil (pi_current.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%P|%t) ERROR: Could not resolve "
                             "PICurrent object.\n"),
                            -1);
        }

      // Insert some data into the allocated PICurrent slot.
      CORBA::Any data;
      CORBA::Long number = 46;

      data <<= number;

      // Now reset the contents of our slot in the thread-scope
      // current (TSC).
      pi_current->set_slot (::slot_id,
                            data);

      // Resolve the target object, and perform the invocation.
      obj =
        orb->string_to_object (ior);

      PICurrentTest::test_var server =
        PICurrentTest::test::_narrow (obj.in ());

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior),
                            1);
        }

      // BUG 2656 testing - _get_policy_overrides() should return an empty
      // sequence rather than nill.
      CORBA::PolicyTypeSeq types;
      CORBA::PolicyList_var policies = server->_get_policy_overrides(types);

      if (policies.ptr () == 0)
        {
            ACE_ERROR ((LM_ERROR,
                        "(%P|%t) _get_policy_overrides returned nill pointer\n"));
            throw CORBA::INTERNAL ();
        }
      else
        {
          CORBA::ULong const list_size = policies->length();
          if (list_size != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "(%P|%t) _get_policy_overrides returned list with size not equal 0\n"));
              throw CORBA::INTERNAL ();
            }
        }

      server->invoke_me ();

      CORBA::Any_var new_data =
        pi_current->get_slot (::slot_id);

      // The original data in the TSC was of type CORBA::Long.  If the
      // following extraction from the CORBA::Any fails, then the
      // original data in the TSC was not replaced within the client
      // request interceptor, as this test should do.
      const char *str = 0;
      if (new_data.in () >>= str)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) Retrieved \"%C\" from the TSC.\n",
                      str));
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Unable to extract data (a string) "
                      "from the TSC.\n"));

          throw CORBA::INTERNAL ();
        }

      server->invoke_we ();

      server->shutdown ();

      ACE_OS::sleep(1);
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("PICurrent test (client-side):");
      return -1;
    }

  return 0;
}
