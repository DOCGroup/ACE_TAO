// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tests/Smart_Proxy
//
// = FILENAME
//     client.cpp
//
// = DESCRIPTION
//     This is the client program that tests TAO's Smart Proxy extension.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
//=========================================================================

#include "ace/Get_Opt.h"
#include "testC.h"
#include "Smart_Proxy_Impl.h"

ACE_RCSID(Smart_Proxy, client, "$Id$")

const char *ior1 = "file://test1.ior";
const char *ior2 = "file://test2.ior";
int one_shot_factory = 1;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "i:j:f:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        ior1 = ACE_OS::strdup (get_opts.opt_arg ());
        break;
      case 'j':
        ior2 = ACE_OS::strdup (get_opts.opt_arg ());
        break;
       case 'f':
        one_shot_factory = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-i -j -f"
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}

int
run_test (CORBA::ORB_ptr orb_ptr,
          int target)
{
  CORBA::ORB_var orb = CORBA::ORB::_duplicate (orb_ptr);
  CORBA::Object_var object;
  ACE_TRY_NEW_ENV
    {
      if (target == 1)
        {
          object =
            orb->string_to_object (ior1
                                   TAO_ENV_ARG_PARAMETER);
        }
      else
        {
          object =
            orb->string_to_object (ior2
                                   TAO_ENV_ARG_PARAMETER);
        }
      ACE_TRY_CHECK;

      Test_var server =
        Test::_narrow (object.in ()
                       TAO_ENV_ARG_PARAMETER);
      if (CORBA::is_nil (server.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Object reference is nil\n"),
                          1);

      server->method (0);
      server->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);

      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Client-side exception:");
    }
  ACE_ENDTRY;
return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         ""
                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      // To use the smart proxy it is necessary to allocate the
      // user-defined smart factory on the heap as the smart proxy
      // generated classes take care of destroying the object. This
      // way it a win situation for the application developer who
      // doesnt have to make sure to destoy it and also for the smart
      // proxy designer who now can manage the lifetime of the object
      // much surely.
      // By default this factory is permanent (i.e. registered for
      // this interface) but if there is a need for flexibility per
      // object instance then <one_shot_factory> needs to be set to 0.
      Smart_Test_Factory *test_factory = 0;
      ACE_NEW_RETURN (test_factory,
                      Smart_Test_Factory (one_shot_factory),
                      -1);

      // To make KAI Compiler happy as it considers <test_factory> to be
      // an unused variable.
      ACE_UNUSED_ARG (test_factory);

      run_test (orb.in (), 1);
      run_test (orb.in (), 2);

      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Client-side exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
