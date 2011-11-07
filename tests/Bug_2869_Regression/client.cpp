// $Id$

#include "tao/ORB.h"
#include "tao/Object.h"
#include "tao/SystemException.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT("test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior_output_file = get_opts.opt_arg ();
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
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var tmp =
        orb->string_to_object("iiop://1\"/2$/$3211:2500/EndPoint");
    }
  catch (const CORBA::INV_OBJREF&)
    {
      ACE_DEBUG ((LM_DEBUG, "Test succeeded\n"));
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, "Caught invalid exception\n"));
      return 1;
    }

  // Write dummy file to trigger the test framework we are ready.
  FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
  if (output_file == 0)
     ACE_ERROR_RETURN ((LM_ERROR,
                        "Cannot open output file for writing IOR: %s\n",
                         ior_output_file),
                         1);
  ACE_OS::fprintf (output_file, "dummy");
  ACE_OS::fclose (output_file);

  return 0;
}
