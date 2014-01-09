// $Id$

#include "common.h"
#include "tao/ORB.h"
#include "ace/Get_Opt.h"
#include "ace/ARGV.h"

ACE_TCHAR const *orb1_args = 0;
ACE_TCHAR const *orb2_args = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("a:b:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
        orb1_args = get_opts.opt_arg ();
        break;
      case 'b':
        orb2_args = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s ")
                           ACE_TEXT ("-a <orb1_args> ")
                           ACE_TEXT ("-b <orb2_args>")
                           ACE_TEXT ("\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
do_main (int argc, ACE_TCHAR *argv[], orbs_check_callback check)
{
  try
    {
      if (parse_args (argc, argv) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "ERROR: Cannot process args\n"),
                            1);
        }

      ACE_ARGV orb1_argv (orb1_args);
      int orb1_argc = orb1_argv.argc ();

      ACE_ARGV orb2_argv (orb2_args);
      int orb2_argc = orb2_argv.argc ();

      ACE_DEBUG ((LM_DEBUG,
                  "Initialize ORB instances...\n"));

      CORBA::ORB_var orb1 =
        CORBA::ORB_init (orb1_argc, orb1_argv.argv (), FIRST_ORB_NAME);

      CORBA::ORB_var orb2 =
        CORBA::ORB_init (orb2_argc, orb2_argv.argv (), SECOND_ORB_NAME);

      ACE_DEBUG ((LM_DEBUG,
                  "After ORB_init...\n"));

      if (check && !check ())
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "ERROR: ORB check failed\n"),
                            1);
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught Exception in do_main:\n");
      return 1;
    }

  return 0;
}
