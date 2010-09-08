// $Id$

#include "ace/Get_Opt.h"
#include "Multiple_Inheritance_i.h"
#include "tao/Utils/ORB_Manager.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"

static ACE_TCHAR *ior_output_file = 0;

Multiple_Inheritance_i::Multiple_Inheritance_i (void)
{
  // Sun/CC 5.0 crashes if there is no explicit default
  // constructor
  // Default constructor, do nothin..
}

int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("f:"));
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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Multiple_Inheritance_i servant;
  TAO_ORB_Manager orb_manager;

  ACE_DEBUG ((LM_DEBUG, "\n\tMultiple Inheritance Server\n\n"));
  try
    {
      orb_manager.init_child_poa (argc,
                                  argv,
                                  "child_poa");

      if (parse_args (argc, argv) != 0)
        return -1;

      CORBA::String_var ior =
        orb_manager.activate_under_child_poa ("my_object",
                                              &servant);

      ACE_DEBUG ((LM_DEBUG, "%C\n",
                  ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ior_output_file),
                              -1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      orb_manager.run ();
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("System Exception");
      return -1;
    }

  return 0;
}
