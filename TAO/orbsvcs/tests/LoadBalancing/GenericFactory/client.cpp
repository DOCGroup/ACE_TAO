// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"

ACE_RCSID(GenericFactory, client, "$Id$")

const char *ior = "file://obj.ior";

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

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Basic_var basic =
        Test::Basic::_narrow (tmp.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (basic.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Basic reference <%s>\n",
                             ior),
                            1);
        }

      // @@ Jai, make multiple invocations to force the load balancer
      //    to return forward the client to a different servant.  It's
      //    not crucial to do this.  Doing so is just meant to
      //    exercise the LoadManager internals a bit.  For example,
      //    try putting the below get_string() call in a loop that
      //    executes 10 times.
      CORBA::String_var the_string =
        basic->get_string (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%s>\n",
                  the_string.in ()));

      // @@ Jai, something to think about...  You're shutting down the
      //    servant, but its object reference is still registered with
      //    the LoadManager.  What does that mean?  :-)
      basic->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
