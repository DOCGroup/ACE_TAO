// -*- C++ -*-

#include "ace/Get_Opt.h"

#include "testC.h"
#include "ClientORBInitializer.h"

ACE_RCSID (PICurrent,
           client,
           "$Id$")

const char *ior = "file://test.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
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
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      PortableInterceptor::ORBInitializer_ptr temp_initializer =
        PortableInterceptor::ORBInitializer::_nil ();

      ACE_NEW_RETURN (temp_initializer,
                      ClientORBInitializer,
                      -1);  // No exceptions yet!
      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         "client_orb"
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      // Get the PICurrent object.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("PICurrent"
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableInterceptor::Current_var pi_current =
        PortableInterceptor::Current::_narrow (obj.in ()
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
                            data
                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Resolve the target object, and perform the invocation.
      obj =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PICurrentTest::test_var server =
        PICurrentTest::test::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil.\n",
                             ior),
                            1);
        }

      server->invoke_me (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Any_var new_data =
        pi_current->get_slot (::slot_id
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // The original data in the TSC was of type CORBA::Long.  If the
      // following extraction from the CORBA::Any fails, then the
      // original data in the TSC was not replaced within the client
      // request interceptor, as this test should do.
      const char *str = 0;
      if (new_data.in () >>= str)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) Retrieved \"%s\" from the TSC.\n",
                      str));
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Unable to extract data (a string) "
                      "from the TSC.\n"));

          ACE_TRY_THROW (CORBA::INTERNAL ());
        }

      server->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "PICurrent test (client-side):");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
