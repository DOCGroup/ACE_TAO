// $Id$

//==========================================================================
//
//
// = FILENAME
//     client.cpp
//
// = DESCRIPTION
//
//     This is a simple foo client implementation.  Also looks out for
//     forwarding exceptions
//
// = AUTHOR
//     Irfan Pyarali
//
//==========================================================================

#include "ace/streams.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "FooC.h"

ACE_RCSID(Forwarding, client, "$Id$")

static char *IOR = 0;
static int servers = 2;
static int iterations = 3;

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "s:i:k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        IOR = get_opts.opt_arg ();
        break;
      case 's':
        servers = ::atoi (get_opts.opt_arg ());
        break;
      case 'i':
        iterations = ::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           "-k IOR\n"
                           "-i iterations\n"
                           "\n",
                           argv [0]),
                          -1);
      }

  if (IOR == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Please specify the IOR for the servant\n"),
                      -1);

  // Indicates successful parsing of command line.
  return 0;
}

void
do_calls (Foo_ptr foo
          ACE_ENV_ARG_DECL)
{
  for (int j = 1; j <= servers; j++)
    {
      for (int i = 1; i <= iterations; i++)
        {
          // Invoke the doit() method of the foo reference.
          CORBA::Long result = foo->doit (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

          // Print the result of doit () method of the foo reference.
          ACE_DEBUG ((LM_DEBUG,
                      "doit() returned %d \n",
                      result));
        }

      // Don't forward the last server
      if (j != servers)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Asking server to forward next time\n"));
          foo->forward (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;
        }
    }
}


int
main (int argc, char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Initialize the ORB
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Initialize options based on command-line arguments.
      int parse_args_result =
        parse_args (argc, argv);
      if (parse_args_result != 0)
        return parse_args_result;

      // Get an object reference from the argument string.
      CORBA::Object_var object =
        orb->string_to_object (IOR ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Try to narrow the object reference to a Foo reference.
      Foo_var foo =
        Foo::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      do_calls (foo.in ()
                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      foo->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
