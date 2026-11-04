#include "common.h"
#include "tao/ORB.h"
#include "ace/Get_Opt.h"
#include "ace/ARGV.h"

#include <string>

namespace
{
constexpr char const first_orb_name[] = "first_orb";
constexpr char const second_orb_name[] = "second_orb";
}

int
parse_args (int argc,
            ACE_TCHAR *argv[],
            std::basic_string<ACE_TCHAR> &orb1_args,
            std::basic_string<ACE_TCHAR> &orb2_args)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("a:b:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
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
do_main (int argc, ACE_TCHAR *argv[], orbs_check_callback const check)
{
  try
    {
      std::basic_string<ACE_TCHAR> orb1_args {};
      std::basic_string<ACE_TCHAR> orb2_args {};
      if (parse_args (argc, argv, orb1_args, orb2_args) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "ERROR: Cannot process args\n"),
                            1);
        }

      ACE_ARGV orb1_argv {orb1_args.c_str ()};
      int orb1_argc = orb1_argv.argc ();

      ACE_ARGV orb2_argv {orb2_args.c_str ()};
      int orb2_argc = orb2_argv.argc ();

      ACE_DEBUG ((LM_DEBUG,
                  "Initialize ORB instances...\n"));

      CORBA::ORB_var const orb1 {
        CORBA::ORB_init (orb1_argc, orb1_argv.argv (), first_orb_name)};

      CORBA::ORB_var const orb2 {
        CORBA::ORB_init (orb2_argc, orb2_argv.argv (), second_orb_name)};

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
