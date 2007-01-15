// -*- C++ -*-

#include "ace/Get_Opt.h"
#include "testC.h"
#include "orbsvcs/FaultTolerance/FT_Service_Activate.h"

ACE_RCSID (Group_Ref_Manip,
           client,
           "$Id$")

const char *ior = 0;


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
                           "Usage:  %s "
                           "-k IOR_1 -k IOR_2\n",
                           argv[0]),
                          -1);
      }

  return 0;
}

int
main (int argc, char *argv[])
{
  int status = 0;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            "Client ORB"
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (::parse_args (argc, argv) != 0)
        return -1;

      // Start out with the first IOR.  Interaction with the second
      // IOR occurs during the various interceptions executed during
      // this test.
      CORBA::Object_var object =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ForwardRequestTest::test_var server =
        ForwardRequestTest::test::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Before and after the LOCATION_FORWARD_PERM the marshaled
      // object reference must differ.

      // Create a stringified/marshaled snapshot of Object reference
      CORBA::String_var marshaled_obj_snapshot1 =
          orb->object_to_string (server.in () ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      CORBA::ULong number = 0;
      for (int i = 1; i <= 25; ++i)
        {
          ACE_DEBUG ((LM_INFO,
                      "CLIENT: Issuing request %d.\n",
                      i));

          number += server->number (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_DEBUG ((LM_INFO,
                      "CLIENT: Number %d .\n",
                      number));



        }

      if (number < 250)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR: Did not forward to new location \n"));
          ACE_OS::abort ();
        }

      // One of the request triggerd a LOCATION_FORWARD_PERM, in
      // consequence the marshaled representation of "server" should
      // look different now, compare to snapshot1.
      CORBA::String_var marshaled_obj_snapshot2 =
          orb->object_to_string (server.in () ACE_ENV_ARG_PARAMETER);

      if (strcmp (marshaled_obj_snapshot1.in (), marshaled_obj_snapshot2.in ()) == 0)
        {
          // Error, before and after the marhaled object references look equal
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR: Marshaled Object reference should differ after LOCATION_FORWARD_PERM\n"));
          ACE_OS::abort ();
        }

      server->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return -1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_INFO,
              "Group_Ref_Manip Test passed.\n"));

  return status;
}
