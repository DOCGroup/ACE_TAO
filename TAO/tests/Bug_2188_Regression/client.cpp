// $Id$

#include "brokenC.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Hello, client, "$Id$")

const char *ior = "file://broken.ior";

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
  ServerAdmin_var saobj;
  CORBA::ORB_var orb;

  ACE_TRY_NEW_ENV
    {
      orb = CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object(ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      saobj = ServerAdmin::_narrow (tmp.in() ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil (saobj.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil ServerAdmin reference <%s>\n",
                             ior),
                            1);
        }

      ArrayTest_var atobj = saobj->target(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CharArray_slice * char_array = CharArray_alloc();
      for (int i = 0; i < 10; i++)
        char_array[i] = 'a' + i;
      atobj->a_charArray (char_array ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "a_charArray worked OK.\n"));

      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
    }
  ACE_ENDTRY;

  saobj->shutdown();

  orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  return 0;
}
