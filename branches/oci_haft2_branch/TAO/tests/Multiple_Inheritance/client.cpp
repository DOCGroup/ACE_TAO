// $Id$
//
//===================================================================
//  = LIBRARY
//      TAO/tests/Multiple_Inheritance
//
//  = FILENAME
//     client.cpp
//
//  = DESCRIPTION
//      A client program for the Multiple Inheritance module
//
//  = AUTHOR
//     Irfan Pyarali
//
//====================================================================

#include "ace/streams.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "Multiple_InheritanceC.h"

ACE_RCSID(Multiple_Inheritance, client, "$Id$")

static char *ior = 0;
static char *ior_input_file = 0;

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "k:f:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 'f':
        ior_input_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "\nusage:  %s "
                           "-i <ior_input_file> "
                           "-k IOR "
                           "\n",
                           argv [0]),
                          -1);
      }

  if (ior == 0 && ior_input_file == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "\nPlease specify the IOR or IOR input file"
                       " for the servant"),
                      -1);
  if (ior != 0 && ior_input_file != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "\nPlease specify only an IOR or only an IOR"
                       " input file but not both"),
                      -1);

  // Indicates successful parsing of the command line.
  return 0;
}

int
main (int argc, char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Initialize the ORB
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Parse the command-line arguments to get the IOR
      parse_args (argc, argv);

      // If ior_input_file exists, Read the file, and get the IOR
      // else, it must have been specified on the command line
      if (ior_input_file != 0)
        {
          ACE_HANDLE input_file = ACE_OS::open (ior_input_file, 0);
          if (input_file == ACE_INVALID_HANDLE)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open input file for reading IOR: %s\n",
                               ior_input_file),
                              -1);
          ACE_Read_Buffer ior_buffer (input_file);
          char *data = ior_buffer.read ();
          if (data == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Unable to read ior\n"),
                              -1);
          ior = ACE_OS::strdup (data);
          ior_buffer.alloc ()-> free (data);
          ACE_OS::close (input_file);
        }

      // Get the object reference with the IOR
      CORBA::Object_var object = orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var string;

      // Narrow the object reference
      A_var a = A::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Narrow the object reference
      B_var b = B::_narrow (a.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Narrow the object reference
      C_var c = C::_narrow (a.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Narrow the object reference
      D_var d = D::_narrow (c.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      string = a->method1 (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

      string = b->method1 (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

      string = b->method2 (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

      string = c->method1 (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

      string = c->method3 (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

      string = d->method1 (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

      string = d->method2 (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

      string = d->method3 (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));

      string = d->method4 (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG, "%s\n", string.in ()));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "client");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
