// $Id$

#include "ace/Get_Opt.h"
#include "Multiple_Inheritance_i.h"
#include "tao/Utils/ORB_Manager.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/Argv_Type_Converter.h"

ACE_RCSID(Multiple_Inheritance, server, "$Id$")

static char *ior_output_file = 0;

Multiple_Inheritance_i::Multiple_Inheritance_i (void)
{
  // Sun/CC 5.0 crashes if there is no explicit default
  // constructor
  // Default constructor, do nothin..
}

int
parse_args (int argc, char **argv)
{
  ACE_Get_Arg_Opt<char> get_opts (argc, argv, "f:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'f':
        ior_output_file = ACE_OS::strdup (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-f <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}


int
ACE_TMAIN (int argc, ACE_TCHAR **argv)
{
  ACE_Argv_Type_Converter convert (argc, argv);

  Multiple_Inheritance_i servant;
  TAO_ORB_Manager orb_manager;

  ACE_DEBUG ((LM_DEBUG, "\n\tMultiple Inheritance Server\n\n"));
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      orb_manager.init_child_poa (convert.get_argc(), convert.get_ASCII_argv(),
                                  "child_poa"
                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (convert.get_argc(), convert.get_ASCII_argv()) != 0)
        return -1;

      CORBA::String_var ior =
        orb_manager.activate_under_child_poa ("my_object",
                                              &servant
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "%s\n",
                  ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, ACE_TEXT("w"));
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ior_output_file),
                              -1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      orb_manager.run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex, "System Exception");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
